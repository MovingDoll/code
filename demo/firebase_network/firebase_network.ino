#include <ArduinoJson.h>
#include <M5Stack.h>
#include <WiFi.h>
#include <Wire.h>
#include <FirebaseESP32.h>

// wifi
#define WIFI_SSID "Buffalo-G-10D8"
#define WIFI_PW	"hcc468kcuhkk5"
// firebase
#define FIREBASE_URL "m5stack-data.firebaseio.com"
#define FIREBASE_AUTH "QCT6zw9CXbqM6TITH1AROK3HITZhNdhVtZTuu3HB"

// class
FirebaseData firebaseData;
IPAddress ip;

// function
String iptostr();
int8_t getBatteryLevel();

// variable
String mypost;

void setup(){
	M5.begin(); M5.Power.begin(); Serial.begin(115200);

	// wifi connection
	WiFi.begin(WIFI_SSID, WIFI_PW);
	Serial.print("connecting");
	while (WiFi.status() != WL_CONNECTED){ Serial.print("."); delay(500); }

	// firebase initialization
	Firebase.begin(FIREBASE_URL, FIREBASE_AUTH);
	Firebase.reconnectWiFi(true);

	// WiFi Connected
	Serial.println("\nWiFi Connected.");
	ip = WiFi.localIP();
	Serial.println(ip);
	
	// printscr
	M5.Lcd.fillScreen(BLACK);
	M5.Lcd.setCursor(0, 0);
	M5.Lcd.printf("Battery Level: %d\n", getBatteryLevel());
	for (int i = 0; i < 4; i++){
		M5.Lcd.println(String(ip[i]));
	}
	
	mypost = iptostr();
	M5.Lcd.print(mypost);
}

int cnt = 0;
void loop(){
	cnt++;
	Firebase.setInt(firebaseData, mypost, cnt);
	delay(1000);
}


String iptostr(){
	String res = "/";
	for (int i = 0; i < 4; i++){
    if (ip[i]<10) { res+= "0"; }
    if (ip[i]<100){ res+= "0"; }
		res += String(ip[i]);
	}
	return res;
}
int8_t getBatteryLevel(){
  Wire.beginTransmission(0x75);
  Wire.write(0x78);
  if (Wire.endTransmission(false) == 0
   && Wire.requestFrom(0x75, 1)) {
    switch (Wire.read() & 0xF0) {
    case 0xE0: return 25;
    case 0xC0: return 50;
    case 0x80: return 75;
    case 0x00: return 100;
    default: return 0;
    }
  }
  return -1;
}
