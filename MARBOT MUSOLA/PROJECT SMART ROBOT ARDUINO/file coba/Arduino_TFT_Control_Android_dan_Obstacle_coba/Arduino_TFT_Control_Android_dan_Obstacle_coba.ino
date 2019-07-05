/*  Arduino TFT Tutorial
 *  Program made by Dejan Nedelkovski,
 *  www.HowToMechatronics.com 
 */
 
/*  This program uses the UTFT and UTouch libraries
 *  made by Henning Karlsen. 
 *  You can find and download them at:
 *  www.RinkyDinkElectronics.com
 */

#include <UTFT.h> 
#include <URTouch.h>
#include <SoftwareSerial.h>
#include <Servo.h>
Servo scanservo;

#define trigPin  A1
#define echoPin  A0
const int scanservopin = 6;
const int distancelimit = 5; //in cm


int bluetoothTx = 1; // TX-O pin of BT module to Arduino pin2
int bluetoothRx = 0; // RX-I pin of B module to Arduino pin3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

int kiri1 = 7;
int kiri2 = 8;
int kanan1 = 9;
int kanan2 = 10;
char cmd[100];
int cmdIndex;


void exeCmd() {
  
  if(cmd[0]=='f') {  // maju
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, HIGH); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, HIGH); 
  } 
  
  if(cmd[0]=='b') {  // mundur
    digitalWrite(kiri1, HIGH);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, HIGH);  
    digitalWrite(kanan2, LOW); 
  } 
   
  if(cmd[0]=='l') { // kiri
    digitalWrite(kiri1, HIGH);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, HIGH);
  } 
  
  if(cmd[0]=='r') {  // kanan
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, HIGH); 
    digitalWrite(kanan1, HIGH);  
    digitalWrite(kanan2, LOW);
  } 
  
  if(cmd[0]=='s') {  // stop
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);
  } 
   if(cmd[0]=='a') {  // ambil
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);
   }
    if(cmd[0]=='p') {  // taruh
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);
    }
     if(cmd[0]=='z'){
          
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);     
     }
        if(cmd[0]=='x'){
          
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);     
     }
        if(cmd[0]=='c'){
 
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);      
     }
        if(cmd[0]=='v'){
          

    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);      
     }
        if(cmd[0]=='n'){
           
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW); 
    
     }
        if(cmd[0]=='m'){
          
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);    
     }

  
  
}

//==== Creating Objects
UTFT    myGLCD(CTE32HR,38,39,40,41); //Parameters should be adjusted to your Display/Schield model
URTouch  myTouch( 6, 5, 4, 3, 2);

//==== Defining Variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];


int x, y;

char currentPage, selectedUnit;

//Ultrasonic Sensor
const int VCC = 13;

long duration;
int distanceInch, distanceCm;

// RGB LEDs
const int redLed = 10;
const int greenLed = 9;
const int blueLed = 8;
int xR=38;
int xG=38;
int xB=38;

void setup() {
// Initial setup
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  // Defining Pin Modes
  pinMode(VCC, OUTPUT); // VCC
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  digitalWrite(VCC, HIGH); // +5V - Pin 13 as VCC

  drawHomeScreen();  // Draws the Home Screen
  currentPage = '0'; // Indicates that we are at Home Screen
  selectedUnit = '0'; // Indicates the selected unit for the first example, cms or inches
  pinMode(kiri1, OUTPUT);
  pinMode(kiri2, OUTPUT);
  pinMode(kanan1, OUTPUT);
  pinMode(kanan2, OUTPUT);
  
  // change Bluetooth module baud rate to 9600:
  
  bluetooth.begin(115200); // Bluetooth default baud is 115200
  
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$"); // enter cmd mode
  delay(250);  
  bluetooth.println("U,9600,N"); // change baud to 9600
  
  bluetooth.begin(9600);
  
  
  cmdIndex = 0;

pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

scanservo.attach(scanservopin); // Attach the scan servo


}

