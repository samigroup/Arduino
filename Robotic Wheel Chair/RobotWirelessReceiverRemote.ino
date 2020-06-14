
/*
  Started off from:
  Analog input, analog output, serial output
  http://www.arduino.cc/en/Tutorial/AnalogInOutSerial
*/
// Sami Oweis
// 06/12/2020


// These constants won't change. They're used to give names to the pins used:
// Input signals from the Wireless Remote:
const int FRPin = A2;  // Analog input pin that the potentiometer is attached to
const int RLPin = A0;

// Output signals to PWM signals:
const int RMotorPin = 10; // Analog output pin that the LED is attached to
const int LMotorPin = 9;

// value read from the pot
int FRValue = 0;        // value read from Front/Reverse Switch.
int RLValue = 0;        // value read from Right/Left Switch.

// value output to the PWM (analog out)
int RMValue = 0;        // value output to Right Motor PWM (analog out)
int LMValue = 0;        // value output to Left Motor PWM (analog out)


const int RMFWDMValue  = 85 ;
const int LMFWDMValue  = 85 ;

const int RMRvrsMValue = 240;
const int LMRvrsMValue = 240;

const int RMRightValue = 190;
const int LMRightValue = 95 ;

const int RMLeftValue = 95 ;
const int LMLeftValue = 190;


const int RMSTOPValue = 0;
const int LMSTOPValue = 0;

const int StateSTOP  = 0;
const int StateFRWD  = 1;
const int StateRevrs = 2;
const int StateRight = 3;
const int StateLeft  = 4;
const int StateRightRev = 5;
const int StateLeftRev  = 6;


const int RMLeftRevValue = 210 ;
const int LMLeftRevValue = 225;

const int RMRightRevValue = 225 ;
const int LMRightRevValue = 210;

int MotorState = StateSTOP;



/*
    Frwd A0 <  100;
    Rvrs A0 > 1000;

    Left A2 > 1000;
    Right A2 < 100;

*/


/**********************************************
  Lifebeat Signal:
  900 - 980

**********************************************/


/***********************************************
   Wiring for Testing:

   Pot 10K Wiring;
   Pin 1 (Left)    - 5V
   Pin 2 (Center)  - OUT
   Pin 3 (Right)   - GND
*/

/*
    LED Wiring:
    Long Leg +v5, Resistor,
    Short Leg GND

*/


/**********************************************
    Wiring:

   Left I/O is connected to A2.
   Reverse I/O is connected to A0

   Pwr -- VIN
   GND -- GND

   Motor PWM
   White - PWM to 5V,
   GND is what being controled by the PWM I/O

   2N222a Transistor


**********************************************/

/**********************************************
    Warning:
    We need to protect for PIN discount.

 ***********************************************/

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
}

void MoveFRWD() {

  RMValue = RMFWDMValue;
  LMValue = LMFWDMValue;

  analogWrite(RMotorPin, RMValue);
  analogWrite(LMotorPin, LMValue);

}
void MoveRevrs() {

  RMValue = RMRvrsMValue;
  LMValue = LMRvrsMValue;

  analogWrite(RMotorPin, RMValue);
  analogWrite(LMotorPin, LMValue);

}
void MoveRight() {

  RMValue = RMRightValue;
  LMValue = LMRightValue;

  analogWrite(RMotorPin, RMValue);
  analogWrite(LMotorPin, LMValue);

}
void MoveLeft() {

  RMValue = RMLeftValue;
  LMValue = LMLeftValue;

  analogWrite(RMotorPin, RMValue);
  analogWrite(LMotorPin, LMValue);

}

void MoveSTOP() {

  RMValue = RMSTOPValue;
  LMValue = LMSTOPValue;

  analogWrite(RMotorPin, RMValue);
  analogWrite(LMotorPin, LMValue);

}
void MoveRightRev() {

  RMValue = RMRightRevValue;
  LMValue = RMRightRevValue;

  analogWrite(RMotorPin, RMValue);
  analogWrite(LMotorPin, LMValue);

}
void MoveLeftRev() {

  RMValue = RMLeftRevValue;
  LMValue = RMLeftRevValue;

  analogWrite(RMotorPin, RMValue);
  analogWrite(LMotorPin, LMValue);

}

void loop() {
  // read the analog in value:

  // Read Remote signal
  FRValue = analogRead(FRPin);
  RLValue = analogRead(RLPin);

  // Get the FRMotor Direction


  // map it to the range of the analog out:
  //outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:

  if (FRValue < 100 )
  {
    MotorState = StateFRWD;
    //  outputValue = 85 ; // map(sensorValue, 0, 1023, 0, 255);

 if (RLValue < 100 )
    {
      MotorState = StateRight;
      //  outputValue = 85 ; // map(sensorValue, 0, 1023, 0, 255);
  
    }
    if (RLValue > 1000 ) {
  
      MotorState = StateLeft;
      // outputValue = 240 ; // map(sensorValue, 0, 1023, 0, 255);
    }
  }
  if (FRValue > 1000 ) {

    MotorState = StateRevrs;
    // outputValue = 240 ; // map(sensorValue, 0, 1023, 0, 255);
 if (RLValue < 100 )
    {
      MotorState = StateRightRev;
      //  outputValue = 85 ; // map(sensorValue, 0, 1023, 0, 255);
  
    }
    if (RLValue > 1000 ) {
  
      MotorState = StateLeftRev;
      // outputValue = 240 ; // map(sensorValue, 0, 1023, 0, 255);
    }
 }

/*
if ((FRValue < 100) | ( FRValue > 1000)){
// If Moving check Side Direction :   
    if (RLValue < 100 )
    {
      MotorState = StateRight;
      //  outputValue = 85 ; // map(sensorValue, 0, 1023, 0, 255);
  
    }
    if (RLValue > 1000 ) {
  
      MotorState = StateLeft;
      // outputValue = 240 ; // map(sensorValue, 0, 1023, 0, 255);
    }
}
*/

// STOP   
  if ((FRValue > 100) & ( FRValue < 1000))
  {
    // outputValue = 0 ; // map(sensorValue, 0, 1023, 0, 255);
    MotorState = StateSTOP;
  }

  
  
  switch (MotorState) {
    
    case StateFRWD:
      MoveFRWD();
      break;

    case StateRevrs:
      MoveRevrs();
      break;

    case StateRight:
      MoveRight();
      break;

    case StateLeft:
      MoveLeft();
      break;

    case StateSTOP:
      MoveSTOP();
      break;
    
    case StateLeftRev:
      MoveLeftRev();
      break;
    case StateRightRev:
      MoveRightRev();
      break;
      
    default:
      MoveSTOP();
      break;

  }



  // analogWrite(analogOutPin, outputValue);

  // print the results to the Serial Monitor:
  Serial.print("Remote State = ");
  Serial.print("");
  Serial.print("\t FRWD/Rvers  = ");
  Serial.print(FRValue);
  Serial.print("\t  Right/Left = ");
  Serial.print(RLValue);
  Serial.print("\t");

  Serial.print("Motor State = ");
  Serial.print(MotorState);
  Serial.print("\t output R = ");
  Serial.print(RMValue);
  Serial.print("\t output L = ");
  Serial.print(LMValue);
  Serial.println("");

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
