#include <WiFi.h>
#include <M5Stack.h>
#include <IOXhop_FirebaseStream.h>
#include <IOXhop_FirebaseESP32.h>
#include <Avatar.h>
#include <faces/DogFace.h>

//avatar
using namespace m5avatar;
Avatar avatar;
Face* faces[2];
int faceIdx = 0;

const Expression expressions[] = {
  Expression::Angry,
  Expression::Sleepy,
  Expression::Happy,
  Expression::Sad,
  Expression::Doubt,
  Expression::Neutral
};
int expressIdx = 0;

//wifi
const char* ssid = "*******************"; //自分のルーターのSSIDに変更してください
const char* password = "****************:"; //自分のルーターのパスワードに変更してください

//firebase
const char* host = "**********************";
const char* firebase_auth = "*********************:::"; //database secrets

 
//*****セットアップ******************
void setup() {
  M5.begin();
//
//  
//  Serial.begin(115200);
//  //ツール→シリアルモニタ（右下の数値を115200bpsにする）
 
  WiFi.begin(ssid, password);
  Serial.print( "connecting" );
  while ( WiFi.status() != WL_CONNECTED ) {
    Serial.print( "." );//wifiに繋がらないと永遠と....が出てくる
    delay(500);
  }
  Serial.println();
  Serial.print( "connected: " );
  Serial.println(WiFi.localIP());
  
  Firebase.begin(host, firebase_auth);
  faces[0] = avatar.getFace();
  faces[1] = new DogFace();
  avatar.init();
}
//****メインループ********************
void loop() {
  M5.update();
//  if(avatar_init){
//    avatar.init();
//    avatar_init = false;
//  }
  
  int now_faceIdx = faceIdx;
  int now_expressIdx = expressIdx;
  faceIdx = int(Firebase.getFloat("/M5Stack/face"));
  expressIdx = int(Firebase.getFloat("/M5Stack/expression"));
  Serial.print("received number: ");
  Serial.println(faceIdx);
  Serial.println(expressIdx);
  if(now_faceIdx!=faceIdx){
    avatar.setFace(faces[faceIdx]);
  }
  if(now_expressIdx!=expressIdx){
    avatar.setExpression(expressions[expressIdx]);
  }

  delay(1000);

}
//***********************************
