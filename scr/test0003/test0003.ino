//FOR A
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

float data[6] = {3, 3, 3, 3, 3, 3};

void setup() {
    Serial.begin(115200);
    Serial1.begin(9600);   // UART1 để gửi sang ESP32
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
    //đo
    data[0] = ens160.getAQI();
    data[1] = ens160.getTVOC();
    data[2] = ens160.getECO2();

    sensors_event_t humidity, temp;
    aht21.getEvent(&humidity, &temp);

    data[3] = temp.temperature;
    data[4] = humidity.relative_humidity;

    data[5] = readDustDensity();

    //gửi
    Serial1.write('<');  // Gửi ký tự bắt đầu
    Serial1.write((uint8_t*)data, sizeof(data));  // Gửi mảng float dạng nhị phân
    Serial1.write('>');  // Gửi ký tự kết thúc
    delay(20000);
}
