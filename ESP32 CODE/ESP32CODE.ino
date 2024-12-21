#define BLYNK_TEMPLATE_ID "TMPL64zh24SwV"
#define BLYNK_TEMPLATE_NAME "NHIET DO DO AM"
#define BLYNK_AUTH_TOKEN "ndT2rt3LSs0P_TRT61vCaw8I41QlfQ0K"

#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Adafruit_NeoPixel.h> 

// Thông tin WiFi
char ssid[] = "ACLAB";
char pass[] = "ACLAB2023";

// Chân LED vật lý
#define LED1_PIN 26
#define LED2_PIN 25

// BlynkTimer
BlynkTimer timer;

// Biến trạng thái LED
bool led1State = false;
bool led2State = true; // LED2 bắt đầu với trạng thái ngược LED1

// Biến nhiệt độ và độ ẩm
float temperature = 0.0;
float humidity = 0.0;
float fahrenheit = 0.0;

// Hàm tách và xử lý dữ liệu
String parseValue(String data, String key) {
  int startIndex = data.indexOf(key);
  if (startIndex == -1) return "N/A"; // Không tìm thấy khóa

  int endIndex = data.indexOf(';', startIndex);
  if (endIndex == -1) endIndex = data.length(); // Nếu không có dấu `;`, lấy đến cuối chuỗi

  return data.substring(startIndex + key.length(), endIndex);
}

// Hàm xử lý dữ liệu từ UART
void readSensorData() {
  if (Serial2.available() > 0) {
    String data = Serial2.readStringUntil('\n'); // Đọc đến khi nhận ký tự xuống dòng
    data.trim();                                  // Loại bỏ khoảng trắng ở đầu và cuối chuỗi
    Serial.println("Received data: " + data);

    // Tách và chuyển đổi giá trị nhiệt độ và độ ẩm
    String tempStr = parseValue(data, "TEMP:");
    String humiStr = parseValue(data, "HUMI:");

    if (tempStr != "N/A" && humiStr != "N/A") {
      float tempValue = tempStr.toFloat();
      float humiValue = humiStr.toFloat();

      // Kiểm tra giá trị hợp lệ và gán
      if (tempValue != 0 || tempStr == "0") {
        temperature = tempValue; // Gán giá trị nhiệt độ
      } else {
        Serial.println("Invalid Temperature Value.");
      }

      if (humiValue != 0 || humiStr == "0") {
        humidity = humiValue; // Gán giá trị độ ẩm
      } else {
        Serial.println("Invalid Humidity Value.");
      }

      // Debug giá trị
      Serial.print("Parsed Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C | Parsed Humidity: ");
      Serial.print(humidity);
      Serial.println(" %");

      // Gửi dữ liệu lên Blynk
      fahrenheit = temperature*1.8+32;
      Blynk.virtualWrite(V0, temperature);
      Blynk.virtualWrite(V1, humidity);
      Blynk.virtualWrite(V4, fahrenheit);

      String status;
      if (temperature >= 0 && temperature <= 10) {
        status = "Lạnh cứng người";
      } else if (temperature > 10 && temperature <= 20) {
        status = "Lạnh quá đi :v";
      } else if (temperature > 20 && temperature <= 30) {
        status = "Thoải mái ^^";
      } else if (temperature > 30 && temperature <= 40) {
        status = "Nóng quá trời :D";
      } else {
        status = "Cảnh báo nguy hiểm!";
      }

      // Gửi chuỗi trạng thái lên Blynk
      Blynk.virtualWrite(V5, status); 
    } else {
      Serial.println("Invalid data format");
    }
  }
}

// Blynk WRITE cho LED (không thay đổi trạng thái nhấp nháy)
BLYNK_WRITE(V2) {
  int ledState = param.asInt();
  digitalWrite(LED1_PIN, ledState);
}

BLYNK_WRITE(V3) {
  int ledState = param.asInt();
  digitalWrite(LED2_PIN, ledState);
}

// Hàm điều khiển bật/tắt LED
void toggleLeds() {
  // Đảo trạng thái LED
  led1State = !led1State;
  led2State = !led2State;

  // Điều khiển LED vật lý
  digitalWrite(LED1_PIN, led1State);
  digitalWrite(LED2_PIN, led2State);

  // Gửi trạng thái lên ứng dụng Blynk
  Blynk.virtualWrite(V2, led1State);
  Blynk.virtualWrite(V3, led2State);

  // Debug trạng thái LED
  Serial.print("LED1: ");
  Serial.print(led1State ? "ON" : "OFF");
  Serial.print(" | LED2: ");
  Serial.println(led2State ? "ON" : "OFF");
}

#define PIN 18        // Chân điều khiển SIG (GPIO18, bạn có thể thay bằng chân khác)
#define NUM_LEDS 4    // Số lượng LED trên module (thường là 4)

Adafruit_NeoPixel strip(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setLEDColorBasedOnTemperature(int temp) {
  uint32_t color;
  if (temp < 20) {
    color = strip.Color(0,255, 0);   // Màu xanh dương
  } else if (temp >= 20 && temp < 30) {
    color = strip.Color(0, 200, 255);   // Màu xanh lá
  } else if (temp >= 30 && temp <= 35) {
    color = strip.Color(255, 255, 0); // Màu vàng
  } else if (temp > 35) {
    color = strip.Color(255, 0, 0);   // Màu đỏ
  }

  // Đặt màu cho tất cả các LED
  for (int i = 0; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
}

void setup() {
  // Khởi động Serial để debug
  Serial.begin(115200);

  // Cấu hình UART để đọc từ STM32
  Serial2.begin(115200, SERIAL_8N1, 16, 17); // RX = GPIO16, TX = GPIO17 

  // Kết nối Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Serial.println("Connected to Blynk!");
  
  // Cấu hình chân LED làm OUTPUT
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Tắt cả hai LED ban đầu
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);

  // Cài đặt timer để nhấp nháy LED mỗi 1 giây
  timer.setInterval(1000L, toggleLeds);

  // Cài đặt timer để đọc dữ liệu từ UART
  timer.setInterval(500L, readSensorData); // Đọc dữ liệu từ STM32 mỗi 500ms

  strip.begin();      // Bắt đầu điều khiển LED
  strip.show();       // Xóa tất cả LED (tắt đèn ban đầu)

}

void loop(){
  Blynk.run();   
  timer.run();  

setLEDColorBasedOnTemperature(temperature); // Gọi hàm để thay đổi màu LED theo nhiệt độ
  strip.show();        // Cập nhật LED
   
}
