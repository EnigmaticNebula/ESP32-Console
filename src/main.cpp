#include <Arduino.h>
#include <iostream>
#include <algorithm>
#include <memory>
#include <led_array_driver.hpp>
#include <pin_definitions.hpp>
#include <menu_handler\menu_handler.hpp>
#include <games\conways\conways.hpp>
#include <games\naughts_crosses\naughts_crosses.hpp>
#include <games\game.hpp>
#include <games\snake\snake.hpp>
using namespace std;

// Function declarations
static void matrix_refresh(void* pvParameters);
static void game_loop(void* pvParameters);
void IRAM_ATTR button_1_isr();
void IRAM_ATTR button_2_isr();
void IRAM_ATTR button_3_isr();
void IRAM_ATTR nav_up();
void IRAM_ATTR nav_right();
void IRAM_ATTR nav_down();
void IRAM_ATTR nav_left();
void IRAM_ATTR nav_act();
void IRAM_ATTR encoder_a_change();
void IRAM_ATTR encoder_b_change();

// Buffers
bool game_buffer[16][16];
bool display_buffer[16][16];
int display_pixel_brightness[16][16];
bool (*game_buffer_ptr)[16] = game_buffer;
bool (*display_buffer_ptr)[16] = display_buffer;
int (*display_pixel_brightness_ptr)[16] = display_pixel_brightness;

// Inputs / Debounce timers
unsigned int last_iteration = 0;
unsigned int last_speed_check = 0;
unsigned int iteration_delay = 0;

volatile bool button_1_pressed = false;
volatile bool cleared = false;
volatile bool iterate = false;

volatile unsigned int last_button_1_press = 0;
volatile unsigned int last_button_2_press = 0;
volatile unsigned int last_button_3_press = 0;
volatile unsigned int last_nav_up_press = 0;
volatile unsigned int last_nav_right_press = 0;
volatile unsigned int last_nav_down_press = 0;
volatile unsigned int last_nav_left_press = 0;
volatile unsigned int last_nav_action_press = 0;
volatile unsigned int last_encoder_a_change = 0;
volatile unsigned int last_encoder_b_change = 0;
volatile unsigned int last_joystick_input = 0; // This variable is necessary to prevent joystick spam and filter out unnecessary joystick actions when moving left, right and down (excl. up)

volatile bool clockwise_rotation = false;
volatile bool anticlockwise_rotation = false;

const unsigned int BUTTON_DEBOUNCE_DELAY = 150;
const unsigned int JOYSTICK_DEBOUNCE_DELAY = 100;
const unsigned int JOYSTICK_ACTION_DELAY = 200; // Rejects joystick actions occuring within x ms of another

const unsigned int EXIT_GAME_BUTTON_WINDOW = 200; // Defines the window in which button 2 must be pressed after button 1 (or vice versa) when exiting to menu from within a game
bool button1_game_exit = false;
bool button2_game_exit = false;

// Game instantiations
const unsigned int GAME_COUNT = 11;
array<unique_ptr<Game>, GAME_COUNT> games;

// Class instantiations
LedDriver led_driver{display_buffer_ptr, display_pixel_brightness_ptr};
MenuHandler menu_handler{game_buffer_ptr, display_pixel_brightness_ptr};
SemaphoreHandle_t buffer_mutex;

