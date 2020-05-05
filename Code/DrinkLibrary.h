#ifndef dl
#define dl

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

class DrinkLib {
  public:
  //Constructor
  DrinkLib();
  //Methods
  String getDrinkName();
  int getDrinkNum();
  void setDrink(int drinkSelect);
  private:
  //Drink Variables 
  String drinkName;
  int drinkNum;
};
#endif
