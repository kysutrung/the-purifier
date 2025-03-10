#include <Wire.h>
#include "DFRobot_ENS160.h"
#include <Adafruit_AHTX0.h>

// Cảm biến ENS160 (I2C)
DFRobot_ENS160_I2C ens160(&Wire, 0x53); 

// Cảm biến AHT21 (I2C)
Adafruit_AHTX0 aht21;

// Cảm biến GP2Y1014AU0F (Analog)
const int pinVo = A0;   // Đọc tín hiệu bụi
const int pinLed = 2;   // Điều khiển LED
const float kFactor = 0.5; // Hệ số hiệu chỉnh theo datasheet

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Khởi động ENS160
    if (ens160.begin() != 0) {
        Serial.println("Không tìm thấy ENS160, kiểm tra kết nối!");
        while (1);
    }
    Serial.println("Cảm biến ENS160 đã sẵn sàng!");
    ens160.setPWRMode(ENS160_STANDARD_MODE);

    // Khởi động AHT21
    if (!aht21.begin()) {
        Serial.println("Không tìm thấy AHT21, kiểm tra kết nối!");
        while (1);
    }
    Serial.println("Cảm biến AHT21 đã sẵn sàng!");

    // Cảm biến bụi
    pinMode(pinLed, OUTPUT);
}

// Hàm đọc mật độ bụi từ GP2Y1014AU0F
float readDustDensity() {
    digitalWrite(pinLed, LOW);  
    delayMicroseconds(280);      
    int sensorValue = analogRead(pinVo);
    delayMicroseconds(40);
    digitalWrite(pinLed, HIGH);  
    delayMicroseconds(9680);

    float voltage = sensorValue * (5.0 / 1023.0);
    float dustDensity = (voltage - 0.1) / kFactor;
    if (dustDensity < 0) dustDensity = 0; // Tránh giá trị âm

    return dustDensity;
}

void loop() {
    // Đọc cảm biến ENS160
    Serial.print("AQI: ");
    Serial.println(ens160.getAQI());

    Serial.print("TVOC (ppb): ");
    Serial.println(ens160.getTVOC());

    Serial.print("eCO2 (ppm): ");
    Serial.println(ens160.getECO2());

    // Đọc cảm biến AHT21
    sensors_event_t humidity, temp;
    aht21.getEvent(&humidity, &temp);

    Serial.print("Nhiệt độ (°C): ");
    Serial.println(temp.temperature, 1);

    Serial.print("Độ ẩm (%RH): ");
    Serial.println(humidity.relative_humidity, 1);

    // Đọc cảm biến bụi GP2Y1014AU0F
    float dust = readDustDensity();
    Serial.print("Mật độ bụi (mg/m³): ");
    Serial.println(dust);

    Serial.println("--------------------");
    delay(2000); // Đợi 2 giây trước khi đọc lại
}
