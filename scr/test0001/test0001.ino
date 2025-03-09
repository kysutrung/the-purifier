#include <Wire.h>
#include <Adafruit_AHTX0.h>

#define ENS160_I2C_ADDR 0x53  // Địa chỉ I²C của ENS160

Adafruit_AHTX0 aht;  // Khởi tạo cảm biến AHT21

// Hàm ghi dữ liệu vào thanh ghi ENS160
void writeENS160(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(ENS160_I2C_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

// Hàm đọc dữ liệu từ thanh ghi ENS160
uint8_t readENS160(uint8_t reg) {
    Wire.beginTransmission(ENS160_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(ENS160_I2C_ADDR, 1);
    return Wire.available() ? Wire.read() : 0;
}

// Hàm đọc 2 byte dữ liệu từ ENS160
uint16_t readENS160_16bit(uint8_t reg) {
    Wire.beginTransmission(ENS160_I2C_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(ENS160_I2C_ADDR, 2);
    uint8_t low = Wire.read();
    uint8_t high = Wire.read();
    return (high << 8) | low;
}

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Khởi tạo cảm biến AHT21
    if (!aht.begin()) {
        Serial.println("Không tìm thấy cảm biến AHT21!");
        while (1);
    }
    Serial.println("AHT21 đã sẵn sàng!");

    // Kiểm tra ENS160
    uint8_t chipID = readENS160(0x00);
    Serial.print("ENS160 Chip ID: 0x");
    Serial.println(chipID, HEX);

    if (chipID != 0x60) {
        Serial.println("Không tìm thấy ENS160!");
        while (1);
    }

    // Chuyển ENS160 sang chế độ đo bình thường
    writeENS160(0x10, 0x02);
}

void loop() {
    // Đọc nhiệt độ & độ ẩm từ AHT21
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    
    float temperature = temp.temperature;
    float humidityVal = humidity.relative_humidity;

    // Cập nhật nhiệt độ & độ ẩm cho ENS160 để hiệu chỉnh
    writeENS160(0x13, (uint8_t)temperature);
    writeENS160(0x14, (uint8_t)humidityVal);

    // Đọc dữ liệu từ ENS160
    uint16_t tvoc = readENS160_16bit(0x22);
    uint16_t eco2 = readENS160_16bit(0x24);
    uint8_t airQuality = readENS160(0x20);

    // Hiển thị dữ liệu lên Serial Monitor
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidityVal);
    Serial.print("% | ");

    Serial.print("TVOC: ");
    Serial.print(tvoc);
    Serial.print(" ppb, eCO2: ");
    Serial.print(eco2);
    Serial.print(" ppm, AQI: ");
    Serial.println(airQuality);

    delay(2000);
}
