#include <Keypad.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <Servo.h>
const byte numRows = 4;
const byte numCols = 4;
char keymap[numRows][numCols] =
    {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}};
char keypressed;
char code[] = {'2', '0', '2', '1'};
char code_buff1[sizeof(code)];
char code_buff2[sizeof(code)];

short a = 0, i = 0, s = 0, j = 0;
int index = 0;

byte rowPins[numRows] = {2, 3, 4, 6};
byte colPins[numCols] = {8, 9, 10, 11};

LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
Servo myservo;
bool check_IO();
void ChangeCode();
void GetCode();
void GetNewCode1();
void GetNewCode2();
void OpenDoor();

void setup()
{
  pinMode(5,OUTPUT);
  pinMode(12,INPUT);
  pinMode(1,OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.print("HVKT MAT MA");
  myservo.attach(7);
  myservo.write(0);
  // digitalWrite(0,LOW);
}

void loop()
{
  if (index == 3)
  {
    digitalWrite(5, HIGH);
  }
    if (check_IO() == false)
  {
    myservo.write(90);
  }
  else
  {
    myservo.write(0);
  }
  keypressed = myKeypad.getKey();
  Serial.println(keypressed);
  if (keypressed == '*')
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Nhap Password: ");
    GetCode();
    if (a == sizeof(code))
    OpenDoor();
    else
    {
      lcd.clear();
      lcd.print("Sai Password ");
      index++;
    }
    delay(1500);
    lcd.clear();
    lcd.print("HVKT MAT MA");
  }

  if (keypressed == '#')
  {
    ChangeCode();
    lcd.clear();
    lcd.print("HVKT MAT MA");
  }
}

void GetCode()
{
  i = 0;
  a = 0;
  j = 0;

  while (keypressed != 'A')
  {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A')
    {
      lcd.setCursor(j, 1);
      lcd.print("*");
      j++;
      if (keypressed == code[i] && i < sizeof(code))
      {
        a++;
        i++;
      }
      else
        a--;
    }
  }
  keypressed = NO_KEY;
}

void ChangeCode()
{
  lcd.clear();
  lcd.print("Doi Password: ");
  delay(1000);
  lcd.clear();
  lcd.print("Password Cu: ");
  GetCode();

  if (a == sizeof(code))
  {
    index = 0;
    lcd.clear();
    lcd.print("Thay Doi ");
    GetNewCode1();
    GetNewCode2();
    s = 0;
    for (i = 0; i < sizeof(code); i++)
    {
      if (code_buff1[i] == code_buff2[i] && (code_buff1[i] != '\0') && (code_buff2[i] != '\0'))
        s++;
    }
    if (s == sizeof(code))
    {

      for (i = 0; i < sizeof(code); i++)
      {
        code[i] = code_buff2[i];
        EEPROM.put(i, code[i]);
      }
      lcd.clear();
      lcd.print("Thanh Cong! ");
      delay(2000);
    }
    else
    {
      lcd.clear();
      lcd.print("That Bai!");
      delay(2000);
    }
  }

  else
  {
    lcd.clear();
    lcd.print("Sai Password");
    delay(2000);
  }
}

void GetNewCode1()
{
  i = 0;
  j = 0;
  lcd.clear();
  lcd.print("Password Moi: ");
  lcd.setCursor(0, 1);
  lcd.print("Xac Nhan: A");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Xac Nhan: A");

  while (keypressed != 'A')
  {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A')
    {
      lcd.setCursor(j, 0);
      lcd.print("*");
      code_buff1[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void GetNewCode2()
{
  i = 0;
  j = 0;

  lcd.clear();
  lcd.print("Xac Nhan Lai");
  lcd.setCursor(0, 1);
  lcd.print("Xac Nhan :A");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Xac Nhan :A");

  while (keypressed != 'A')
  {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A')
    {
      lcd.setCursor(j, 0);
      lcd.print("*");
      code_buff2[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void OpenDoor()
{
  index = 0;
  lcd.clear();
  lcd.print("Mo Cua!");
  digitalWrite(1,HIGH);
  myservo.write(90);
  delay(2000);
  myservo.write(0);
  digitalWrite(1,LOW);
  digitalWrite(5, LOW);
}
bool check_IO()
{
  if (digitalRead(12) == 1)
  {
    return true;
  }
  return false;
}