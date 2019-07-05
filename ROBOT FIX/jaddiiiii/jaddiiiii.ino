
#include <Servo.h>
#include<LiquidCrystal.h>
LiquidCrystal lcd (13,12,11,10,8,7);
Servo scanservo;   //Ping Sensor Servo
#define motorkif  2
#define motorkib  3
#define motorkaf  4
#define motorkab  5
#define trigPin  A1
#define echoPin  A0
const int scanservopin = 6;
const int distancelimit = 10; //in cm

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("SREMPET GUDAL");
  lcd.setCursor(0,1);
  lcd.print("READY");

  // put your setup code here, to run once:
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(motorkif, OUTPUT);
pinMode(motorkib, OUTPUT);
pinMode(motorkaf, OUTPUT);
pinMode(motorkab, OUTPUT);
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
 
  return round(distance);

}

void go(){
digitalWrite(motorkif,HIGH);
digitalWrite(motorkib,LOW);
digitalWrite(motorkaf,HIGH);
digitalWrite(motorkab,LOW);
 
}
void turnleft(){
 digitalWrite(motorkif,LOW);
digitalWrite(motorkib,HIGH);
digitalWrite(motorkaf,HIGH);
digitalWrite(motorkab,LOW);
delay(500);
digitalWrite(motorkif,HIGH);
digitalWrite(motorkib,LOW);
digitalWrite(motorkaf,HIGH);
digitalWrite(motorkab,LOW);
}

void turnright(){
digitalWrite(motorkif,HIGH);
digitalWrite(motorkib,LOW);
digitalWrite(motorkaf,LOW);
digitalWrite(motorkab,HIGH);
delay(500);
digitalWrite(motorkif,HIGH);
digitalWrite(motorkib,LOW);
digitalWrite(motorkaf,HIGH);
digitalWrite(motorkab,LOW);
}
void turnaround(){

digitalWrite(motorkif,HIGH);
digitalWrite(motorkib,LOW);
digitalWrite(motorkaf,LOW);
digitalWrite(motorkab,HIGH);
delay(1000);
digitalWrite(motorkif,HIGH);
digitalWrite(motorkib,LOW);
digitalWrite(motorkaf,HIGH);
digitalWrite(motorkab,LOW);
}
void stopp(){
digitalWrite(motorkif,LOW);
digitalWrite(motorkib,LOW);
digitalWrite(motorkaf,LOW);
digitalWrite(motorkab,LOW);
delay(1000);
}

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



