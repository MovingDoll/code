#include <M5Stack.h>

#define pi  M_PI // pi
#define SCRW    320 // width of screen
#define SCRH    240 // height of screen
#define ANGRY   0
#define SLEEPY  1
#define HAPPY   2
#define SAD     3
#define DOUBT   4
#define NEUTRAL 5
#define FILL    'f' // circle filled
#define LINE    'l' // circle line
#define EYEC    WHITE // eye color

void line(int x1,int y1,int x2,int y2,int thick);
void circle(int x,int y,int r,char mode);
void eye();

/*void setup(){M5.begin();}
void loop(){eye(ANGRY);}*/

void eye(char feeling){
    int eyew = 130; // width of eyes
    int eyeh = 100; // height of eyes
    int eyeR = 25;  // radius of eye line
    int eyer = 12;  // radius of hitomi
    int eyed = 7;   // distance between eye line & hitomi
    int eyeD = 3;     // distance between eye & eyebrow
    int ebth = 4;     // thickness of eyebrow
    int eblen = eyeR*2*1.2; // length of eyebrow
    int mth = 150; //height of mouth

    M5.Lcd.fillScreen(BLACK);
 
    // eyes
    circle(SCRW/2-eyew/2, eyeh, eyeR, LINE);
    circle(SCRW/2+eyew/2, eyeh, eyeR, LINE);
    circle(SCRW/2-eyew/2+eyeR-eyer-eyed, eyeh, eyer, FILL);
    circle(SCRW/2+eyew/2-eyeR+eyer+eyed, eyeh, eyer, FILL);
    
    // eyebrow
    double theta = 0;   // angle of eyebrow
    switch(feeling){
        case ANGRY:   theta = pi/6;   eyeD *= -4;   break;
        case SLEEPY:  theta = 0;      eyeD *= -4;   break;
        case HAPPY:   theta = 0;      eyeD *=  4;   break;
        case SAD:     theta = -pi/8;  eyeD *= -2;   break;
        case DOUBT:   theta = 0;      eyeD *= -1;   break;
        case NEUTRAL: break;
    }

    //mouth
    int mtr = 0; //radius of mouth
    int mths = 0;
    switch(feeling){
      case ANGRY: mtr = 100; mths = 100; break;
      case SLEEPY: mtr = 10; break;
      case HAPPY: mtr = 30 ; break;
      case SAD: mtr = 20; break;
      case DOUBT: mtr= 5; break;
      case NEUTRAL: break;
    }

    circle(SCRW/2, mth+mths , mtr, LINE);
  
    int xl1 = SCRW/2-eyew/2+eyeR;       // start position of left eyebrow
    int yl1 = eyeh-eyeR-eyeD;           // start position of left eyebrow
    int xr1 = SCRW/2+eyew/2-eyeR;       // start position of right eyebrow
    int yr1 = eyeh-eyeR-eyeD;           // start position of right eyebrow
    
    int xl2 = xl1 - cos(theta)*eblen;   // end position of left eyebrow
    int yl2 = yl1 - sin(theta)*eblen;   // end position of left eyebrow
    int xr2 = xr1 + cos(theta)*eblen;   // end position of right eyebrow
    int yr2 = yr1 - sin(theta)*eblen;   // end position of right eyebrow
 
    if (feeling != NEUTRAL){
        line(xl1,yl1,xl2,yl2,ebth);
        line(xr1,yr1,xr2,yr2,ebth);
    }

    printAcc(); // MyIP, ToIP, Battery
}

void circle(int x, int y, int r, char s){
    for (int i=x-r; i<=x+r; i++){
        for (int j=y-r; j<=y+r; j++){
            if (s == FILL){
                if ((i-x)*(i-x)+(j-y)*(j-y)<=r*r){
                    M5.Lcd.drawPixel(i,j,EYEC);
            }}
            if (s == LINE){
                if ((i-x)*(i-x)+(j-y)*(j-y)<=r*r*1.1 && (i-x)*(i-x)+(j-y)*(j-y)>=r*r*0.9){
                    M5.Lcd.drawPixel(i,j,EYEC);
      }}
}}}
void line(int x1,int y1,int x2,int y2, int t){
    int l;
    for (int i=min(x1,x2); i<=max(x1,x2); i++){
        for (int j=min(y1,y2); j<=max(y1,y2); j++){
            l = abs( abs(j-y1)*abs(x2-x1) - abs(y2-y1)*abs(i-x1) );
            if (l<=20){
                circle(i,j,t/2,FILL);
}}}}
void rectangle(int x1, int y1, int x2, int y2){
    for (int i=min(x1,x2); i<=max(x1,x2); i++){
        for (int j=min(y1,y2); j<=max(y1,y2); j++){
            M5.Lcd.drawPixel(i,j,WHITE);  
}}}
