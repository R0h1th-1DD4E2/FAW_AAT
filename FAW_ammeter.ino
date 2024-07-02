#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_INA219.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_INA219 ina219;

#define sensorPin A1
#define resV1 A2
#define resV2 A3
#define button 12

const float calibrationFactor = 0.1;
unsigned int state = 1;
float current = 0;

void setup() {
    pinMode(sensorPin,INPUT);
    pinMode(button,INPUT);
    pinMode(resV1,INPUT);
    pinMode(resV2,INPUT);
    
    lcd.init();
    ina219.begin();

    lcd.backlight();
    Serial.begin(115200);
    lcd.print("Digital Ammeter");
    delay(2000);
    lcd.clear();
}

void loop() {

    button_click();
    Serial.println(state);

    if(state == 0) {
        // Hall Effect Sensor (ACS712)
        current = hall_sensor();
        display_current(current);
    }
    else if(state == 1) {
        // INA219
        current = i2c_sensor();
        display_current(current);
    }
    delay(1000);
}


void button_click() {
    int button_in = digitalRead(button);
    Serial.print("Button : "); Serial.println(button_in);
    if (button_in) {
        if (state >= 1 ){
            state = 0;
        }
        else {
            state++;
        }
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Mode");
        lcd.setCursor(5, 0);
        lcd.print(state);
        lcd.setCursor(0, 1);
        lcd.print("Sensor:");
        lcd.setCursor(9, 1);
        lcd.print(state ? "I2C" : "Hall");
        delay(1000);
        lcd.clear();
    }
}


void display_current(float current_value) {
    char currentStr[8];
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

float hall_sensor() {
    float current = 0;
    int sensor_value = analogRead(sensorPin);
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

    return current_mA/1000;
}