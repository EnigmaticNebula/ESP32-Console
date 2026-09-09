#include <Arduino.h>
#include <iostream>
#include <algorithm>
#include <led_array_driver.hpp>
#include <pin_definitions.hpp>
#include <menu_handler\menu_handler.hpp>
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
bool (*game_buffer_ptr)[16] = game_buffer;
bool (*display_buffer_ptr)[16] = display_buffer;

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

LedDriver led_driver{display_buffer_ptr};
MenuHandler menu_handler{game_buffer_ptr};

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

int get_iteration_speed() {
    int raw_potentiometer_output = analogRead(ITERATION_SPEED_PIN);
    int iteration_delay = ::map(raw_potentiometer_output, 0, 4095, 1000, 0);
    return iteration_delay;
}

static void matrix_refresh(void* pvParameters) {
  for (;;) {
    // Ensure that buffers are not currently being used by the simulation loop
    if (xSemaphoreTake(buffer_mutex, (TickType_t) 10) == pdTRUE) {
      led_driver.refresh();
      xSemaphoreGive(buffer_mutex);
    }
    // Prevent task watchdog timer panics
    vTaskDelay(pdMS_TO_TICKS(2)); 
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
  attachInterrupt(digitalPinToInterrupt(NAV_ACTION_PIN), nav_act, FALLING); // Nav action must be triggered on rising edge so actions always occur after joystick movements
  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_A_PIN), encoder_a_change, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ROTARY_ENCODER_B_PIN), encoder_b_change, CHANGE);

  for (;;) {
    unsigned int current_time = millis();
    if (current_time - last_speed_check >= 100) {
      last_speed_check = current_time;
      iteration_delay = get_iteration_speed();
    }

    if (xSemaphoreTake(buffer_mutex, portMAX_DELAY) == pdTRUE) {
      for (int i = 0; i < 16; i++) {
        memcpy(display_buffer_ptr, game_buffer_ptr, 16*16*sizeof(bool));
      }
      
      xSemaphoreGive(buffer_mutex);
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

void IRAM_ATTR button_1_isr() {
  unsigned int current_time = millis();
  if (current_time - last_button_1_press >= BUTTON_DEBOUNCE_DELAY) {
    Serial.println("BUTTON 1");
    last_button_1_press = current_time;
  }
}

void IRAM_ATTR button_2_isr() {
  unsigned int current_time = millis();
  if (current_time - last_button_2_press >= BUTTON_DEBOUNCE_DELAY) {
    Serial.println("BUTTON 2");
    last_button_2_press = current_time;
  }
}

void IRAM_ATTR button_3_isr() {
  unsigned int current_time = millis();
  if (current_time - last_button_3_press >= BUTTON_DEBOUNCE_DELAY) {
    Serial.println("BUTTON 3");
    last_button_3_press = current_time;
  }
}

void IRAM_ATTR nav_up() {
  unsigned int current_time = millis();
  if (current_time - last_nav_up_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_up_press = current_time;
    last_joystick_input = current_time;
    Serial.println("UP");
  }
}

void IRAM_ATTR nav_right() {
  unsigned int current_time = millis();
  if (current_time - last_nav_right_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_right_press = current_time;
    last_joystick_input = current_time;
  }
}

void IRAM_ATTR nav_down() {
  unsigned int current_time = millis();
  if (current_time - last_nav_down_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_down_press = current_time;
    last_joystick_input = current_time;
  }
}

void IRAM_ATTR nav_left() {
  unsigned int current_time = millis();
  if (current_time - last_nav_left_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_left_press = current_time;
    last_joystick_input = current_time;
  }
}

void IRAM_ATTR nav_act() {
  unsigned int current_time = millis();
  if (current_time - last_nav_action_press >= JOYSTICK_DEBOUNCE_DELAY) {
    last_nav_action_press = current_time;
    Serial.println("ACT");
  }
}

void IRAM_ATTR encoder_a_change() {
  unsigned int current_time = millis();
  if (current_time - last_encoder_a_change >= JOYSTICK_DEBOUNCE_DELAY) {
    if (!anticlockwise_rotation) {
      clockwise_rotation = true;
      if (menu_handler.menu_active) {
        Serial.println("a");
        menu_handler.next_game();
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
        Serial.println("b");
        menu_handler.previous_game();
      }
    } else {
      clockwise_rotation = false;
    }
    last_encoder_b_change = current_time;
  }
}