void loop() { 
  // Home Screen
  if (currentPage == '0') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX(); // X coordinate where the screen has been pressed
      y=myTouch.getY(); // Y coordinates where the screen has been pressed
      // If we press the Distance Sensor Button 
      if ((x>=35) && (x<=285) && (y>=90) && (y<=130)) {
        drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '1'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawDistanceSensor(); // It is called only once, because in the next iteration of the loop, this above if statement will be false so this funtion won't be called. This function will draw the graphics of the first example.
      }
      // If we press the RGB LED Control Button 
      if ((x>=35) && (x<=285) && (y>=140) && (y<=180)) {
        drawFrame(35, 140, 285, 180);
        currentPage = '2';
        myGLCD.clrScr();
        drawLedControl();
      }  
      // If we press ANDROID CONTROL
      if ((x>=35) && (x<=285) && (y>=190) && (y<=230)) {
        drawFrame(35, 190, 285, 230);
        currentPage = '3';
        myGLCD.clrScr();
        drawAndroidControl();
      }
      // If we press the next
      if ((x>=390) && (x<=270) && (y>=250) && (y<=290)) {
        drawFrame(390, 250, 270, 290);
        currentPage = '4';
        myGLCD.clrScr();
        drawHomeScreen2();
      }
    }
  }
  // Distance Sensor Example
  if (currentPage == '1') {    
      getDistance(); // Gets distance from the sensor and this function is repeatedly called while we are at the first example in order to print the lasest results from the distance sensor
      getRobot();
      if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
       
        // If we press the Centimeters Button
        if ((x>=10) && (x<=135) &&(y>=90) && (y<=163)) {
          selectedUnit = '0';
        }
        // If we press the Inches Button
        if ((x>=10) && (x<=135) &&(y>=173) && (y<=201)) {
          selectedUnit = '1';
        }
        // If we press the Back Button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0'; // Indicates we are at home screen
          myGLCD.clrScr();
          drawHomeScreen(); // Draws the home screen
        }
      }
  }

  
  // RGB LED Control 
  if (currentPage == '2') {
    setLedColor();
    if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
        
        //Back button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0';
          myGLCD.clrScr();
          drawHomeScreen();
          // Turns the LED off
          analogWrite(redLed, 0);
          analogWrite(greenLed, 0);
          analogWrite(blueLed, 0);
        }
    }
  }
//==== This section of the code, for the game example, is explained in my next tutorial
  // Birduino Game
  if (currentPage == '3') {
    androidControl();
        

        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0'; // Indicates we are at home screen
          myGLCD.clrScr();
          drawHomeScreen(); // Draws the home screen
        }
    }
      //next page
  if (currentPage == '4') {
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
    }
  }
}

