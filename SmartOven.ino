#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
byte time = 5;
int temp = 0;

void setup() {
  lcd.begin(20, 4);
  pinMode(3, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);
  pinMode(1, INPUT_PULLUP);
  pinMode(13, OUTPUT); // Oven output
}

void loop() {
  mainMenu();
}

void showMainMenu() {
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("START");

  lcd.setCursor(2, 1);
  lcd.print("TEMP");

  lcd.setCursor(2, 2);
  lcd.print("TIME");
}

void mainMenu() {
  showMainMenu();

  lcd.setCursor(0, temp);
  lcd.print('>');
  while(true) {
    byte key = getKeys();
    if(key != 0) {
      lcd.setCursor(0, temp);
      lcd.print(' ');
    }
    switch(key) {
      case 3: //UP
        if(temp > 0) {
          temp = temp - 1;
        }
        lcd.setCursor(0,temp);
        lcd.print('>');
        waitForKeyRelease();
        break;
    case 2: //down
      if(temp < 2) {
        temp = temp + 1;
      }
      lcd.setCursor(0,temp);
      lcd.print('>');
      waitForKeyRelease();
      break;
    case 1://OK
      switch(temp){
        case 0 : 
          startWork();
          break;
        case 1 : 
          tempWork();
          break;
        case 2 : 
          setTime();
          break;
      }
      showMainMenu();
      lcd.setCursor(0, temp);
      lcd.print('>');
      waitForKeyRelease();
      break;
    }
  }
  delay(50);
}

void startWork() {
  time = EEPROM.read(0);
  digitalWrite(13, HIGH);
  lcd.clear();
  lcd.setCursor(9, 1);
  lcd.print("ON");
  lcd.setCursor(2,2);
  lcd.print("TIME LEFT: ");
  for (int i = time; i >= 0 ; i = i - 1) {
    lcd.setCursor(13, 2);
    lcd.print(i);
    delay(1000);
  }
  digitalWrite(13, LOW);
  lcd.clear();
  lcd.setCursor(2, 2);
  lcd.print("FINISHED");
  delay(2000);
}

void setTime() {
  time = EEPROM.read(0);
  byte key = 0;
  lcd.clear();
  lcd.setCursor(1, 2);
  lcd.print("TIME: ");
  lcd.setCursor(9, 2);
  lcd.print(time);
  while(true) {
    key = getKeys();
    if(key != 0) {
      lcd.setCursor(9, 2);
      lcd.print(time);
    }
    switch(key){
      case 3 : 
        if(time < 60) {
          time = time + 1;
          lcd.setCursor(9, 2);
          lcd.print(time);
        }
        waitForKeyRelease();
        break;
      case 2:
        if(time > 5) {
          time = time - 1;
          lcd.setCursor(9, 2);
          lcd.print(time);
        }
        waitForKeyRelease();
        break;
      case 1:
        EEPROM.write(0, time);
        mainMenu();
    }
  }
}

void tempWork() {
  lcd.clear();
  lcd.setCursor(1, 3);
  lcd.print("TEMP");
  delay(500);
}

// This Method have gotten from sample code menu 6
byte getKeys(){
  if(digitalRead(1) == LOW){
    delay(20);
    if(digitalRead(1) == LOW)
      return 1;    
  }

  if(digitalRead(2) == LOW) {
    delay(20);
    if(digitalRead(2) == LOW)
      return 2;
  }

  if(digitalRead(3) == LOW){
    delay(20);
    if(digitalRead(3) == LOW)
      return 3;
  }  
  return 0;
}

// This Method have gotten from sample code menu 6
void waitForKeyRelease(){
  do{
    while(getKeys() != 0) //wait for keys to be released
    {}
    delay(20);
  }while(getKeys() != 0);
} //repeat if the keys are not released
