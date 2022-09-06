#include<Arduino.h>

// Declaring All Variables As Integers
int Z,Y,X,W,F;

// Function for Displaying
void disp_7447(int D, int C, int B, int A)
{
  digitalWrite(6, A); //LSB
  digitalWrite(7, B); 
  digitalWrite(8, C); 
  digitalWrite(9, D); //MSB

}


// Initialising Input and Output Pins of Arduino
void setup() {
    pinMode(6, OUTPUT);  
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(2, INPUT);  
    pinMode(3, INPUT);
    pinMode(4, INPUT);
    pinMode(5, INPUT);
    
}

// The Main Loop Function
void loop() {
X = digitalRead(2);//LSB  
Y = digitalRead(3);  
Z = digitalRead(4);  
W = digitalRead(5);//MSB

F=(X&&!Y)||(X&&W)||(Y&&Z)||(Z&&!W);

if(F==1){
disp_7447(0,0,0,1);
}
else if (F==0) {
disp_7447(0,0,0,0);
} 
}


}