// ====== Custom Funtions ======
// drawHomeScreen - Custom Function
void drawHomeScreen() {
  // Title
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("ARDUINO SMART ROBOT", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0,32,479,32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("by HowToMechatronics.com", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("MAIN MENU", CENTER, 64);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Developed by Arief Setyo Nugroho", CENTER, 300);
  
  // Button - Distance Sensor
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (90, 90, 390, 130); // Draws filled rounded rectangle (LEBAR KIRI,TINGGI BAWAH,LEBAR KANAN,BAWAH)
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (90, 90, 390, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("DISTANCE SENSOR", CENTER, 102); // Prints the string
  
  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (90, 140, 390, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (90, 140, 390, 180);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("RGB LED CONTROL", CENTER, 152);

  // Button - Android
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (90, 190, 390, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (90, 190, 390, 230);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("ANDROID ROBOT", CENTER, 202);

    // Button - Previous
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (90, 250, 210, 290);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (90, 250, 210, 290);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("PREV", (90,210,195,120), 262);

      // Button - Previous
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (390, 250, 270, 290);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (390, 250, 270, 290);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("NEXT", (10,210,195,297), 262);


}

// Highlights the button when pressed
void drawFrame(int x1, int y1, int x2, int y2) {
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawRoundRect (x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
    myGLCD.setColor(255, 255, 255);
    myGLCD.drawRoundRect (x1, y1, x2, y2);
}
//====================================================
void drawAndroidControl() {
 myGLCD.setColor(100, 155,203);
 myGLCD.setFont(BigFont);
 myGLCD.print("Android Robot Controller", CENTER, 50); 
}
//====================================================
void drawDistanceSensor() {
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  myGLCD.print("Ultrasonic Sensor", CENTER, 50);
  myGLCD.print("HC-SR04", CENTER, 76);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0,100,319,100);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Select Unit", 10, 114);
  myGLCD.setFont(BigFont);
  myGLCD.print("Distance:", 130, 120);
  myGLCD.setColor(223, 77, 55);
  myGLCD.fillRoundRect (10, 135, 90, 163);
  myGLCD.setColor(225, 255, 255);
  myGLCD.drawRoundRect (10, 135, 90, 163);
  myGLCD.setBackColor(223, 77, 55);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("cm", 33, 140);
  myGLCD.setColor(223, 77, 55);
  myGLCD.fillRoundRect (10, 173, 90, 201);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 173, 90, 201);
  myGLCD.setBackColor(223, 77, 55);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("inch", 17, 180);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Source code at: HowToMechatronics.com", CENTER, 220);  
}
//====================================================
//===== getDistance - Custom Function
void getDistance() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distanceCm= duration*0.034/2;
  distanceInch= distanceCm/2.53;
  // Prints the distance in centimeters
  if (selectedUnit == '0' && distanceCm <=400) {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(distanceCm,130, 145, 3,'0');
    myGLCD.setFont(BigFont);
    myGLCD.print("cm  ", 235, 178);
  
  }
  // Prints the distance in inches
  if (selectedUnit == '1' && distanceCm <=160) {
    myGLCD.setFont(SevenSegNumFont);
    myGLCD.setColor(0, 255, 0);
    myGLCD.setBackColor(0, 0, 0);
    myGLCD.printNumI(distanceInch,130, 145, 3,'0');
    myGLCD.setFont(BigFont);
    myGLCD.print("inch", 235, 178);
  } 
  delay(10);
}
void getRobot() {
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
            turnaround();
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
digitalWrite(kiri1,HIGH);
digitalWrite(kiri2,LOW);
digitalWrite(kanan1,HIGH);
digitalWrite(kanan2,LOW);
 
}
void turnleft(){
digitalWrite(kiri1,LOW);
digitalWrite(kiri2,HIGH);
digitalWrite(kanan1,HIGH);
digitalWrite(kanan2,LOW);
delay(500);
digitalWrite(kiri1,HIGH);
digitalWrite(kiri2,LOW);
digitalWrite(kanan1,HIGH);
digitalWrite(kanan2,LOW);
}

void turnright(){
digitalWrite(kiri1,HIGH);
digitalWrite(kiri2,LOW);
digitalWrite(kanan1,LOW);
digitalWrite(kanan2,HIGH);
delay(500);
digitalWrite(kiri1,HIGH);
digitalWrite(kiri2,LOW);
digitalWrite(kanan1,HIGH);
digitalWrite(kanan2,LOW);
}
void turnaround(){

digitalWrite(kiri1,HIGH);
digitalWrite(kiri2,LOW);
digitalWrite(kanan1,LOW);
digitalWrite(kanan2,HIGH);
delay(1000);
digitalWrite(kiri1,HIGH);
digitalWrite(kiri2,LOW);
digitalWrite(kanan1,HIGH);
digitalWrite(kanan2,LOW);
}
void stopp(){
digitalWrite(kiri1,LOW);
digitalWrite(kiri2,LOW);
digitalWrite(kanan1,LOW);
digitalWrite(kanan2,LOW);
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
  choice = 'l';
  }

  else if ((rightscanval>leftscanval) && (rightscanval>centerscanval)){
  choice = 'r';
  }

  else{
  choice = 's';
  }

  return choice;

}
//====================================================
void drawLedControl() {
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 10, 60, 36);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 10, 60, 36);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 15);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Back to Main Menu", 70, 18);
  myGLCD.setFont(BigFont);
  myGLCD.print("RGB LED Control", CENTER, 50);
  myGLCD.print("LED Color:", 10, 95);
  myGLCD.print("R", 10, 135);
  myGLCD.print("G", 10, 175);
  myGLCD.print("B", 10, 215);
  myGLCD.setColor(255, 0, 0);
  myGLCD.drawLine(0,75,319,75); 
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRect(30, 138, 310, 148); // R - Slider
  myGLCD.drawRect(30, 178, 310, 188);
  myGLCD.drawRect(30, 218, 310, 228);  
}
//====================================================
//============= setLedColor() - Custom Funtion
void setLedColor() {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x=myTouch.getX();
    y=myTouch.getY();        
    // Area of the Red color slider
    if( (y>=130) && (y<=156)) {
      xR=x; // Stores the X value where the screen has been pressed in to variable xR
      if (xR<=38) { // Confines the area of the slider to be above 38 pixels
        xR=38;
      }
      if (xR>=303){ /// Confines the area of the slider to be under 310 pixels
        xR=303;
      }
    }
    // Area of the Green color slider
    if( (y>=170) && (y<=196)) {
      xG=x;
      if (xG<=38) {
        xG=38;
      }
      if (xG>=303){
        xG=303;
      }
    }
    // Area of the Blue color slider
    if( (y>=210) && (y<=236)) {
      xB=x;
      if (xB<=38) {
        xB=38;
      }
      if (xB>=303){
        xB=303;
      }
    }    
  }
  // Maps the values of the X - Axis from 38 to 0 and 310 to 255, because we need values from 0 to 255 for turning on the led
  int xRC = map(xR,38,310,0,255);
  int xGC = map(xG,38,310,0,255);
  int xBC = map(xB,38,310,0,255);
  
  // Sends PWM signal to the pins of the led
  analogWrite(redLed, xRC);
  analogWrite(greenLed, xGC);
  analogWrite(blueLed, xBC);
  
  // Draws a rectangle with the latest color combination 
  myGLCD.setColor(xRC, xGC, xBC);
  myGLCD.fillRoundRect(175, 87, 310, 119);
  
  // Draws the positioners
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(xR,139,(xR+4),147); // Positioner
  myGLCD.setColor(xRC, 0, 0);
  myGLCD.fillRect(31, 139, (xR-1), 147);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect((xR+5), 139, 309, 147);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(xG,179,(xG+4),187);
  myGLCD.setColor(0, xGC, 0);
  myGLCD.fillRect(31, 179, (xG-1), 187);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect((xG+5), 179, 309, 187);
  

  myGLCD.setColor(255, 255, 255);
  myGLCD.fillRect(xB,219,(xB+4),227);
  myGLCD.setColor(0, 0, xBC);
  myGLCD.fillRect(31, 219, (xB-1), 227);
  myGLCD.setColor(0, 0, 0);
  myGLCD.fillRect((xB+5), 219, 309, 227);
}

