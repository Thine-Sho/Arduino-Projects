#include <Servo.h>

#include <Keypad.h>

byte sequence = 1;

const byte rows = 4;
const byte cols = 4;


const byte key_length = 6;
const byte verified = 8;
char userKey[key_length];
bool access = true;

const char masterKey[verified][key_length] = {
  {'#', 'A', '9', 'D', '0', '*'},
  {'#', 'B', '7', 'C', '*', '*'},
  {'#', 'D', '2', '#', '1', '*'},
  {'#', 'C', 'D', '7', '3', '*'},
  {'#', 'B', '3', '*', '0', '*'},
  {'#', '6', '#', '8', '4', '*'},
  {'#', '1', '5', '#', '6', '*'},
  {'#', 'C', '9', '7', '3', '*'}
};

const char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte row_pins[rows] = {9, 8, 7, 6};
byte col_pins[cols] = {5, 4, 3, 2};

Keypad keypad = Keypad( makeKeymap(keys), row_pins, col_pins, rows, cols );

//####### SERVO ###########
const int servos = 2;
Servo servo[servos];
const byte servo_pins[servos] = {14, 15};

//####### other ###########
const int led = 10;
const int dc_motor = 16;

//#############
//#############

void setup()
{
	for(int i=0; i<servos; i++)
    {
      servo[i].write(0);
      servo[i].attach(servo_pins[i]);
    }
  	pinMode(led, OUTPUT);
  	pinMode(dc_motor, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
  Serial.print("\nSEQUENCE INITIATED: ");
  Serial.print(sequence++);
  Serial.print("\n");
  
  validation();
  delay(2000);
  
  Serial.print("SEQUENCE FINISHED\n\n");
}

//#############
//#############


//Custom Functions 
void turnServo()
{
	Serial.print("\nServo Function started\n");
  	for(int i=0; i<180; i++)
    {
    	servo[0].write(i);
      	servo[1].write(i);
      	delay(10);
    }
  	delay(1500);
   	for(int i=180; i>0; i--)
    {
    	servo[0].write(i);
        servo[1].write(i);
      	delay(30);
    }
  	Serial.print("\nServo Function ended\n");
}

void lightSignal(int pin, int ondelay, int offdelay){
	digitalWrite(pin, HIGH);
  	delay(ondelay);
 	digitalWrite(pin, LOW);
  	delay(offdelay);
}

void getKey()
{
  	Serial.print("Code: ");
	for(int i=0; i<key_length; i++)
    {	digitalWrite(led, LOW);
    	char newKeyPad = keypad.waitForKey();
      	if(newKeyPad)
        {	
          	userKey[i] = newKeyPad;
          	digitalWrite(led, HIGH);
        	Serial.print(userKey[i]);
        }
    }
  
  Serial.print("\n\nKey Logged\n\n");
}

bool keyIsValid(const char uKey[], const char mKey[][6])
{
  for(int i=0; i<verified; i++)
  {
    if(uKey[2] != mKey[i][2] && uKey[5] != mKey[i][5])
    {
      	Serial.print("Verified Index: "); Serial.print(i);
      	Serial.print(" checked ~ FALSE\n");
    	continue;
    }else if(uKey[4] == mKey[i][4] && uKey[1] == mKey[i][1])
    {
      if(uKey[3] == mKey[i][3] && uKey[5] == '*')
      {
        if(uKey[0] == '#' && uKey[2] == mKey[i][2] )
        {
            Serial.print("Verified Index: "); Serial.print(i);
            Serial.print(" checked ~ TRUE\n");
			return true;
          	break;
        }
      }
    }
  }
  Serial.print("## Key not in table ##\n");
  return false;
}

void validation()
{
  getKey();
  if(keyIsValid(userKey, masterKey))
  {
  	Serial.print("\nCorrect Key Sequence, Please Enter\n");
    turnServo(); //Function Runs ONLY IF keyisValid() == TRUE
  }else{
  	Serial.print("\nIncorrect Key Sequence\n");
    digitalWrite(dc_motor, 500); delay(1000);
    digitalWrite(dc_motor, 0);
  }
  Serial.print("\n#######  Validation Finished  #######\n");
}