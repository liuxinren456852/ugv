const int pinESC = 9;
const int pinServo = 11;
const int frontPin = 4;
const int rightPin = 5;
const int leftPin = 6;

int cmd = 0;

void goForward();
void turnLeft();
void turnRight();
void stopMotors();
//void handshake();

void setup(){

pinMode(frontPin, INPUT);
pinMode(leftPin, INPUT);
pinMode(rightPin, INPUT);

pinMode(pinESC, OUTPUT);
pinMode(pinServo, OUTPUT);

stopMotors();



}


void loop(){


  if (frontPin == HIGH) goForward();
  
  else if (leftPin == HIGH) turnLeft();
  
  else if (rightPin == HIGH) turnRight();
    
  else if (cmd == 's') stopMotors();

delay(5);

}

void goForward(){
analogWrite(pinServo, 160); // test this
analogWrite(pinESC, 180); // test this 

}

void turnLeft(){
analogWrite(pinServo, 230); // test this
analogWrite(pinESC, 180); // decrease speed 

}

void turnRight(){
analogWrite(pinServo, 100); // test this
analogWrite(pinESC, 180); // decrease speed 

}

void stopMotors(){
analogWrite(pinServo, 0); // test this
analogWrite(pinESC, 0); // decrease speed 

}

//void handshake(char cmd){
//  
//  if (cmd == 'f') Serial.write("cmd: going forward");
//  
//  else if (cmd == 'l') Serial.write("cmd: turning left");
//  
//  else if (cmd == 'r') Serial.write("cmd: turning right");
//    
//  else if (cmd == 's') Serial.write("cmd: stropping motors");
//
//  
//  
//}
