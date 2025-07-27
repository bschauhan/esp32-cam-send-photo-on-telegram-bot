#include <WiFi.h>
#include "esp_camera.h"
#include <WiFiClientSecure.h>
#include <HTTPClient.h>


#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* botToken = "bot_token_here"; // Replace with your Telegram bot token
const char* chatID = "bot_chat_id_here"; // Replace with your Telegram chat ID

// Telegram API Endpoint
String telegramApiUrl = "https://api.telegram.org/bot" + String(botToken) + "/sendPhoto";

void sendPhotoToTelegram(camera_fb_t* fb) {
  WiFiClientSecure client;
  client.setInsecure();  // WARNING: for testing only

  const char* host = "api.telegram.org";
  const int httpsPort = 443;

  if (!client.connect(host, httpsPort)) {
    Serial.println("Connection to Telegram failed");
    return;
  }

  String boundary = "----WebKitFormBoundary7MA4YWxkTrZu0gW";
  String startRequest = "--" + boundary + "\r\n";
  startRequest += "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n";
  startRequest += chatID;
  startRequest += "\r\n--" + boundary + "\r\n";
  startRequest += "Content-Disposition: form-data; name=\"photo\"; filename=\"esp32-cam.jpg\"\r\n";
  startRequest += "Content-Type: image/jpeg\r\n\r\n";

  String endRequest = "\r\n--" + boundary + "--\r\n";

  int contentLength = startRequest.length() + fb->len + endRequest.length();

  // Send HTTP POST headers
  client.println("POST /bot" + String(botToken) + "/sendPhoto HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("Content-Type: multipart/form-data; boundary=" + boundary);
  client.println("Content-Length: " + String(contentLength));
  client.println();

  // Send multipart body
  client.print(startRequest);
  client.write(fb->buf, fb->len);
  client.print(endRequest);

  // Read the response
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") break; // headers done
  }

  String response = client.readString();
  Serial.println("Telegram response:");
  Serial.println(response);
}



void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Booting...");

    // Enable verbose logging for all ESP modules
  esp_log_level_set("*", ESP_LOG_VERBOSE);


  WiFi.mode(WIFI_STA);
  Serial.println("Starting WiFi...");
  WiFi.begin(ssid, password);
  Serial.println("WiFi.begin() called");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected!");
  Serial.println(WiFi.localIP());

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = Y2_GPIO_NUM;
  config.pin_d1       = Y3_GPIO_NUM;
  config.pin_d2       = Y4_GPIO_NUM;
  config.pin_d3       = Y5_GPIO_NUM;
  config.pin_d4       = Y6_GPIO_NUM;
  config.pin_d5       = Y7_GPIO_NUM;
  config.pin_d6       = Y8_GPIO_NUM;
  config.pin_d7       = Y9_GPIO_NUM;
  config.pin_xclk     = XCLK_GPIO_NUM;
  config.pin_pclk     = PCLK_GPIO_NUM;
  config.pin_vsync    = VSYNC_GPIO_NUM;
  config.pin_href     = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn     = PWDN_GPIO_NUM;
  config.pin_reset    = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // Init with high specs to avoid low frame issues
  config.frame_size = FRAMESIZE_XGA; // try FRAMESIZE_UXGA if QVGA works
  config.jpeg_quality = 12;
  config.fb_count = 2;

  Serial.println("Initializing camera...");
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
  }
  Serial.println("Camera init success");

  // Capture Photo
  Serial.println("Capturing photo...");
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  Serial.println("Photo successfully captured!");

  Serial.printf("Photo size: %zu bytes\n", fb->len);
  Serial.printf("Photo width: %zu, height: %zu\n", fb->width, fb->height);
  Serial.printf("Photo format: %d\n", fb->format);

  Serial.println("Sending photo to Telegram...");
  sendPhotoToTelegram(fb);


  esp_camera_fb_return(fb);
}

void loop() {
  delay(1000);
}