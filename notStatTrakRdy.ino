#include <Arduino.h>
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 4
#define DIO 5

boolean point_up = true;
boolean point_down = true;
boolean dual = false;
boolean StatTrak = true;
boolean oneLoop = false;

int Current_Number=0;

TM1637Display display(CLK, DIO);

const uint8_t SEG_UNKN[] = {
  SEG_D | SEG_E | SEG_F | SEG_C | SEG_B,           // U
  SEG_E | SEG_G | SEG_C,                           // n
  SEG_E | SEG_G | SEG_C | SEG_F | SEG_A,           // K
  SEG_E | SEG_G | SEG_C                            // n
  };

const uint8_t SEG_USER[] = {
  SEG_D | SEG_E | SEG_F | SEG_C | SEG_B,           // U
  SEG_A | SEG_F | SEG_G | SEG_C | SEG_D,           // S
  SEG_A | SEG_F | SEG_G | SEG_E | SEG_D,           // E
  SEG_E | SEG_G                                    // r
  };

void setup() {
  // put your setup code here, to run once:

  //Display Brightness
  display.setBrightness(0x0f);

  display.showNumberDec(Current_Number, true);

  //POINT DOWN
  //Interrupt on digital pin D3 linked to button K1
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(3), Press_PointDown,RISING);

  //POINT UP
  //Interrupt on digital pin D2 linked to button K4
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), Press_PointUp,RISING);
}

void loop() {
  handleInput();
  displayState();

}

void displayState() {
  if (StatTrak == true){
    display.showNumberDec(Current_Number, true);
  }
  if (StatTrak == false){
    display.setSegments(SEG_UNKN);
    delay(1500);
    oneLoop = true;
    display.setSegments(SEG_USER);
    delay(1500);
  }
}

void handleInput(){

  point_down = digitalRead(3);
  point_up = digitalRead(2);
  dual = (point_down == LOW && point_up == LOW);
  
  if (dual){
    StatTrak = false;
    oneLoop = false;
  }
  else if (!point_up) {
    if (Current_Number == 9999) {
      //Does nothing
    }
    else {
      Current_Number++;
    }
    StatTrak = true;
    delay(100);
  }
  else if (!point_down) {
    if (Current_Number == 0) {
      //Does nothing
    }
    else {
      Current_Number--;
    }
    StatTrak = true;
    delay(100);
  }
  else {
  }
}

void Press_PointDown() {
  if (oneLoop == true) {
    StatTrak = true;
  }
}
 
void Press_PointUp() {
  if (oneLoop == true) {
    StatTrak = true;
  }
}
