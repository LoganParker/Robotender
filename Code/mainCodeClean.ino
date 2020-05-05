#include <LiquidCrystal.h>
#include "DrinkLibrary.h"
//Construct drink library
DrinkLib drinks;


/*(MOTORS)
  /You will need this for every motor connected(8 motors, 4 Booze, 4 Mix.) Probably a good idea to name to the actual drink(IE vodka) and have set spots

  Set Pin num once assembled
*/
const int ALC1_RELAY_PIN = 31;
const int ALC2_RELAY_PIN = 32;
const int ALC3_RELAY_PIN = 33;
const int ALC4_RELAY_PIN = 34;

const int MIX1_RELAY_PIN = 35;
const int MIX2_RELAY_PIN = 36;
const int MIX3_RELAY_PIN = 37;
const int MIX4_RELAY_PIN = 38;
const int NUM_RELAYS = 8;
/*LCD SETUP
  LCD Pins - these are default pins, we can fuck with later.
  I am currently using pins specified from circut diagram
*/
const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal screen(rs, en, d4, d5, d6, d7);
/*    Rotary Encoder
  inputCLK and inputDT determine position of encoder.
  SW Controls the buttonpress on the encoder.
  Counter is the encoder value, changes with position of encoder.
  currentState is the current position of the CLK line.
  previousState is prev state of encoder. used to check if moving.
  encDir is the direction the encoder is moving(Clockwise/Counterclockwise)
  Note encDir not currently in use.
*/
//    Pins
#define SW 14
#define inputCLK 2
#define inputDT 3
//Variables
int counter = 0;
int currentState;
int previousState;
//String encDir = "";
/*    Variables for pour duration

*/
const unsigned long singleShotDuration = 1000;
const unsigned long singleMixDuration = 2000;
unsigned long alcPour;
unsigned long mixPour;

