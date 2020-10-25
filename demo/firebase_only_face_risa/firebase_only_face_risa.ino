#include <ArduinoJson.h>
#include <IOXhop_FirebaseStream.h>
#include <IOXhop_FirebaseESP32.h>
#include <M5Stack.h>
#include <WiFi.h>


//初期値設定
int num = 0;


//wifi
#define WIFI_SSID "**************"//自分のルーターのSSIDに変更してください
#define WIFI_PASSWORD "****************" //自分のルーターのパスワードに変更してください

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

}



//****メインループ********************
void loop() {
  M5.update();
  //M5.Lcd.setCursor(10, 100);
  
  eye(num%6);

  //***実行内容***
  //A,Cボタンでアバターを選択してBボタンで送る
  //相手がアバターを更新したら、それを検知して変更を反映させる
    
  //Aボタン→減少、Cボタン→増加　Bボタン→データ送信
  if (M5.BtnA.wasReleased()) {
    num--;
    M5.Lcd.fillScreen(BLACK);
  }

  if (M5.BtnC.wasPressed()){
    num++;
    M5.Lcd.fillScreen(BLACK);
  }
  
  if(M5.BtnB.wasReleased()) {
    Firebase.setInt("/M5Stack/MovingFace/num", num);
    Firebase.setInt("/M5Stack/MovingFace/new", 2222);
    M5.Lcd.setCursor(50, 170);
    M5.Lcd.print("send!");
    delay(1000);
    M5.Lcd.fillScreen(BLACK);

  }

//  //new=1111になっているのは相手が更新した証拠
  if(int(Firebase.getFloat("/M5Stack/MovingFace/new"))==1111){
    num =  int(Firebase.getFloat("/M5Stack/MovingFace/num"));
    Firebase.setInt("/M5Stack/num/new", 0);
    M5.Lcd.fillScreen(BLACK);
  }

}
