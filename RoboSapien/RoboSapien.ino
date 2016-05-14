#define RSTurnRight       0x80
#define RSRightArmUp      0x81
#define RSRightArmOut     0x82
#define RSTiltBodyRight   0x83
#define RSRightArmDown    0x84
#define RSRightArmIn      0x85
#define RSWalkForward     0x86
#define RSWalkBackward    0x87
#define RSTurnLeft        0x88
#define RSLeftArmUp       0x89
#define RSLeftArmOut      0x8A
#define RSTiltBodyLeft    0x8B
#define RSLeftArmDown     0x8C
#define RSLeftArmIn       0x8D
#define RSStop            0x8E
#define RSWakeUp          0xB1
#define RSBurp            0xC2
#define RSRightHandStrike 0xC0
#define RSNoOp            0xEF
#define RSRightHandSweep  0xC1
#define RSRightHandStrike2 0xC3
#define RSHigh5           0xC4
#define RSFart            0xC7
#define RSLeftHandStrike  0xC8
#define RSLeftHandSweep  0xC9
#define RSWhistle         0xCA
#define RSRoar            0xCE

volatile int viRobsapienCmd = -1;  // A robosapien command sent over the UART request
const int analogPin = A0; //Aalog pin for the photoresistor; this pin will read in analog values from the photoresistor 
const int IROut= 3;            // Where the echoed command will be sent from
const int bitTime=516;          // Bit time (Theoretically 833 but 516)
int calibratedBrightness = 0;

void RSSendCommand(int command) {
  digitalWrite(IROut,LOW);
  delayMicroseconds(8*bitTime);
  for (int i=0;i<8;i++) {
    digitalWrite(IROut,HIGH);  
    delayMicroseconds(bitTime);
    if ((command & 128) !=0) delayMicroseconds(3*bitTime);
    digitalWrite(IROut,LOW);
    delayMicroseconds(bitTime);
    command <<= 1;
  }
  digitalWrite(IROut,HIGH);
  delay(250); // Give a 1/4 sec before next
}

/******************************************************** Arduino stuff *******************************************************/

void setup()
{
  Serial.begin(115200);
  pinMode(IROut, OUTPUT);
  digitalWrite(IROut, HIGH);
  calibratedBrightness = analogRead(analogPin);
}

void loop()
{ 
  int brightness = analogRead(analogPin) - calibratedBrightness; //Reads and maps input voltages between 0 and 5 volts into integer values between 0 and 1023 yielding a resolution between readings of 5 volts/1024 units via the analog pin and a resistor
  //Serial.println(brightness);
  if (brightness > 25)
  {
    RSSendCommand(RSWalkBackward);
    Serial.println("Backward");
    delay(2000);
    RSSendCommand(RSRightHandStrike);
    Serial.println("Strike");
    delay(1000);
    RSSendCommand(RSTurnRight);
    Serial.println("Turn Right");
    delay(3000);
  } 

  else 
  {
    RSSendCommand(RSWalkForward);
    Serial.println("Forward");
  }
}
