#include <SparkFunHTU21D.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_LiquidCrystal.h>

#define LED 2

const int RS = 13, EN = 12, D0 = 14, D1 = 27, D2 = 26, D3 = 25;

Adafruit_LiquidCrystal lcd(RS, EN, D0, D1, D2, D3);
Adafruit_BMP085 bmp; 
HTU21D GTU21D;
// BMP180 myBMP(BMP180_STANDARD);


void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Serial.println("Init");
  Serial.printf("%x\n", 16);
  ledcSetup(0, 5000, 8);
  ledcAttachPin(LED, 0);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, 1);
  lcd.begin(16, 2);
  lcd.print("hello, world!");

  Serial.println("Initialize");
  GTU21D.begin();
  bmp.begin(BMP085_STANDARD);

  // while (!bmp.begin()) //sda, scl
  // {
  //   Serial.println(("Bosch BMP180/BMP085 is not connected or fail to read calibration coefficients"));
  //   delay(5000);
  // }
  
  // Serial.println(("Bosch BMP180/BMP085 sensor is OK")); //(F()) saves string to flash & keeps dynamic memory 

}

void loop() {

  String text = "0";
  lcd.clear();
  lcd.setCursor(0, 0);
  // lcd.print(millis()/1000);

  /* ############# TEMP 1 ###########*/

  lcd.print("T1:");
  lcd.print(bmp.readTemperature(),1);
  lcd.print(" ");

  Serial.print("Temp = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C, ");

  /* ############# TEMP 2 ###########*/

  lcd.print("T2:");
  lcd.print(GTU21D.readTemperature(),1);
  lcd.print(" ");

  Serial.print("Temp2 = ");
  Serial.print(GTU21D.readTemperature());
  Serial.println(" *C, ");

  lcd.setCursor(0,1);

  /* ############# PRESS 1 ###########*/

  lcd.print("P:");
  lcd.print((bmp.readPressure()/100.0),1);
  lcd.print(" ");
  
  Serial.print("Press = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");

  /* ############# HUM 1 ###########*/

  lcd.print("H:");
  lcd.print(GTU21D.readHumidity(),1);
  lcd.print("%");
  
  Serial.print("Hum = ");
  Serial.print(GTU21D.readHumidity());
  Serial.println(" %");


  delay(5000);
    // lcd.setCursor(0, 0);
  // lcd.print(millis()/1000);
    // Calculate altitude assuming 'standard' barometric
    // pressure of 1013.25 millibar = 101325 Pascal
    // Serial.print("Altitude = ");
    // Serial.print(bmp.readAltitude());
    // Serial.println(" meters");

    // Serial.print("Pressure at sealevel (calculated) = ");
    // Serial.print(bmp.readSealevelPressure());
    // Serial.println(" Pa");

  // // you can get a more precise measurement of altitude
  // // if you know the current sea level pressure which will
  // // vary with weather and such. If it is 1015 millibars
  // // that is equal to 101500 Pascals.
  //   Serial.print("Real altitude = ");
  //   Serial.print(bmp.readAltitude(101500));
  //   Serial.println(" meters");
  // Serial.print(F("Temperature.......: ")); Serial.print(myBMP.getTemperature(), 1); Serial.println(F(" +-1.0C"));
  // Serial.print(F("Pressure..........: ")); Serial.print(myBMP.getPressure());       Serial.println(F(" +-100Pa"));

  // put your main code here, to run repeatedly:
  // delay(1000);
  // digitalWrite(LED, !digitalRead(LED));
  // Serial.println(text);
 
  // increase the LED brightness
  // for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
  //   // changing the LED brightness with PWM
  //   ledcWrite(0, dutyCycle);
  //   delay(15);
  // }

  // // decrease the LED brightness
  // for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
  //   // changing the LED brightness with PWM
  //   ledcWrite(0, dutyCycle);   
  //   delay(5);
  // }

  // if(Serial.available())
  // {
  //   text = Serial.readStringUntil('\n');
  //   Serial.println(text);
  // }
}