void setup() {
  // Pin definitions

  //-- Input pins
  pinMode(ITERATION_SPEED_PIN, INPUT);
  pinMode(BUTTON_3_PIN, INPUT_PULLUP); 
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP); 

  //-- Joystick pins
  pinMode(NAV_UP_PIN, INPUT_PULLUP);
  pinMode(NAV_LEFT_PIN, INPUT_PULLUP);
  pinMode(NAV_RIGHT_PIN, INPUT_PULLUP); 
  pinMode(NAV_ACTION_PIN, INPUT_PULLUP); 
  pinMode(NAV_DOWN_PIN, INPUT_PULLUP); 
  pinMode(ROTARY_ENCODER_A_PIN, INPUT_PULLUP); 
  pinMode(ROTARY_ENCODER_B_PIN, INPUT_PULLUP); 

  //-- Array driver pins
  pinMode(HIGH_SIDE_SERIAL_PIN, OUTPUT); 
  pinMode(HIGH_SIDE_SRCLK_PIN, OUTPUT); 
  pinMode(HIGH_SIDE_RCLK_PIN, OUTPUT);
  pinMode(LOW_SIDE_CLK_PIN, OUTPUT);
  pinMode(LOW_SIDE_LATCH_PIN, OUTPUT);
  pinMode(LOW_SIDE_SERIAL_PIN, OUTPUT);

  buffer_mutex = xSemaphoreCreateMutex();
  games[0].reset(new Conways{game_buffer_ptr, display_buffer_ptr, display_pixel_brightness_ptr});
  games[1].reset(new NaughtsCrosses{game_buffer_ptr, display_buffer_ptr, display_pixel_brightness_ptr});
  games[2].reset(new Snake{game_buffer_ptr, display_buffer_ptr, display_pixel_brightness_ptr});
  Serial.begin(9600);
  menu_handler.init_menu();
  // Tasks

  // Note: NEVER pin the matrix refresh task to core 0 
  xTaskCreatePinnedToCore(
    matrix_refresh,
    "LedDriver",
    10000,
    NULL,
    3, // Do not exceed priority of ~20 as core functions (Wi-Fi, BT, scheduling) operate at these priorities 
    NULL,
    1
  );

  xTaskCreatePinnedToCore(
    game_loop,
    "SimulationLoop",
    10000,
    NULL,
    0,
    NULL,
    0
  );
}

void loop() {

}

static void matrix_refresh(void* pvParameters) {
  for (;;) {
    // Ensure that buffers are not currently being used by the simulation loop
    if (xSemaphoreTake(buffer_mutex, (TickType_t) 10) == pdTRUE) {
      led_driver.refresh();
      xSemaphoreGive(buffer_mutex);
    }
    // Prevent task watchdog timer panics
    vTaskDelay(pdMS_TO_TICKS(1)); 
  }
}

static void game_loop(void* pvParameters) {
  // Input interrupts

  // Note that whether a pin gets pulled high (+3.3V) or low (GND) on input is what determines whether the interrupt should trigger on the falling or rising edge.
  // i.e. pulled high = rising edge, pulled low = falling edge
  attachInterrupt(digitalPinToInterrupt(BUTTON_3_PIN), button_3_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_2_PIN), button_2_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_1_PIN), button_1_isr, FALLING);
  attachInterrupt(digitalPinToInterrupt(NAV_UP_PIN), nav_up, FALLING);
  attachInterrupt(digitalPinToInterrupt(NAV_RIGHT_PIN), nav_right, FALLING);
  attachInterrupt(digitalPinToInterrupt(NAV_DOWN_PIN), nav_down, FALLING);
  attachInterrupt(digitalPinToInterrupt(NAV_LEFT_PIN), nav_left, FALLING);
  attachInterrupt(digitalPinToInterrupt(NAV_ACTION_PIN), nav_act, FALLING);
  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN), encoder_a_change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_B_PIN), encoder_b_change, CHANGE);

  for (;;) {
    /*
    memcpy operation MUST occur before the next game iteration. Some components (e.g. cursor),
    operate directly on the display buffer, skipping the game buffer. If a change to
    the display buffer is made during this operation, changes may not be displayed for
    certain pixels.
    */
    if (xSemaphoreTake(buffer_mutex, portMAX_DELAY) == pdTRUE) {
      memcpy(display_buffer_ptr, game_buffer_ptr, 16*16*sizeof(bool));
      
      xSemaphoreGive(buffer_mutex);
    }

    unsigned int current_time = millis();
    if (current_time - last_speed_check >= 100) {
      last_speed_check = current_time;
      int raw_potentiometer_output = analogRead(ITERATION_SPEED_PIN);
      if (!menu_handler.menu_active) {
        games[menu_handler.selected_game]->potentiometer_change(raw_potentiometer_output);
      }
    }

    if (button1_game_exit && button2_game_exit) {
      games[menu_handler.selected_game]->unload();
      menu_handler.show_menu();
      menu_handler.init_menu();
    }

    if (current_time - last_button_1_press >= EXIT_GAME_BUTTON_WINDOW) {
      button1_game_exit = false;
    }

    if (current_time - last_button_2_press >= EXIT_GAME_BUTTON_WINDOW) {
      button2_game_exit = false;
    }

    if (menu_handler.menu_active) {
      menu_handler.refresh();
    } else {
      games[menu_handler.selected_game]->iterate();
    }
    
    vTaskDelay(pdMS_TO_TICKS(1)); // Prevent task watchdog timer panics
  }
}

