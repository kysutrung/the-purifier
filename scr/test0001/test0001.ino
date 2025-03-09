#include <Wire.h>
#include "DFRobot_ENS160.h"
#include <Adafruit_AHTX0.h>

// Khởi tạo cảm biến ENS160 với giao tiếp I2C
DFRobot_ENS160_I2C ens160(&Wire, 0x53); // Địa chỉ I2C mặc định là 0x53

// Khởi tạo cảm biến AHT21
Adafruit_AHTX0 aht21;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Khởi tạo cảm biến ENS160
    if (ens160.begin() != 0) {
        Serial.println("Không tìm thấy ENS160, kiểm tra kết nối!");
        while (1);
    }
    Serial.println("Cảm biến ENS160 đã sẵn sàng!");

    // Cấu hình chế độ hoạt động
    ens160.setPWRMode(ENS160_STANDARD_MODE);

    // Khởi tạo cảm biến AHT21
    if (!aht21.begin()) {
        Serial.println("Không tìm thấy AHT21, kiểm tra kết nối!");
        while (1);
    }
    Serial.println("Cảm biến AHT21 đã sẵn sàng!");
}

void loop() {
    // Đọc dữ liệu từ ENS160
    Serial.print("AQI: ");
    Serial.println(ens160.getAQI()); // Chỉ số chất lượng không khí

    Serial.print("TVOC (ppb): ");
    Serial.println(ens160.getTVOC()); // Tổng hợp chất hữu cơ bay hơi

    Serial.print("eCO2 (ppm): ");
    Serial.println(ens160.getECO2()); // Nồng độ CO2 ước tính

    // Đọc dữ liệu từ AHT21
    sensors_event_t humidity, temp;
    aht21.getEvent(&humidity, &temp);

    Serial.print("Nhiệt độ (°C): ");
    Serial.println(temp.temperature, 1); // Hiển thị 1 chữ số sau dấu thập phân

    Serial.print("Độ ẩm (%RH): ");
    Serial.println(humidity.relative_humidity, 1); // Hiển thị 1 chữ số sau dấu thập phân

    Serial.println("--------------------");
    delay(2000); // Đợi 2 giây trước khi đọc lại dữ liệu
}
