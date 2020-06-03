#include <LiquidCrystal.h>

#include <Keypad.h>

const int rows = 4;
const int columns = 4;
const int key_length = 5;

char code[key_length];
char master_key[key_length] = {'#', 'A', '8', '7', '*'};

char keys[rows][columns] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte row_pins[rows] = {9, 8, 7, 6};
byte col_pins[columns] =  {5, 4, 3, 2};

Keypad keypad = Keypad( makeKeymap(keys), row_pins, col_pins, rows, columns);
//###### LCD ######
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);

void setup(){
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.print("Welcome!!!");
  delay(2000);
}
  
void loop(){
  signIn();
}

//Prints text to the LCD
void lcdPrintOut(const String item, const int delayBy)
{
  	lcd.setCursor(0, 1);
  	lcd.print(item);
  delay(delayBy);
}

//Retreives each Key value and assigns
//key value to array
void getCode()
{
  for(int i=0; i<key_length; i++){
    char customKey = keypad.waitForKey();
    if (customKey){
      code[i] = customKey;
      Serial.println(code[i]);
      lcd.print("*");
    }
  }
}

//returns TRUE if Key format is correct
bool codeIsValid(char item[])
{
  if(item[0] != '#'){
  	Serial.print("\nInvalid Code: Missing '#'\n");
    lcdPrintOut("Invalid Code:'#'", 1000);
    signIn();
  }else{return true;}
  
  return false;
}

//returns TRUE if correct Key was inputed
bool keyIsValid(char inputKey[], char masterKey[])
{
  for(int i=0; i<key_length; i++)
  {
    if(inputKey[i] != masterKey[i]){
      	Serial.print("\nIncorrect Key\n");
      	lcdPrintOut("Incorrect Key", 1000);
    	return false;
    }
  }
  return true;
}

//Sends user through if everything correct
void signIn(){
  lcd.clear();
  lcd.print("Enter Key: ");
  lcd.noBlink();delay(500); 
  lcd.blink();delay(500); 
  
  getCode();
  
  if(codeIsValid(code))
  {
    if(keyIsValid(code, master_key))
    {
      Serial.print("\nValid Key\n");
      lcdPrintOut("Valid Key", 1000);
      lcd.clear();
      //main(); //this function runs after logIn completed;
    }
  }
  
  Serial.print("\nDone\n");
}