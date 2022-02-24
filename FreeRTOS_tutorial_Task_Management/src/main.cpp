/**
 * ######################### FREERTOS TASK SCHEDULING #########################
 * Create two tasks
 * 1. First Task should read the serial monitor for user inputs.
 * 2. Second task should change the blinking rate of a LED according to those
 *    serial inputs.
 */


#include <Arduino.h>

// Configuring ESP32 to use only one core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t appcpu = 0;
#else
static const BaseType_t appcpu = 1;
#endif

// Task handle for task 1
static TaskHandle_t SERIAL_READ_TASK_HANDLE = NULL;

// Task handle for task 2
static TaskHandle_t CHANGE_BLINK_RATE_TASK_HANDLE = NULL;

// LED goes to GPIO 17
#define LED 17

// Blinking rate for the LED
static int BLINK_RATE = 100;

// Task to read the serial input
void task_SerialRead(void *parameter)
{
  // infinite loop
  for (;;)
  {
    if (Serial.available())
    {
      String rate = Serial.readString();
      BLINK_RATE = rate.toInt();
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
  }
}

// Task to chage the blinking rate of the LED
void task_ChangeRate(void *parameter)
{
  for (;;)
  {
    Serial.println(BLINK_RATE);
    digitalWrite(LED, HIGH);
    vTaskDelay(BLINK_RATE / portTICK_PERIOD_MS);
    // delay(BLINK_RATE);
    digitalWrite(LED, LOW);
    // delay(BLINK_RATE);
    vTaskDelay(BLINK_RATE / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  // Start with 2 second delay
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  xTaskCreatePinnedToCore(task_SerialRead,          // Function name
                          "Serial Read task",       // Describe name for debugging
                          1000,                     // Stack size
                          NULL,                     // Parameters
                          1,                        // Priority level
                          &SERIAL_READ_TASK_HANDLE, // Task handle
                          appcpu);                  // core number

  xTaskCreatePinnedToCore(task_ChangeRate,
                          "Change Blink Rate Task",
                          1000,
                          NULL,
                          1,
                          &CHANGE_BLINK_RATE_TASK_HANDLE,
                          appcpu);
}

void loop()
{
  // put your main code here, to run repeatedly:
}