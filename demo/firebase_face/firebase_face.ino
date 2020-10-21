#include <ArduinoJson.h>
#include <IOXhop_FirebaseStream.h>
#include <IOXhop_FirebaseESP32.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <Avatar.h>
#include <faces/DogFace.h>


using namespace m5avatar;
Avatar avatar;

Face* faces[2];
const int facesSize = sizeof(faces) / sizeof(Face*);
int faceIdx = 0;

const Expression expressions[] = {
  Expression::Angry,
  Expression::Sleepy,
  Expression::Happy,
  Expression::Sad,
  Expression::Doubt,
  Expression::Neutral
};
const int expressionsSize = sizeof(expressions) / sizeof(Expression);
int idx = 0;

//wifi
#define WIFI_SSID "****************"//自分のルーターのSSIDに変更してください
#define WIFI_PASSWORD "***************" //自分のルーターのパスワードに変更してください

// FirebaseのデータベースURL（自分のデータベースURLに変更してください）
#define FIREBASE_DATABASE_URL "************************"


//*****セットアップ******************
void setup() {
  M5.begin();

  // Wi-Fi接続
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");//wifiに繋がらないと永遠と....が出てくる
    delay(500);
  }
  Serial.println();

  // Firebase初期化
  Firebase.begin(FIREBASE_DATABASE_URL);

  // WiFi Connected
  Serial.println("\nWiFi Connected.");
  Serial.println(WiFi.localIP());
//  M5.Lcd.setTextSize(3);
//  M5.Lcd.setCursor(10, 100);
//  M5.Lcd.println("Button Click!");
  faces[0] = avatar.getFace();
  faces[1] = new DogFace();
  //avatar
  avatar.init();
  //speaker
  M5.Speaker.begin();
}

//****メインループ********************
void loop() {
  M5.update();

  if (M5.BtnA.wasReleased()) {
    faceIdx = (faceIdx + 1) % facesSize;
    avatar.setFace(faces[faceIdx]);
    
  }

    if (M5.BtnC.wasPressed())
  {
    idx = (idx + 1) % expressionsSize;
    avatar.setExpression(expressions[idx]);
    
  }

  if(M5.BtnB.wasReleased()) {
  
    avatar.setSpeechText("send!");
    Firebase.setInt("/M5Stack/face", faceIdx% facesSize);
    Firebase.setInt("/M5Stack/expression", idx% expressionsSize);
    delay(1000);
    avatar.setSpeechText("");


  }
}
