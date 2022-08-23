#include<Arduino.h>

// Declaring All Variables As Integers
int Z,Y,X,W,F;

// Function for Displaying
void disp_7447(int D, int C, int B, int A)
{
  digitalWrite(2, A); //LSB
  digitalWrite(3, B); 
  digitalWrite(4, C); 
  digitalWrite(5, D); //MSB

}


// Initialising Input and Output Pins of Arduino
void setup() {
    pinMode(2, OUTPUT);  
    pinMode(3, OUTPUT);
    pinMode(4, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(6, INPUT);  
    pinMode(7, INPUT);
    pinMode(8, INPUT);
    pinMode(9, INPUT);
    
}

// The Main Loop Function
void loop() {
X = digitalRead(6);//LSB  
Y = digitalRead(7);  
Z = digitalRead(8);  
W = digitalRead(9);//MSB

F=(X&&!Y)||(X&&W)||(X&&Z)|(Y&&Z)||(Z&&!W);

if(F==1){
disp_7447(0,0,0,1);
}
else if (F==0) {
disp_7447(0,0,0,0);
} 
}


}
