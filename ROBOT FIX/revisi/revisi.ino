
#include <Servo.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd (13,12,11,10,8,7);
Servo scanservo;   //Ping Sensor Servo
const int A1A = 16;//define pin 2 for A1A
const int A1B = 5;//define pin 3 for A1B

const int B1A = 4;//define pin 8 for B1A
const int B1B = 0;//define pin 9 for B1B

#define trigPin  2
#define echoPin  14
const int scanservopin = 8;
const int distancelimit = 15; //in cm

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("SREMPET GUDAL");
  lcd.setCursor(0,1);
  lcd.print("READY");

  // put your setup code here, to run once:
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
  pinMode(B1A,OUTPUT);// define pin as output
  pinMode(B1B,OUTPUT);
  
  pinMode(A1A,OUTPUT);
  pinMode(A1B,OUTPUT);   
scanservo.attach(scanservopin); // Attach the scan servo
delay(2000);      // wait two seconds
}

void loop() {
  // put your main code here, to run repeatedly:
go();
int jarak = ping();
  if (jarak<distancelimit){
    stopp();
    char turndir = scan();
        switch (turndir){
            case 'l':
            turnleft();
            break;

            case 'r':
            turnright();
            break;

            case 's':
            go();
            break;
            }
  }

}

int ping(){

  long duration, distance;

  //Send Pulse

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  //Read Echo
  duration = pulseIn(echoPin, HIGH);
  // convert the time into a distance
  distance = (duration/2) / 29.1;
  Serial.println(distance);
 
  return round(distance);

}

void go(){
  motorA('L');// Turn motor A to RIGHT
  motorB('L'); // Turn motor A to RIGHT 
}
void turnleft(){
  motorA('L');// Turn motor A to RIGHT
  motorB('R'); // Turn motor A to RIGHT
  delay(100);
  motorA('L');// Turn motor A to RIGHT
  motorB('L'); // Turn motor A to RIGHT  
}
void turnright(){
  motorA('R');// Turn motor A to RIGHT
  motorB('L'); // Turn motor A to RIGHT 
  delay(100);
  motorA('L');// Turn motor A to RIGHT
  motorB('L'); // Turn motor A to RIGHT 
}

void stopp(){
  motorA('R');// Turn motor A to RIGHT
  motorB('R'); // Turn motor A to RIGHT
  delay(100);
  motorA('O');// Turn motor A to RIGHT
  motorB('O'); // Turn motor A to RIGHT  
}
void motorA(char d)

{
  if(d =='R'){
    digitalWrite(A1A,LOW);
    digitalWrite(A1B,HIGH); 
  }else if (d =='L'){
    digitalWrite(A1A,HIGH);
    digitalWrite(A1B,LOW);    
  }else{
    //Robojax.com L9110 Motor Tutorial
    // Turn motor OFF
    digitalWrite(A1A,LOW);
    digitalWrite(A1B,LOW);    
  }
}// motorA end

void motorB(char d)
{

    if(d =='R'){
      digitalWrite(B1A,LOW);
      digitalWrite(B1B,HIGH); 
    }else if(d =='L'){
      digitalWrite(B1A,HIGH);
      digitalWrite(B1B,LOW);    
    }else{
    //Robojax.com L9110 Motor Tutorial
    // Turn motor OFF      
      digitalWrite(B1A,LOW);
      digitalWrite(B1B,LOW);     
    }

}// motorB end 

char scan(){

  int leftscanval, centerscanval, rightscanval;
  char choice;

  //Look left
  scanservo.write(178);
  delay(300);
  leftscanval = ping();

  //Look right
  scanservo.write(4);
  delay(1000);
  rightscanval = ping();

  //center scan servo
  scanservo.write(83);
  delay(500);
  centerscanval = ping();
 

  if ((leftscanval>rightscanval) && (leftscanval>centerscanval)){
  choice = 'r';
  }

  else if ((rightscanval>leftscanval) && (rightscanval>centerscanval)){
  choice = 'l';
  }

  else{
  choice = 's';
  }

  return choice;

}



