#include <ArduinoJson.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <Wire.h>
#include "porthub.h"
#include <FirebaseESP32.h>

#define SERVO_ADDR 0x53

/***************Definition*********************/
int num = 0;              //表情のパターン
int angle = 0;            //角度センサー
int before_angle = 0;     //角度センサーの前の数値と比較する用
int now_received_angle=0; //相手から受け取った角度
int received_angle=0;     //以前受け取った値と比較する用
int now_received_face=0;  //相手から受け取った表情
int received_face=0;      //以前受け取った値と比較する用
String mypost;            //受信するデータポスト
String topost;            //送信するデータポスト

// PbHUB UNIT
PortHub porthub;
uint8_t HUB_ADDR[6]={HUB1_ADDR,HUB2_ADDR,HUB3_ADDR,HUB4_ADDR,HUB5_ADDR,HUB6_ADDR};

// wifi
#define WIFI_SSID     "************"    //自分のルーターのSSIDに変更してください
#define WIFI_PASSWORD "************"     //自分のルーターのパスワードに変更してください

// Firebase
#define FIREBASE_DATABASE_URL "m5stack-data.firebaseio.com"
#define FIREBASE_AUTH         "QCT6zw9CXbqM6TITH1AROK3HITZhNdhVtZTuu3HB"
FirebaseData firebaseData;

IPAddress ip1, ip2;   // MyIP, ToIP
/***************Definition*********************/


/***************Setup***************&***/
void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  
  // wifi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("."); //wifiに繋がらないと永遠と....が出てくる
    delay(500);
  }

  // Firebase Initialization
  Firebase.begin(FIREBASE_DATABASE_URL, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
   
  // WiFi Connected
  Serial.println("\nWiFi Connected.");
  ip1 = WiFi.localIP();
  Serial.println(ip1);
  mypost = iptostr(ip1); // ="/xxx.xxx.xx.x"
  angle_input(); // get IP input -> ip2
  topost = iptostr(ip2);

  // Initialize mypost Data
  Firebase.setString(firebaseData, mypost+"/name", "");
  Firebase.setInt(firebaseData, mypost+"/angle", 0);
  Firebase.setInt(firebaseData, mypost+"/face", 0);
  Firebase.setString(firebaseData, topost+"/name", mypost);
  Firebase.setInt(firebaseData, topost+"/angle", 0);
  Firebase.setInt(firebaseData, topost+"/face", 0);

  Wire.begin(21, 22, 100000); // SERVO
  porthub.begin(); // PbHUB UNIT

  eye(num%6);
}
/***************Setup***************&***/

/***************SERVO********************/
void Servo_write_us(uint8_t number, uint16_t us) {
    Wire.beginTransmission(SERVO_ADDR);
    Wire.write(0x00 | number);
    Wire.write(us & 0x00ff);
    Wire.write(us >> 8 & 0x00ff);
    Wire.endTransmission();
}
/***************SERVO********************/


/***************loop********************/
void loop() {
  M5.update();
  
  //***実行内容***
  //Aボタンで送信先のIPアドレスを設定
  //Cボタンで表情選択
  //Bボタンで送信
  //回転角センサーの値が変化した場合、送る
  //相手がメッセージ或いは回転角センサーの値を更新したら、それを検知して変更を反映させる
  //Aボタン→減少、Cボタン→増加　Bボタン→データ送信

  // set IP
  if (M5.BtnA.wasPressed()) {
    angle_input(); // get IP input -> ip2
    topost = iptostr(ip2);
  }

  // select face
  if (M5.BtnC.wasPressed()){
    num = (num + 1) % 6;
    eye(num%6);
  }

  // send face
  if(M5.BtnB.wasPressed()) {
    M5.Lcd.setCursor(100, 200);
    M5.Lcd.setTextSize(5);
    M5.Lcd.print("send!");
    Firebase.setString(firebaseData, topost+"/name", mypost);
    Firebase.setInt(firebaseData, topost+"/face", num);
    eye(num%6);
  }

  // read angle
  angle = porthub.hub_a_read_value(HUB_ADDR[0]);
  Serial.print(angle);
  Serial.println("\n");

  //前と比べて20以上変化あれば送信(負荷を減らすため)
  if(abs(before_angle-angle)>20){
    Firebase.setInt(firebaseData, topost+"/angle", angle);
  }
  before_angle = angle;

  if(Firebase.getInt(firebaseData, mypost+"/angle")){
    now_received_angle = int(firebaseData.intData());
  }

  //前と比べて20以上変化あれば回転(負荷を減らすため)
  if(abs(now_received_angle-received_angle) > 20){
    Servo_write_us(0,now_received_angle*3);
    Serial.print("received");
    Serial.print(now_received_angle);
    Serial.println("\n");
    received_angle = now_received_angle;
  }
    
  if(Firebase.getInt(firebaseData, mypost+"/face")){
    now_received_face = int(firebaseData.intData());
  }
  if(now_received_face!=received_face){
    num = now_received_face;   
    Serial.print("received");
    Serial.print(num);
    Serial.println("\n");
    received_face = now_received_face;

	  if (received() == 1){   // send back
      if (Firebase.getString(firebaseData, mypost+"/name")){
        topost = String(firebaseData.stringData());
      }
	  }
	  eye(num%6);
  }
}
