SYSTEM_THREAD(ENABLED)

#include "avr/pgmspace.h"

// setup pins for input and output
int SOLENOID = D0;
int DOWN = D1; //input for swiping down
int UP = D3; //input for swiping up
int LEFT = D4; //input for swiping left (referred to as backward in sensor docs)
int RIGHT = D5; //input for swiping right (referred to as forward in sensor docs)
int TAP = D2; //input for tap
int LED = D7; // indicator diode

int inp = 0; // variable for reading input states
int count =0; // variable for array location

// create passcode arrays
int Passcode[]={1,2,3,4}; // 1=up, 2=down, 3=left, 4=right
int Testcode[4];

void setup() {
    pinMode(SOLENOID,OUTPUT);
    pinMode(DOWN,INPUT);
    pinMode(UP,INPUT);
    pinMode(LEFT,INPUT);
    pinMode(RIGHT,INPUT);
    pinMode(TAP,INPUT);
    pinMode(LED,OUTPUT);
    digitalWrite(LED,LOW);  //initialize LED to be off
    digitalWrite(SOLENOID,LOW);  //initialize LED to be off
}

void loop() {
    if(digitalRead(TAP)==HIGH){
        inp=0;
    }
    else if(digitalRead(UP)==HIGH){
        inp=1;
    }
    else if(digitalRead(DOWN)==HIGH){
        inp=2;
    }
    else if(digitalRead(LEFT)==HIGH){
        inp=3;
    }
    else if(digitalRead(RIGHT)==HIGH){
        inp=4;
    }
    else{
        inp=-1;
    }
    if(inp>-1){
        digitalWrite(LED,HIGH);
        delay(200);
        digitalWrite(LED,LOW);
        delay(500);
    }
    if(inp==0){
        count=0; //reset to first digit of combo
    }
    else if(inp>0){
        Testcode[count]=inp;  //store digit in the code to be tested at location count
        count++;  //get ready for next digit
        for(int n=0;n<inp;n++){
            digitalWrite(LED,HIGH);
            delay(500);
            digitalWrite(LED,LOW);
            delay(500);
        }
    }
    if(count==4){
        if(memcmp_P(Testcode,Passcode,4)==0){
             digitalWrite(SOLENOID,HIGH); //this actuates the solenoid
            delay(1000);
            digitalWrite(SOLENOID,LOW);
        }
    }
}
