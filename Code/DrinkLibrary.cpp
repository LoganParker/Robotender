#include "DrinkLibrary.h"

DrinkLib::DrinkLib()  {

}
//Sets drinks so i dont have to keep using fuckin switches
void DrinkLib::setDrink(int drinkSelect) {
  switch (drinkSelect) {
    case 0:
      drinkName = "drink0";
      drinkNum = 0;
      break;
    case 1:
      drinkName = "drink1";
      drinkNum = 1;
      break;
    case 2:
      drinkName = "drink2";
      drinkNum = 2;
      break;
    case 3:
      drinkName = "drink3";
      drinkNum = 3;
      break;
    case 4:
      drinkName = "drink4";
      drinkNum = 4;
      break;
  }
}
//Returns the name of the drink - Primarily for menu display.
String DrinkLib::getDrinkName() {
  return drinkName;
}
//Returns drink num for pour function.
int DrinkLib::getDrinkNum(){
  return drinkNum;
}
