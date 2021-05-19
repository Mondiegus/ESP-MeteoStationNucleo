// #include "..\components\SparkFun_HTU21D_Humidity_and_Temperature_Sensor_Breakout\SparkFunHTU21D.h"
// #include "..\components\Adafruit_BMP085_Library\Adafruit_BMP085.h"
// #include "..\components\Adafruit_LiquidCrystal\Adafruit_LiquidCrystal.h"
// #include <hd44780.h>
#include <stdio.h>
#include "esp_sleep.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "hd44780.h"
#include "bmp180.h"
#include "si7021.h"
#include <string.h>

#define BLINK_GPIO 2

RTC_DATA_ATTR const int RS = 13, EN = 12, D4 = 14, D5 = 27, D6 = 26, D7 = 25, SDA = 21, SCL = 22;

// Adafruit_LiquidCrystal lcd(RS, EN, D0, D1, D2, D3);
// Adafruit_BMP085 bmp;
// HTU21D GTU21D;
// BMP180 myBMP(BMP180_STANDARD);
bmp180_dev_t dev;
i2c_dev_t dev2;

RTC_DATA_ATTR hd44780_t lcd =
    {
        .write_cb = NULL,
        .font = HD44780_FONT_5X8,
        .lines = 2,
        .pins = {
            .rs = RS,
            .e = EN,
            .d4 = D4,
            .d5 = D5,
            .d6 = D6,
            .d7 = D7,
            .bl = HD44780_NOT_USED}};
TaskHandle_t xHandle = NULL;

  float temp;
  uint32_t pressure;
  float val;
  esp_err_t res2;
  esp_err_t res;
  char out[10] = {0};
  RTC_DATA_ATTR  static uint8_t i = 0;
  
void setup()
{
  ESP_ERROR_CHECK(i2cdev_init());

  // put your setup code here, to run once:
  gpio_pad_select_gpio(BLINK_GPIO);
  /* Set the GPIO as a push/pull output */
  gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

  printf("Init\n");
  printf("%x\n\n", 16);
  gpio_set_level(BLINK_GPIO, 1);

  ESP_ERROR_CHECK(hd44780_init(&lcd));
  hd44780_gotoxy(&lcd, 0, 0);
  hd44780_puts(&lcd, "INITIALIZING");

  memset(&dev, 0, sizeof(bmp180_dev_t)); // Zero descriptor

  ESP_ERROR_CHECK(bmp180_init_desc(&dev, 0, SDA, SCL));
  ESP_ERROR_CHECK(bmp180_init(&dev));
  hd44780_clear(&lcd);
  hd44780_puts(&lcd, "INITIALIZed");

  memset(&dev2, 0, sizeof(i2c_dev_t));
  ESP_ERROR_CHECK(si7021_init_desc(&dev2, 0, SDA, SCL));
  // while (!bmp.begin()) //sda, scl
  // {
  //   printf(("Bosch BMP180/BMP085 is not connected or fail to read calibration coefficients"));
  //   delay(5000);
  // }

  // printf(("Bosch BMP180/BMP085 sensor is OK")); //(F()) saves string to flash & keeps dynamic memory
}

void loop(void *pvParameters)
{


  while (1)
  {
    /* ############# TEMP 1 ###########*/
    res = bmp180_measure(&dev, &temp, &pressure, BMP180_MODE_STANDARD);

    hd44780_clear(&lcd);
    hd44780_gotoxy(&lcd, 0, 0);
    snprintf(out, 9, "%.1f", temp);
    hd44780_puts(&lcd, "T1:");
    hd44780_puts(&lcd, out);
    hd44780_puts(&lcd, " ");
    memset(&out[0], 0, sizeof(out));

    /* ############# TEMP 2 ###########*/
    res2 = si7021_measure_temperature(&dev2, &val);

    if (res != ESP_OK)
      printf("Could not measure temperature: %d (%s)\n", res2, esp_err_to_name(res2));
    else
      printf("Temperature: %.2f\n", val);

    snprintf(out, 9, "%.1f", val);
    hd44780_puts(&lcd, "T1:");
    hd44780_puts(&lcd, out);
    hd44780_puts(&lcd, " ");
    memset(&out[0], 0, sizeof(out));

    // /* ############# PRESS 1 ###########*/
    if (res != ESP_OK)
    {
      printf("Could not measure: %d\n", res);
    }
    else
    {
      printf("Temperature: %.2f degrees Celsius; Pressure: %d Pa\n", temp, pressure);
    }

    hd44780_gotoxy(&lcd, 0, 1);
    snprintf(out, 9, "%.1f", ((float)pressure/100.0));
    hd44780_puts(&lcd, "P:");
    hd44780_puts(&lcd, out);
    hd44780_puts(&lcd, "hPa ");
    memset(&out[0], 0, sizeof(out));

    // /* ############# HUM 1 ###########*/
    res2 = si7021_measure_humidity(&dev2, &val);

    if (res != ESP_OK)
      printf("Could not measure humidity: %d (%s)\n", res2, esp_err_to_name(res2));
    else
      printf("Humidity: %.2f\n", val);

    snprintf(out, 9, "%.0f", val);
    hd44780_puts(&lcd, "H:");
    hd44780_puts(&lcd, out);
    hd44780_puts(&lcd, "%");
    memset(&out[0], 0, sizeof(out));
    if(255 == i)
    {
      i = 0;
    }
    i++;
    if (!(i % 2))
    {
      gpio_set_level(BLINK_GPIO, 0);
    }
    else
    {
      gpio_set_level(BLINK_GPIO, 1);
    }

    // if(0 == i%60)
      esp_deep_sleep_start();
    printf("i: %d \n", i);

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void app_main(void)
{
  esp_sleep_enable_timer_wakeup(5* 60 * 1000000);
   
  setup();

  // xTaskCreatePinnedToCore(loop, "loop", 1024, NULL, 5, NULL, 0);
  xTaskCreate(loop, "loop", 2 * 1024, NULL, 0, xHandle);

}