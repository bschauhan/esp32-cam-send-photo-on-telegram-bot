# ESP32-CAM Telegram Photo Sender

This project allows your ESP32-CAM to capture a photo and send it directly to your Telegram account using a Telegram bot. It connects to WiFi, initializes the camera, captures a photo, and posts it as a message via the Telegram Bot API.

---

## 📦 Hardware Requirements

- ESP32-CAM board (AI-Thinker or similar)
- FTDI programmer or USB-to-TTL converter
- Jumper wires
- Optional: PIR sensor for motion detection
- Micro USB cable

---

## ⚙️ Software Requirements

- Arduino IDE
- ESP32 board support installed via Board Manager
- Required libraries:
  - `WiFi.h`
  - `esp_camera.h`
  - `WiFiClientSecure.h`
  - `HTTPClient.h`

---

## 🤖 How to Create and Configure a Telegram Bot

1. **Open Telegram app** and search for `@BotFather`.
2. **Start a chat** with BotFather and type: /newbot
3. **Provide a name** for your bot (e.g., `MyESP32CamBot`).
4. **Provide a unique username** ending in `bot` (e.g., `esp32camphoto_bot`).
5. BotFather will reply with your **Bot Token**, like: 123456789:ABCDefGhIJKlmNoPQrstUVwxYZ
6. **Start a chat with your bot** by searching its username in Telegram and clicking "Start".
7. **Get your chat ID:**
- Open your browser and go to:
  ```
  https://api.telegram.org/bot<YourBotToken>/getUpdates
  ```
- After you send a message to the bot, the API response will show your chat ID. Look for:
  ```json
  "chat":{"id":890506551,"first_name":"YourName", ... }
  ```

---

## 🧠 Code Configuration

Open the `esp32-cam-telegram.ino` and update these values:

```cpp
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";
const char* botToken = "your_bot_token_here";
const char* chatID = "your_chat_id_here";
