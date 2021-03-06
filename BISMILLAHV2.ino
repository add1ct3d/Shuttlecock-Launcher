#include "U8glib.h"

U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE); // I2C 128x64(col2-col129) SH1106,Like HeiTec 1.3' I2C OLED 

#define RPWM 10
#define LPWM 11
#define L_EN 12
#define R_EN 13
#define potensio A7
#define KEY_NONE 0
#define KEY_UP 1
#define KEY_DOWN 2
#define KEY_SELECT 3
#define KEY_RESET 4
#define RESET 10

uint8_t uiKeyUp = A3;
uint8_t uiKeyDown = A2;
uint8_t uiKeySelect = A1;
uint8_t uiKeyReset = A0;

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;
uint8_t selected = RESET ;
int startTime;
void uiSetup(void) {

  pinMode(RPWM, OUTPUT);
  pinMode(LPWM,OUTPUT);
  digitalWrite(LPWM, LOW);
  digitalWrite(RPWM, LOW);
  pinMode(uiKeyUp, INPUT_PULLUP);           // set pin to input     // turn on pullup resistors
  pinMode(uiKeyDown, INPUT_PULLUP);           // set pin to input      // turn on pullup resistors
  pinMode(uiKeySelect, INPUT_PULLUP);           // set pin to input      // turn on pullup resistors
  pinMode(uiKeyReset, INPUT_PULLUP);           // set pin to input
         
}

void uiStep(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(uiKeyUp) == LOW )
  {
    uiKeyCodeFirst = KEY_UP;
  }
  else if ( digitalRead(uiKeyDown) == LOW )
  {
    uiKeyCodeFirst = KEY_DOWN;
  }
  else if ( digitalRead(uiKeySelect) == LOW )
  {
    uiKeyCodeFirst = KEY_SELECT;
    startTime = millis();
  }
  else if ( digitalRead(uiKeyReset) == LOW )
  {
    uiKeyCodeFirst = KEY_RESET;
    selected = RESET;
    //Stop();       
  }
  else 
  {
    uiKeyCodeFirst = KEY_NONE;
  }
  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = KEY_NONE;
}


#define MENU_ITEMS 2
char *menu_strings[MENU_ITEMS] = {"1. STATIS","2. DINAMIS" };

uint8_t menu_current = 0;
uint8_t menu_redraw_required = 0;
uint8_t last_key_code = KEY_NONE;


void drawMenu(void) {
  uint8_t i, h;
  u8g_uint_t w, d;

  u8g.setFont(u8g_font_9x18);
  u8g.setFontRefHeightText();
  u8g.setFontPosTop();
  
  h = u8g.getFontAscent()-u8g.getFontDescent();
  w = u8g.getWidth();
  for( i = 0; i < MENU_ITEMS; i++ ) {
    d = (w-u8g.getStrWidth(menu_strings[i]))/2;
    u8g.setDefaultForegroundColor();
    if ( i == menu_current ) {
      u8g.drawBox(0, i*h+1, w, h);
      u8g.setDefaultBackgroundColor();
    }
    u8g.drawStr(d, i*h, menu_strings[i]);
  }
}

void updateMenu(void) {
  if ( uiKeyCode != KEY_NONE && last_key_code == uiKeyCode ) {
    return;
  }
  last_key_code = uiKeyCode;
  
  switch ( uiKeyCode ) {
    case KEY_UP:
      menu_current++;
      if ( menu_current >= MENU_ITEMS )
        menu_current = 0;
      menu_redraw_required = 1;
      break;
    case KEY_DOWN:
      if ( menu_current == 0 )
        menu_current = MENU_ITEMS;
      menu_current--;
      menu_redraw_required = 1;
      break;
    case KEY_SELECT:
     // clearOLED();
      selected = menu_current;
      break;
    case KEY_RESET:
      selected = RESET;
  }
}
void modeStatis()
{
  int Nilai = analogRead(potensio);
  Nilai = map(Nilai,0,1023,0,185);
  motor(Nilai);
}

void modeDinamis()
{
  if ((millis() - startTime) >= 4000)
  {
  startTime = millis();
  }
  else
  {
  int randData = random(75,185);
  motor(randData);
  }
}

void motor (int val)
{
 digitalWrite(RPWM, LOW);
 analogWrite(LPWM, val);
}

void setup() {
  Serial.begin(9600);
  uiSetup();
  pinMode(7, OUTPUT);                          
  menu_redraw_required = 1;    
}

String KONDISI;
int infrared;
void loop() {
  infrared=analogRead(A6);
  if (infrared<=900)
  {
    KONDISI = "S";
    digitalWrite(7,LOW);
  }
  else
  {
    KONDISI = "T";
    digitalWrite(7,HIGH);
  }
  if(KONDISI == "S")
  {
  uiStep(); 
  if (selected == RESET)
  {                
  if (  menu_redraw_required != 0 ) {
    u8g.firstPage();
    do  {
      drawMenu();
    } while( u8g.nextPage() );
    menu_redraw_required = 0;
  }
  updateMenu();                            // update menu bar
  }
  else if (selected == 0)
  {
    Serial.println(KONDISI);
    modeStatis();
  }
  else if ( selected == 1)
  {
    Serial.println(KONDISI);
    modeDinamis();
  }
  else if (selected == 2)
  {
    Serial.println(KONDISI);
    selected = RESET;
  }
  }
  else
  {
    Serial.println(KONDISI);
    Stop();
  }
  delay(10);
}

void Stop()
{
 digitalWrite(RPWM, LOW);
 digitalWrite(LPWM, LOW);
}

