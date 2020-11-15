#include <M5Stack.h>
#include <Wire.h>
#include "porthub.h"

PortHub porthub;
uint8_t HUB_ADDR[6]={HUB1_ADDR,HUB2_ADDR,HUB3_ADDR,HUB4_ADDR,HUB5_ADDR,HUB6_ADDR};

void setup(){
	M5.begin();
	M5.Power.begin();
	angle_input();
}
void loop(){}

void angle_input(){
	int angle;
	int toip[4] = {0};
	int digit = 0;
	char confirm = 0;
	char changed = 0;
	while (confirm == 0){
		M5.update();
		angle = porthub.hub_a_read_value(HUB_ADDR[0]) / 3.5;

		if (M5.BtnA.wasPressed()){ changed = 1; if (digit > 0){ digit--; } }
		if (M5.BtnB.wasPressed()){ changed = 1; confirm = 1; }
		if (M5.BtnC.wasPressed()){ changed = 1; if (digit < 4){ digit++; } }

		if (angle != toip[digit] || changed){
			toip[digit] = angle;
      changed = 0;
			M5.Lcd.fillScreen(BLACK);
			M5.Lcd.setCursor(0, 0);
			M5.Lcd.setTextSize(2);
			M5.Lcd.printf("%d\%", getBatteryLevel());

			M5.Lcd.setCursor(80, 100);
			for (int i = 0; i < 4; i++){
				if (i == digit){
					M5.Lcd.setTextColor(BLACK, WHITE);
				} else {
					M5.Lcd.setTextColor(WHITE, BLACK);
				}
				M5.Lcd.print(toip[i]);
				M5.Lcd.setTextColor(WHITE, BLACK);
				if (i < 3){ M5.Lcd.print("."); }
			}

			M5.Lcd.setTextSize(3);
			M5.Lcd.setCursor(45, 210); M5.Lcd.print("<-");
			M5.Lcd.setCursor(145, 210); M5.Lcd.print("OK");
			M5.Lcd.setCursor(235, 210); M5.Lcd.print("->");
		}
    delay(100);
	}
}