void setup() {
  /*      LCD SETUP
     columns(16) / rows(2)
     columns = max char available per line, rows = how many lines available
  */
  screen.begin(16, 2);
  /*      ROTARY ENCODER SETUP
    //encoder pin as inputs
  */
  pinMode(inputCLK, INPUT);
  pinMode(inputDT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  //start the cereal :)
  Serial.begin(9600);
  /*      MOTOR SETUP
    setup relay pins for every relay connected
  */
  pinMode(ALC1_RELAY_PIN, OUTPUT);
  pinMode(ALC2_RELAY_PIN, OUTPUT);
  pinMode(ALC3_RELAY_PIN, OUTPUT);
  pinMode(ALC4_RELAY_PIN, OUTPUT);

  pinMode(MIX1_RELAY_PIN, OUTPUT);
  pinMode(MIX2_RELAY_PIN, OUTPUT);
  pinMode(MIX3_RELAY_PIN, OUTPUT);
  pinMode(MIX4_RELAY_PIN, OUTPUT);

  for (int i = 0; i < NUM_RELAYS; i++) {
    closeRelay(i);
  }

  
  //Read pos of encoder and assign to previous state
  previousState = digitalRead(inputCLK);
}

void loop() {
  //prompt on top line for shot selection and then move cursor to bottom line.
  int shotSelect = -1;
  nextMenu("How many shots?");
  //select shot number.
  while (shotSelect < 0) {
    //Encoder movement function, setting lower limit to 0 shots and upper to 2 shots.
    encoderMovement(0, 2);
    //Displays the # of shots
    clearLineTwo();
    screen.print(counter);
    //if current # shot selected, store and reset counter.
    Serial.println(shotSelect);
    if (buttonPressed()) {
      shotSelect = counter;
      counter = 0;
      Serial.println("if statement executed.");
      Serial.println(shotSelect);
    }
  }
  //selecting drink
  nextMenu("Pick your drink!");
  int drinkSelect = -1;
  while (drinkSelect < 0) {
    Serial.println("Drink select Loop executed");
    //0 and 4 are temp values until we decide on drink options
    encoderMovement(0, 4);
    clearLineTwo();
    drinks.setDrink(counter);
    screen.print(drinks.getDrinkName());
    if (buttonPressed()) {
      drinkSelect = counter;
      counter = 0;
    }
  }

  /*Confirm or cancel selection. If selection is confirmed, Pour the drink
    otherwise, reset and jump to top of loop.
  */
  nextMenu(" Press to pour!");
  int confirmSelect = -1;
  while (confirmSelect < 0) {
    encoderMovement(0, 1);
    clearLineTwo();
    if (counter == 0) {
      screen.print(drinks.getDrinkName());
    }else {
      screen.print("Cancel?");
    }
    if (buttonPressed()) {
      confirmSelect = counter;
      counter = 0;
    }
  }
  
  
  if (confirmSelect == 0) {
    pour(drinks.getDrinkNum(), shotSelect);
  }
  else{
    Serial.println("Selection was cancelled.");
  }












}
//Function for encoder movement
void encoderMovement(int lowestNum, int highestNum) {
  Serial.println(counter);
  currentState = digitalRead(inputCLK); //Checks the current position of the encoder.
  if (currentState != previousState) { //if wheel is turned
    //if inputDT state is different than inputCLK(remember encoders have 2 different wave lookin things)
    //then the encoder is turning counter-clockwise
    if (digitalRead(inputDT) != currentState) {
      counter--;
      //if the counter is at 0 and you try to scroll further to the left, set it to the next highest option(2)
      if (counter < lowestNum) {
        counter = highestNum;
      }
    } else {
      //if that first statement is not true then it must be moving clockwise.
      counter++;
      // if the counter scrolls up past the highest option, jump back down to your other one
      if (counter > highestNum) {
        counter = lowestNum;
      }
    }
  }
  previousState = currentState;
}
//buttonPressed function for the rotary encoder
bool buttonPressed() {
  if (digitalRead(SW) == LOW) {
    Serial.println("Button Pushed.");
    //Waits until user releases button
    while(digitalRead(SW) == LOW){
      Serial.println("Waiting for button release");
    }
    return true;
  } else {
    Serial.println("Button not pushed.");
    return false;
  }
}
//This function prints the title of the menu and then sets the cursor to be on the second line
void nextMenu(String menuTitle) {
  screen.clear();
  screen.setCursor(0, 0);
  screen.print(menuTitle);
  screen.setCursor(0, 1);
}
//Clears bottom line of screen without affecting title.
void clearLineTwo() {
  screen.setCursor(0, 1);
  screen.print("                ");
  screen.setCursor(0,1);
}
//Activates relay
void openRelay(int relayNum) {
  switch (relayNum) {

    //Relays activate when pin is LOW
    case 0:
      digitalWrite(ALC1_RELAY_PIN, LOW);
      // sprintf(openMSG, "Relay: %d at pin %d opened.", relayNum, ALC1_RELAY_PIN);
      // Serial.println(openMSG);
      break;
    case 1:
      digitalWrite(ALC2_RELAY_PIN, LOW);
      //sprintf(openMSG, "Relay: %d at pin %d opened.", relayNum, ALC2_RELAY_PIN);
      //Serial.println(openMSG);
      break;
    case 2:
      digitalWrite(ALC3_RELAY_PIN, LOW);
      //sprintf(openMSG, "Relay: %d at pin %d opened.", relayNum, ALC3_RELAY_PIN);
      // Serial.println(openMSG);
      break;
    case 3:
      digitalWrite(ALC4_RELAY_PIN, LOW);
      // sprintf(openMSG, "Relay: %d at pin %d opened.", relayNum, ALC4_RELAY_PIN);
      // Serial.println(openMSG);
      break;
    case 4:
      digitalWrite(MIX1_RELAY_PIN, LOW);
      //   sprintf(openMSG, "Relay: %d at pin %d opened.", relayNum, MIX1_RELAY_PIN);
      // Serial.println(openMSG);
      break;
    case 5:
      digitalWrite(MIX2_RELAY_PIN, LOW);
      //      sprintf(openMSG, "Relay: %d at pin %d opened.", relayNum, MIX2_RELAY_PIN);
      //     Serial.println(openMSG);
      break;
    case 6:
      digitalWrite(MIX3_RELAY_PIN, LOW);
      //      sprintf(openMSG, "Relay: %d at pin %d opened.", relayNum, MIX3_RELAY_PIN);
      //      Serial.println(openMSG);
      break;
    case 7:
      digitalWrite(MIX4_RELAY_PIN, LOW);
      //      sprintf(openMSG, "Relay: %d at pin %d opened.", relayNum, MIX4_RELAY_PIN);
      //    Serial.println(openMSG);
      break;
  }
}
//Deactivates relay
void closeRelay(int relayNum) {
  switch (relayNum) {
    case 0:
      digitalWrite(ALC1_RELAY_PIN, HIGH);
      //  sprintf(closedMSG, "Relay: %d at pin %d closed.", relayNum, ALC1_RELAY_PIN);
      // Serial.println(closedMSG);
      break;
    case 1:
      digitalWrite(ALC2_RELAY_PIN, HIGH);
      //      sprintf(closedMSG, "Relay: %d at pin %d closed.", relayNum, ALC2_RELAY_PIN);
      //      Serial.println(closedMSG);
      break;
    case 2:
      digitalWrite(ALC3_RELAY_PIN, HIGH);
      //     sprintf(closedMSG, "Relay: %d at pin %d closed.", relayNum, ALC3_RELAY_PIN);
      //    Serial.println(closedMSG);
      break;
    case 3:
      digitalWrite(ALC4_RELAY_PIN, HIGH);
      //   sprintf(closedMSG, "Relay: %d at pin %d closed.", relayNum, ALC4_RELAY_PIN);
      // Serial.println(closedMSG);
      break;
    case 4:
      digitalWrite(MIX1_RELAY_PIN, HIGH);
      //    sprintf(closedMSG, "Relay: %d at pin %d closed.", relayNum, MIX1_RELAY_PIN);
      //   Serial.println(closedMSG);
      break;
    case 5:
      digitalWrite(MIX2_RELAY_PIN, HIGH);
    //  sprintf(closedMSG, "Relay: %d at pin %d closed.", relayNum, MIX2_RELAY_PIN);
    //  Serial.println(closedMSG);
    case 6:
      digitalWrite(MIX3_RELAY_PIN, HIGH);
      //  sprintf(closedMSG, "Relay: %d at pin %d closed.", relayNum, MIX3_RELAY_PIN);
      //  Serial.println(closedMSG);
      break;
    case 7:
      digitalWrite(MIX4_RELAY_PIN, HIGH);
      //  sprintf(closedMSG, "Relay: %d at pin %d closed.", relayNum, MIX4_RELAY_PIN);
      // Serial.println(closedMSG);
      break;
  }
}
//Pour drink
void pour(int drinkNum, int shotSelect) {
  switch (drinkNum) {
    case 0:
      //Open relays connected to liquor
      openRelay(0);
      //open relays connected to Mix
      openRelay(3);
      Serial.println("Pouring");
      //figure out your delays
      delay(5000);
      closeRelay(0);
      closeRelay(4);
      break;
    case 1:
      openRelay(1);
      openRelay(5);

      break;
    case 2:
      openRelay(2);
      openRelay(6);
      break;
    case 3:
      openRelay(3);
      openRelay(7);
      break;
    case 4://fuckmeup
      openRelay(0);
      openRelay(1);
      openRelay(2);
      openRelay(3);
      delay(3000);
      closeRelay(0);
      closeRelay(1);
      closeRelay(2);
      closeRelay(3);
      break;
  }
}
//Relay timer
void pourTimer(int shotNum) {
  alcPour = millis();

}
