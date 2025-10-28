#include <Keypad.h>
#include <Stepper.h>

// ----- Keypad Setup -----
const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// ----- LED Morse Code -----
int LASER_PIN = 11;
int orderAmount = 0;
char buttonMemory = '\0';

// ----- Stepper Setup -----
#define STEPS 200   // steps per revolution
Stepper stepper(STEPS, A2, A3, A4, A5);
int previousStep = 0;  // current stepper position

// ----- Morse Functions -----
void dot() { digitalWrite(LASER_PIN,HIGH); delay(250); digitalWrite(LASER_PIN,LOW); delay(500);}
void dash(){ digitalWrite(LASER_PIN,HIGH); delay(750); digitalWrite(LASER_PIN,LOW); delay(500);}
void startOrder(){ dot(); dash(); dot(); dash(); Serial.println("Start Order"); delay(750);}
void endOrder(){ dot(); dash(); dot(); dash(); dot(); dash(); orderAmount=0; buttonMemory='\0'; Serial.println("System reset"); delay(750);}
void commPattern(int repeats,bool isDash){ for(int i=0;i<repeats;i++){ if(isDash) dash(); else dot(); } }

// ----- Utility -----
void printStatus(){ Serial.print("Order amount: "); Serial.println(orderAmount); Serial.print("Button Memory: "); Serial.println(buttonMemory); }

// ----- Smooth Analog Read -----
int smoothedRead(int pin){
  long sum=0;
  const int samples=10;
  for(int i=0;i<samples;i++){ sum += analogRead(pin); delay(2); }
  return sum / samples;
}

// ----- Setup -----
void setup(){
  pinMode(LASER_PIN, OUTPUT);
  stepper.setSpeed(10);  // slower speed = smooth
  Serial.begin(9600);
  Serial.println("System Ready");
}

// ----- Main Loop -----
void loop(){
  // ----- Keypad + Morse -----
  char button = customKeypad.getKey();
  if(button){
    Serial.print("Button Pressed: "); Serial.println(button);

    if(button=='1') commPattern(1,true);
    else if(button=='2') commPattern(2,true);
    else if(button=='3') commPattern(3,true);
    else if(button=='A') commPattern(4,true);
    else if(button=='4') commPattern(1,false);
    else if(button=='5') commPattern(2,false);
    else if(button=='6') commPattern(3,false);
    else if(button=='B') commPattern(4,false);
    else if(button=='7'){ dash(); dot();}
    else if(button=='8'){ dash(); dot(); dash(); dot();}
    else if(button=='9'){ dash(); dot(); dash(); dot(); dash();}
    else if(button=='C'){ dash(); dot(); dash(); dot(); dash(); dot();}
    else if(button=='#') startOrder();
    else if(button=='D') endOrder();
    else digitalWrite(LASER_PIN, LOW);

    if(button != '#'){ buttonMemory=button; orderAmount++; }
    else{ orderAmount=0; }

    printStatus();
    delay(750);
  }

  // ----- Potentiometer Control (Stepper) -----
  int potVal = smoothedRead(A0);
  int targetStep = map(potVal, 0, 1023, 0, STEPS); // map full pot sweep to full motor revolution

  int diff = targetStep - previousStep;
  if(abs(diff) >= 1){           // move only if significant
    stepper.step(diff);
    previousStep = targetStep;   // update absolute position
  }

  delay(10);  // small delay for smooth motion
}
