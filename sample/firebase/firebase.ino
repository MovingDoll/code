#include <ArduinoJson.h>
#include <IOXhop_FirebaseStream.h>
#include <IOXhop_FirebaseESP32.h>
#include <M5Stack.h>
#include <WiFi.h>

#define WIFI_SSID "******************"
#define WIFI_PASSWORD "**************"

// FirebaseのデータベースURL（ご自身のデータベースURLに変更してください）
#define FIREBASE_DATABASE_URL "*********************"

// カウント初期化
int count = 0;

void setup() {
  M5.begin();

  // Wi-Fi接続
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();

  // Firebase初期化
  Firebase.begin(FIREBASE_DATABASE_URL);

  // WiFi Connected
  Serial.println("\nWiFi Connected.");
  Serial.println(WiFi.localIP());
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(10, 100);
  M5.Lcd.println("Button Click!");
}

void loop() {
  M5.update();

  if (M5.BtnA.wasReleased()) {
    // カウントアップ
    count++;

    // ディスプレイ表示
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.fillScreen(RED);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextSize(3);
    M5.Lcd.printf("Count Up: %d", count);
  }

  if(M5.BtnC.wasReleased()) {
    // カウントダウン
    count--;

    // ゼロ以下にはしない
    if (count <= 0) count = 0;

    // ディスプレイ表示
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.fillScreen(GREEN);
    M5.Lcd.setTextColor(BLACK);
    M5.Lcd.setTextSize(3);
    M5.Lcd.printf("Count Down: %d", count);
  }

  if(M5.BtnB.wasReleased()) {
    // ディスプレイ表示
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.fillScreen(BLUE);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(3);
    M5.Lcd.printf("Count Send: %d", count);

    // カウント送信
    Firebase.setInt("/M5Stack/counter", count);

  }
}
