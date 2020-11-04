#include <ArduinoJson.h>
#include "FirebaseESP32.h"//ツール→ライブラリを管理→ Firebase　ESP32　Client
#include <M5Stack.h>
#include <WiFi.h>


//初期値設定
int num = 0;//表情のパターン

int now_received_face=0;//相手から受け取った表情
int received_face=0;//以前受け取った値と比較する用

int count = 0;//loopの周期をカウント

//wifi
#define WIFI_SSID "**********"//自分のルーターのSSIDに変更してください
#define WIFI_PASSWORD "***************:" //自分のルーターのパスワードに変更してください

// FirebaseのデータベースURL（自分のデータベースURLに変更してください）
#define FIREBASE_DATABASE_URL "m5data2-868b2.firebaseio.com"
#define FIREBASE_AUTH "***********************"//Slackで送ったものに変更

FirebaseData firebaseData;

//*****セットアップ******************
void setup() {
  M5.begin();
  M5.Power.begin();

  // Wi-Fi接続
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");//wifiに繋がらないと永遠と....が出てくる
    delay(500);
  }
  Serial.println();

  // Firebase初期化
   Firebase.begin(FIREBASE_DATABASE_URL, FIREBASE_AUTH);
   Firebase.reconnectWiFi(true);
   
  // WiFi Connected
  Serial.println("\nWiFi Connected.");
  Serial.println(WiFi.localIP());
  M5.Lcd.setTextSize(5);
  M5.Lcd.setCursor(10, 100);

  //顔の表情
  eye(num%6);
}



//****メインループ********************
void loop() {
  M5.update();
  //M5.Lcd.setCursor(10, 100);
  
  //***実行内容***
  //A,Cボタンでアバターを選択してBボタンで送る
  //相手がアバターを更新したら、それを検知して変更を反映させる
    
  //Aボタン→減少、Cボタン→増加　Bボタン→データ送信
  if (M5.BtnA.wasReleased()) {
    num--;
    M5.Lcd.fillScreen(BLACK);
    eye(num%6);
  }

  if (M5.BtnC.wasPressed()){
    num++;
    M5.Lcd.fillScreen(BLACK);
    eye(num%6);
  }
  
  if(M5.BtnB.wasReleased()) {
    M5.Lcd.setCursor(160, 200);
    M5.Lcd.setTextSize(5);
    M5.Lcd.print("send!");
    Firebase.setInt(firebaseData, "/M5Stack/risa/face", num);
    delay(1000);
    M5.Lcd.fillScreen(BLACK);
    eye(num%6);

  }

  //7回周期で受け取る
  if(count%7==0){
    if(Firebase.getInt(firebaseData, "/M5Stack/jun/face")){
      now_received_face = int(firebaseData.intData());
    }
    if(now_received_face!=received_face){
      num = now_received_face;   
      Serial.print("received");
      Serial.print(num);
      Serial.println("\n");
      received_face = now_received_face;
      
      M5.Lcd.fillScreen(BLACK);
      eye(num%6);
      M5.Lcd.setCursor(100, 200);
      M5.Lcd.setTextSize(5);
      M5.Lcd.print("received!");
    }
  }
  count++;
  Serial.print(count);
  Serial.println("\n");

}
