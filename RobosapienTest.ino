// RoboSapienRu.cpp
// RobosapienRu - test Robosapien hackspace.ru


// This project combines the default webserver example in the IDE distribution and Karl Castleton's(http://home.mesastate.edu/~kcastlet) RoboSapienIR (http://www.arduino.cc/playground/Main/RoboSapienIR).
// Source code merged from those two sources.

// Include files
//#include <string.h>

volatile int viRobsapienCmd = -1;  // A robosapien command sent over the UART request

//////////////////////////////////////////////////////////////////
// Begin Robosapien specific variable deinitions
//////////////////////////////////////////////////////////////////


// Some but not all RS commands are defined
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

// 
#define RSRightHandSweep  0xC1
#define RSRightHandStrike2 0xC3
#define RSHigh5           0xC4
#define RSFart            0xC7
#define RSLeftHandStrike  0xC8
#define RSLeftHandSweep  0xC9

#define RSWhistle         0xCA
#define RSRoar            0xCE

int command, buff[]={0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 
0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0xB1, 
0xC2, 0xC0, 0xEF, 0xC1, 0xC3, 0xC4, 0xC7, 0xC8, 0xC9};

int LedControl = 13;     // Show when control on 
int IROut= 3;            // Where the echoed command will be sent from
int bitTime=516;          // Bit time (Theoretically 833 but 516)
                          // works for transmission and is faster
//int last;                 // Previous command from IR





//////////////////////////////////////////////////////////////////
// Begin Robosapien specific code
//////////////////////////////////////////////////////////////////
// send the command 8 bits
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


// Set up RoboSpapien functionality
void RSSetup()                    
{
  pinMode(IROut, OUTPUT);
  pinMode(LedControl,OUTPUT);
  digitalWrite(IROut,HIGH);

  // Make robot burp to indicate setup is complete
  RSSendCommand(RSBurp);
}


// Loop for RoboSapien functionality
void RSLoop() 
{ 
  RSSendCommand(RSRightHandSweep);
  digitalWrite(LedControl,HIGH);
  // Has a new command come?
  if(viRobsapienCmd != -1)
    {
    // New command - send it to robot
    Serial.print("Sending command to RoboSapien: ");
    Serial.println(viRobsapienCmd, HEX);
    RSSendCommand(viRobsapienCmd);

    // Now clear command
    viRobsapienCmd = -1;
    }
  digitalWrite(LedControl,LOW);
}


void setup()
{
  Serial.begin(115200);
  Serial.println("RobSapien Start");

  RSSetup();
}

void loop()
{ 
  /*
  //Uncomment this to make RoboSapien show random movements  
  command=random(27);
  RSSendCommand(buff[command]);
  delay(1000);
  */  
  if (Serial.available() > 0) {
  // read the incoming byte:
  viRobsapienCmd = Serial.read();
  RSSendCommand(viRobsapienCmd);
  }  
  //RS routine
  RSLoop();
}
