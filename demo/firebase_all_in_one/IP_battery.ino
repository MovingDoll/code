String iptostr(IPAddress ip){
	String res = "/";
	for (int i = 0; i < 4; i++){
    if (ip[i]<10) { res+= "0"; }
    if (ip[i]<100){ res+= "0"; }
		res += String(ip[i]);
	}
	return res;
}
void printAcc(){
    M5.Lcd.setTextSize(1);
    // battery
    rectangle(270,0,285,7);
    rectangle(285,2,288,5);
    M5.Lcd.setCursor(290, 0);
    M5.Lcd.print(getBatteryLevel());
    M5.Lcd.print("%");
    // My IP
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.print("My IP: ");
    printIP(ip1);
    // connected IP
    M5.Lcd.setCursor(0,10);
    M5.Lcd.print("To IP: ");
    printIP(ip2);
}
void printIP(IPAddress ip){
  for (int i = 0; i < 4; i++){
    M5.Lcd.print(ip[i]);
    if (i < 3){ M5.Lcd.print("."); }
  }  
}
int8_t getBatteryLevel(){
	Wire.beginTransmission(0x75);
	Wire.write(0x78);
	if (Wire.endTransmission(false) == 0 && Wire.requestFrom(0x75, 1)) {
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
