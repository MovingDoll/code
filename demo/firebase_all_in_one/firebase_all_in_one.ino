#include <ArduinoJson.h>
#include <IOXhop_FirebaseStream.h>
#include <IOXhop_FirebaseESP32.h>
#include <M5Stack.h>
#include <WiFi.h>
#include "porthub.h"
#define SERVO_ADDR 0x53

//PbHUB UNIT
PortHub porthub;
uint8_t HUB_ADDR[6]={HUB1_ADDR,HUB2_ADDR,HUB3_ADDR,HUB4_ADDR,HUB5_ADDR,HUB6_ADDR};

//初期値設定
int num = 0;
int angle = 0;
int before_angle = 0;

//wifi
#define WIFI_SSID "***********"//自分のルーターのSSIDに変更してください
#define WIFI_PASSWORD "***********" //自分のルーターのパスワードに変更してください

// FirebaseのデータベースURL（自分のデータベースURLに変更してください）
#define FIREBASE_DATABASE_URL "m5data-f5280.firebaseio.com"

//*****セットアップ******************
void setup() {
  M5.begin(true, false, true);
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
   Firebase.begin(FIREBASE_DATABASE_URL);
   
  // WiFi Connected
  Serial.println("\nWiFi Connected.");
  Serial.println(WiFi.localIP());
  M5.Lcd.setTextSize(5);
  M5.Lcd.setCursor(10, 100);

  //サーボモーター
  Wire.begin(21, 22, 100000);

  //PbHUB UNIT
  porthub.begin();


}


//****サーボモーター回転********************
void Servo_write_us(uint8_t number, uint16_t us) {
    Wire.beginTransmission(SERVO_ADDR);
    Wire.write(0x00 | number);
    Wire.write(us & 0x00ff);
    Wire.write(us >> 8 & 0x00ff);
    Wire.endTransmission();
}


//****メインループ********************
void loop() {
  M5.update();
  M5.Lcd.setCursor(10, 100);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.print(num);

  //***実行内容***
  //A,Cボタンで選択したメッセージ（アバター）をBボタンで送る
  //回転角センサーの値を常時送る
  //相手がメッセージ或いは回転角センサーの値を更新したら、それを検知して変更を反映させる
    
  //Aボタン→減少、Cボタン→増加　Bボタン→データ送信
  if (M5.BtnA.wasReleased()) {
    num--;
  }

  if (M5.BtnC.wasPressed()){
    num++;
  }
  
  if(M5.BtnB.wasReleased()) {
    Firebase.setInt("/M5Stack/message/num", num);
    Firebase.setInt("/M5Stack/message/new", 2222);
    M5.Lcd.setCursor(10, 100);
    M5.Lcd.print("send!");
    delay(1000);

  }

  //angleが前の値よりも10以上変化したら数値を送る
  angle = porthub.hub_a_read_value(HUB_ADDR[0]);
  if(abs(before_angle-angle)>10){
    Firebase.setInt("/M5Stack/angle/num", angle);
    Firebase.setInt("/M5Stack/angle/new", 2222);
    Serial.print(angle);
    before_angle = angle;
  }
//
//  //new=1111になっているのは相手が更新した証拠
//  //値を受け取り、サーボモーター(0番)を回転させ、new=0に戻す
//  if(int(Firebase.getFloat("/M5Stack/angle/new"))==2222){
//    int received_angle = Firebase.getFloat("/M5Stack/angle/num");
//    //0-760 →　-90～90にする
//    received_angle -= 380;
//    int moving_angle = received_angle * 9/38;
//    Firebase.setInt("/M5Stack/angle/check", moving_angle);
//    Servo_write_us(0,moving_angle);
//    Firebase.setInt("/M5Stack/angle/new", 0);
//  }
//  
//  //上同様にメッセージを受け取る
//  if(int(Firebase.getFloat("/M5Stack/message/new"))==1111){
//    num =  int(Firebase.getFloat("/M5Stack/message/num"));
//    Firebase.setInt("/M5Stack/num/new", 0);
//  }

}
