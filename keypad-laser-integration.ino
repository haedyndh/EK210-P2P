#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; 
byte colPins[COLS] = {5, 4, 3, 2}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int LASER_PIN = 11;

void printStatus() { //troubleshooting function
Serial.print("Order amount: "); Serial.println(orderAmount); Serial.print("Button Memory: "); Serial.println(buttonMemory); }

// -- Morse Code functions -- 

//function for dot
void dot() { digitalWrite(LASER_PIN, HIGH); delay(250); digitalWrite(LASER_PIN, LOW); delay(500); }

//function for dash
void dash(){ digitalWrite(LASER_PIN, HIGH); delay(750); digitalWrite(LASER_PIN, LOW); delay(500); }

//function for start order
void startOrder() { dot(); dash(); dot(); dash(); Serial.println("Start Order"); delay(750);  }

//function for add to order
//void addToOrder() { dot(); dash(); delay(750); }

//function for end order
void endOrder() {  dot(); dash(); dot(); dash(); dot(); dash();  orderAmount = 0; buttonMemory = '\0'; Serial.println("System reset: order ended."); delay(750); }

void commPattern(int repeats, bool isDash){
  for(int i = 0; i < repeats; i++){
    if(isDash) dash();
    else dot();
  }
}

int orderAmount = 0; //Order amount tracker + troubleshooting var
char buttonMemory = '\0'; //End of order assist var

void setup(){
  pinMode(LASER_PIN, OUTPUT);       // Laser as output
  Serial.begin(9600);
}
  
void loop(){
  char buttonState = customKeypad.getKey();

// !! Functions for each message type (dot, dash, space, end of word, add to order, end of order, start order)
if (!buttonState) {
  return;
} 


if (buttonState == '1') {
  //drink 1 -> one dash
  commPattern(1, true); // sends one dash
  printStatus();

  delay(750);
  } 

else if (buttonState == '2') {
  commPattern(2, true); //two dashes
  printStatus();
  
  delay(750);
} 

else if (buttonState == '3') {
  commPattern(3, true);
  printStatus(); 

  delay(750);
  
}  

else if (buttonState == 'A') {
  commPattern(4, true);
  printStatus();

  delay(750);
  
}   

else if (buttonState == '4') {
  commPattern(1, false); //one dot
  printStatus();

  delay(750);

}

else if (buttonState == '5') {
  commPattern(2, false);
  printStatus();

  delay(750);

}

else if (buttonState == '6') {
  commPattern(3, false);
  printStatus();  

  delay(750);

}

else if (buttonState == 'B') {
  commPattern(4, false);
  printStatus();

  delay(750);

}

else if (buttonState == '7') {
  for (int i = 1; i < 2; i++) {
    dash(); dot();
  }

  printStatus(); 

  delay(750);

}

else if (buttonState == '8') {
  for (int i = 1; i < 3; i++) {
    dash(); dot();
  }

  printStatus(); 

  delay(750);

}

else if (buttonState == '9') {
  for (int i = 1; i < 4; i++) {
    dash(); dot();
  }

  printStatus();  

  delay(750);

}

else if (buttonState == 'C') {
  for (int i = 1; i < 5; i++) {
    dash(); dot();
  }

  printStatus(); 

  delay(750);

}

else if (buttonState == '#') {
  startOrder();
}

else if (buttonState == 'D') {
  endOrder();
}

else {
  //no button pressed
  digitalWrite(LASER_PIN, LOW);
}

if (buttonState != '#') {
  buttonMemory = buttonState;
  orderAmount += 1;

} else {
  orderAmount = 0;
}

}