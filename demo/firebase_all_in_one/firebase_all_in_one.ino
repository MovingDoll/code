#include <ArduinoJson.h>
//#include <IOXhop_FirebaseStream.h>
//#include <IOXhop_FirebaseESP32.h>
#include <M5Stack.h>
#include <WiFi.h>
#include "porthub.h"
#include "FirebaseESP32.h"

#define SERVO_ADDR 0x53

//****初期値設定****
int num = 0;//表情のパターン
int angle = 0;//角度センサー
int before_angle = 0;//角度センサーの前の数値と比較する用

int count = 0;//loopの周期をカウント

int now_received_angle=0;//相手から受け取った角度
int received_angle=0;//以前受け取った値と比較する用

int now_received_face=0;//相手から受け取った表情
int received_face=0;//以前受け取った値と比較する用

int changed = 0;
int fluency = 10;

//PbHUB UNIT
PortHub porthub;
uint8_t HUB_ADDR[6]={HUB1_ADDR,HUB2_ADDR,HUB3_ADDR,HUB4_ADDR,HUB5_ADDR,HUB6_ADDR};

//wifi
#define WIFI_SSID "techgarage"//自分のルーターのSSIDに変更してください
#define WIFI_PASSWORD "killtheproject" //自分のルーターのパスワードに変更してください

// FirebaseのデータベースURL（自分のデータベースURLに変更してください）
#define FIREBASE_DATABASE_URL "m5data2-868b2.firebaseio.com"
#define FIREBASE_AUTH "qeG2HHAbMYz23abY1dKTwSbyhikt48Y0PySJhpJo"
FirebaseData firebaseData;
//*****セットアップ******************
void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  
  // Wi-Fi接続
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");//wifiに繋がらないと永遠と....が出てくる
    delay(500);
  }

  // Firebase初期化
   Firebase.begin(FIREBASE_DATABASE_URL, FIREBASE_AUTH);
   Firebase.reconnectWiFi(true);
   
  // WiFi Connected
  Serial.println("\nWiFi Connected.");
  Serial.println(WiFi.localIP());

  //サーボモーター
  Wire.begin(21, 22, 100000);

  //PbHUB UNIT
  porthub.begin();

  eye(num%6);

}


//****サーボモーター回転********************
//void Servo_write_angle(uint8_t number, uint8_t angle) {
//    Wire.beginTransmission(SERVO_ADDR);
//    Wire.write(0x10 | number);
//    Wire.write(angle);
//    Wire.endTransmission();
//}
void Servo_write_us(uint8_t number, uint16_t us) {
    Wire.beginTransmission(SERVO_ADDR);
    Wire.write(0x00 | number);
    Wire.write(us & 0x00ff);
    Wire.write(us >> 8 & 0x00ff);
    Wire.endTransmission();
}
//****メインループ********************
void send(int angle){
//  FirebaseJsonArray arr;
//  
 // int str[] = {1,2,3,4,5};
//  
//  arr.set("/[0]/[1]", (const int) str);
  Firebase.setInt(firebaseData, "/M5Stack/dog/angle", angle);
  return;
}

void loop() {
  M5.update();
  
  //***実行内容***
  //A,Cボタンで選択したアバターをBボタンで送る
  //回転角センサーの値が変化した場合、送る
  //相手がメッセージ或いは回転角センサーの値を更新したら、それを検知して変更を反映させる
    
  //Aボタン→減少、Cボタン→増加　Bボタン→データ送信
  if (M5.BtnA.wasPressed()) {
    num--;
    M5.Lcd.fillScreen(BLACK);
    eye(num%6);
  }

  if (M5.BtnC.wasPressed()){
    num++;
    M5.Lcd.fillScreen(BLACK);
    eye(num%6);
  }
  
  if(M5.BtnB.wasPressed()) {
    M5.Lcd.setCursor(100, 200);
    M5.Lcd.setTextSize(5);
    M5.Lcd.print("send!");
    Firebase.setInt(firebaseData, "/M5Stack/dog/face", num);
    M5.Lcd.fillScreen(BLACK);
    eye(num%6);
  }

  //角度センサーの読み取り
  angle = porthub.hub_a_read_value(HUB_ADDR[0]);
  Serial.print(angle);
  Serial.println("\n");

  //前と比べて50以上変化あれば送信(負荷を減らすため)
  if(abs(before_angle-angle)>20){
    send(angle);
  }
  before_angle = angle;

//  if(abs(changed-count)<5){
//    fluency = 2;
//  }else{
//    fluency = 10;
//  }
  //10回に１回or毎回データベースにアクセスし、変化があるか確認する
  //if(count%1==0){
    if(Firebase.getInt(firebaseData, "/M5Stack/cat/angle")){
      now_received_angle = int(firebaseData.intData());
    }
    
    if(now_received_angle!=received_angle){
      Servo_write_us(0,now_received_angle*3);
      
      Serial.print("received");
      Serial.print(now_received_angle);
      Serial.println("\n");
      received_angle = now_received_angle;
//      changed = count;
    }
  //}
  
  //10回に１回データベースにアクセスし、変化があるか確認する
  //if(count%17==0){
    if(Firebase.getInt(firebaseData, "/M5Stack/cat/face")){
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
  //}
  //count++;

  //delay(100);

}
