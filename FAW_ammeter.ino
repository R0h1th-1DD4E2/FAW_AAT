#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_INA219.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_INA219 ina219;

#define sensorPin A1
const float calibrationFactor = 0.1;
unsigned int state = 0;

void setup() {
    lcd.init();
    ina219.begin();

    lcd.backlight();
    Serial.begin(9600);
    lcd.print("Digital Ammeter");
    delay(2000);
    lcd.clear();
}

void loop() {
    if (state == 0){

    }
    else if(state == 1) {

    }
    else if(state == 2) {

    }

}


void button_click() {

}


void display_current(float current_value) {
    char currentStr[10];
    dtostrf(current_value, 6, 2, currentStr);
    Serial.print("Output Current: ");
    Serial.println(currentStr);

    char buffer[16];
    snprintf(buffer, sizeof(buffer), "Current: %sA", currentStr);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Digital Ammeter");

    lcd.setCursor(0, 1);
    lcd.print(buffer);
}

float hall_sensor(int sensor_value) {
    float current;
    float voltage = (sensor_value / 1023.0) * 5.0;
    Serial.print("Sensor Value: ");
    Serial.println(sensor_value);
    Serial.print("Voltage: ");
    Serial.println(voltage);
    if (abs(voltage - 2.5) >= 0.01) {
      current = (voltage - 2.5) / calibrationFactor;
    } else {
      current = 0;
    }
    return current;
}

float i2c_sensor() {
    float current_mA = 0;

    current_mA = ina219.getCurrent_mA();
    
    Serial.print("Current: "); Serial.print(current_mA); Serial.println(" mA");
    Serial.println("");

    return current_mA;
}