void clear_game_buffer() {
  if (xSemaphoreTake(buffer_mutex, portMAX_DELAY) == pdTRUE) {
      for (int i = 0; i < 16; i++) {
        for (int j = 0; j < 16; j++) {
          game_buffer[i][j] = false;
        }
    }
    xSemaphoreGive(buffer_mutex);
  }
}

void reset_pixel_brightness() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 16; j++) {
      display_pixel_brightness_ptr[i][j] = 7;
    }
  }
}

void IRAM_ATTR button_1_isr() {
  unsigned int current_time = millis();
  if (current_time - last_button_1_press >= BUTTON_DEBOUNCE_DELAY) {
    last_button_1_press = current_time;
    if (menu_handler.menu_active) {
      reset_pixel_brightness();
      games[menu_handler.selected_game]->load();
      menu_handler.hide_menu();
    } else {
      games[menu_handler.selected_game]->button1();
      button1_game_exit = true;
    }
  }
}

void IRAM_ATTR button_2_isr() {
  unsigned int current_time = millis();
  if (current_time - last_button_2_press >= BUTTON_DEBOUNCE_DELAY) {
    last_button_2_press = current_time;
    if (!menu_handler.menu_active) {
      games[menu_handler.selected_game]->button2();
      button2_game_exit = true;
    }
  }
}

void IRAM_ATTR button_3_isr() {
  unsigned int current_time = millis();
  if (current_time - last_button_3_press >= BUTTON_DEBOUNCE_DELAY) {
    last_button_3_press = current_time;
    if (!menu_handler.menu_active) {
      games[menu_handler.selected_game]->button3();
    }
  }
}

void IRAM_ATTR nav_up() {
  Serial.println("UP");
  unsigned int current_time = millis();
  if (current_time - last_nav_up_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_up_press = current_time;
    last_joystick_input = current_time;
    if (!menu_handler.menu_active) {
      games[menu_handler.selected_game]->joystick_up();
    }
  }
}

void IRAM_ATTR nav_right() {
  Serial.println("RIGHT");
  unsigned int current_time = millis();
  if (current_time - last_nav_right_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_right_press = current_time;
    last_joystick_input = current_time;
    if (!menu_handler.menu_active) {
      games[menu_handler.selected_game]->joystick_right();
    }
  }
}

void IRAM_ATTR nav_down() {
  Serial.println("DOWN");
  unsigned int current_time = millis();
  if (current_time - last_nav_down_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_down_press = current_time;
    last_joystick_input = current_time;
    if (!menu_handler.menu_active) {
      games[menu_handler.selected_game]->joystick_down();
    }
  }
}

void IRAM_ATTR nav_left() {
  Serial.println("LEFT");
  unsigned int current_time = millis();
  if (current_time - last_nav_left_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_left_press = current_time;
    last_joystick_input = current_time;
    if (!menu_handler.menu_active) {
      games[menu_handler.selected_game]->joystick_left();
    }
  }
}

void IRAM_ATTR nav_act() {
  Serial.println("ACT");
  unsigned int current_time = millis();
  if (current_time - last_nav_action_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_action_press = current_time;
    if (!menu_handler.menu_active) {
      games[menu_handler.selected_game]->joystick_action();
    }
  }
}

void IRAM_ATTR encoder_a_change() {
  unsigned int current_time = millis();
  if (current_time - last_encoder_a_change >= JOYSTICK_DEBOUNCE_DELAY) {
    if (!anticlockwise_rotation) {
      clockwise_rotation = true;
      if (menu_handler.menu_active && menu_handler.selected_game < GAME_COUNT)  {
        menu_handler.next_game();
      } else if (!menu_handler.menu_active) {
        games[menu_handler.selected_game]->rotary_encoder_clockwise();
      }
    } else {
      anticlockwise_rotation = false;
    }
    last_encoder_a_change = current_time;
  }
}

void IRAM_ATTR encoder_b_change() {
  unsigned int current_time = millis();
  if (current_time - last_encoder_b_change >= JOYSTICK_DEBOUNCE_DELAY) {
    if (!clockwise_rotation) {
      anticlockwise_rotation = true;
      if (menu_handler.selected_game > 0 && menu_handler.menu_active) {
        menu_handler.previous_game();
      } else if (!menu_handler.menu_active) {
        games[menu_handler.selected_game]->rotary_encoder_anticlockwise();
      }
    } else {
      clockwise_rotation = false;
    }
    last_encoder_b_change = current_time;
  }
}