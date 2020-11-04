#include <M5Stack.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST	"m5stack-data.firebaseio.com"
#define FIREBASE_AUTH	"*************" // enter auth copied from database above

#define WIFI_SSID	"*************" // enter the ssid of your wi-fi
#define WIFI_PASSWORD	"*************" // enter PW

FirebaseData firebaseData;

int i = 0;

void setup(){
	M5.begin();
  M5.Power.begin();
  Serial.begin(115200);

  // connecting to WiFi
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);  
  }
  
	M5.Lcd.println("Connecting to IP:");
	M5.Lcd.println(WiFi.localIP());
	M5.Lcd.println();
	
	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
	
  Firebase.reconnectWiFi(true);
	
}

void loop(){
	i++;
	Firebase.setInt(firebaseData, "/test", i);
	delay(100);
  Serial.println(i);
}