void androidControl(){
    if(bluetooth.available()) {
 
    char c = (char)bluetooth.read();
    
    cmd[cmdIndex] = c;  
    if(cmdIndex<99) cmdIndex++;
      
    if(c=='\n') {   // each command ends with '\n'
      exeCmd();
      cmdIndex = 0;
    }  
  
  }
}

//=============NEXT PAGE
void drawHomeScreen2() {
 // Title
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("ARDUINO SMART ROBOT", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0,32,479,32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("by HowToMechatronics.com", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("MAIN MENU", CENTER, 64);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Developed by Arief Setyo Nugroho", CENTER, 300);
  
  // Button - Distance Sensor
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (90, 90, 390, 130); // Draws filled rounded rectangle (LEBAR KIRI,TINGGI BAWAH,LEBAR KANAN,BAWAH)
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (90, 90, 390, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("SUB MENU 1", CENTER, 102); // Prints the string
  
  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (90, 140, 390, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (90, 140, 390, 180);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("SUB MENU 2", CENTER, 152);

  // Button - Android
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (90, 190, 390, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (90, 190, 390, 230);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("SUB MENU 3", CENTER, 202);

    // Button - Previous
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (90, 250, 210, 290);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (90, 250, 210, 290);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("PREV", (90,210,195,120), 262);

      // Button - Previous
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (390, 250, 270, 290);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (390, 250, 270, 290);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("NEXT", (10,210,195,297), 262);


}