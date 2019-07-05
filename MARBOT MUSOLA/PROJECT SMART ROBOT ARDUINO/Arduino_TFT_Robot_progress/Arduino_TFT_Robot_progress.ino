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
int relay1 = A1;
int relay2 = A2;
int relay3 = A3;
int relay4 = A4;
int bluetoothTx = 15;
int bluetoothRx = 14;

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

int kiri1 = 8;
int kiri2 = 9;
int kanan1 = 10;
int kanan2 = 11;
char cmd[100];
int cmdIndex;



//==== Creating Objects
UTFT    myGLCD(ITDB32S,38,39,40,41); //Parameters should be adjusted to your Display/Schield model
URTouch  myTouch( 6, 5, 4, 3, 2);

//==== Defining Variables
extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

extern unsigned int bird01[0x41A];

int x, y;

char currentPage, selectedUnit;

//Ultrasonic Sensor
const int trigPin = 12;
const int echoPin = 13;

long duration;
int distanceInch, distanceCm;



// Control Android

void exeCmd() {
  
  if(cmd[0]=='f') {  // maju
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, HIGH); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, HIGH);
    maju(); 
  } 
  
  if(cmd[0]=='b') {  // mundur
    digitalWrite(kiri1, HIGH);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, HIGH);  
    digitalWrite(kanan2, LOW);
    mundur(); 
  } 
   
  if(cmd[0]=='l') { // kiri
    digitalWrite(kiri1, HIGH);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, HIGH);
    kiri();
  } 
  
  if(cmd[0]=='r') {  // kanan
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, HIGH); 
    digitalWrite(kanan1, HIGH);  
    digitalWrite(kanan2, LOW);
    kanan();
  }  
}

void setup() {
// Initial setup
  myGLCD.InitLCD();
  myGLCD.clrScr();
  myTouch.InitTouch();
  myTouch.setPrecision(PREC_MEDIUM);

  // Defining Pin Modes
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input


  drawHomeScreen();  // Draws the Home Screen
  currentPage = '0'; // Indicates that we are at Home Screen
  selectedUnit = '0'; // Indicates the selected unit for the first example, cms or inches

// Control Android
  pinMode(kiri1, OUTPUT);
  pinMode(kiri2, OUTPUT);
  pinMode(kanan1,OUTPUT);
  pinMode(kanan2, OUTPUT);

   bluetooth.begin(115200); // Bluetooth default baud is 115200
  
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$"); // enter cmd mode
  delay(250);  
  bluetooth.println("U,9600,N"); // change baud to 9600
  
  bluetooth.begin(9600);
  
  
  cmdIndex = 0;

  //==Line Follower
  pinMode(A5,INPUT);
  pinMode(A6,INPUT);

  //RELAY
     //Setup all the Arduino Pins
   pinMode(relay1, OUTPUT);
   pinMode(relay2, OUTPUT);
   pinMode(relay3, OUTPUT);
   pinMode(relay4, OUTPUT);
   
   
   //Turn OFF any power to the Relay channels
   digitalWrite(relay1,HIGH);
   digitalWrite(relay2,HIGH);
   digitalWrite(relay3,HIGH);
   digitalWrite(relay4,HIGH);
   delay(2000); //Wait 2 seconds before starting sequence
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
        drawLineFollower();
      }  
      // If we press the Android Control
      if ((x>=35) && (x<=285) && (y>=190) && (y<=230)) {
        drawFrame(35, 190, 285, 230);
        currentPage = '3';
        myGLCD.clrScr();
        drawAndroidControl();
      }

      //If we press the prev
      if ((x>=10) && (x<=58) && (y>=60) && (y<=82)) {
        drawFrame(10, 60, 58, 82);
        currentPage = '4';
        myGLCD.clrScr();
        drawHomeScreen2();
      }  
    }
  }
  // Robot Avoiding Example
  if (currentPage == '1') {    
      getDistance(); // Gets distance from the sensor and this function is repeatedly called while we are at the first example in order to print the lasest results from the distance sensor
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
          digitalWrite(relay1, HIGH); //relay off
          drawHomeScreen(); // Draws the home screen
        }
      }
  }
  
  // LINE FOLLOWER 
  if (currentPage == '2') {
    digitalWrite(relay2, LOW);
    setLineFollower();
    if (myTouch.dataAvailable()) {
        myTouch.read();
        x=myTouch.getX();
        y=myTouch.getY();
        
        //Back button
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0';
          myGLCD.clrScr();
          digitalWrite(relay2, HIGH); // relay off
          drawHomeScreen();
        }
    }
  }
//==== This section of the code, for the game example, is explained in my next tutorial
  // Android Control
  if (currentPage == '3') {
    androidControl();
    if (myTouch.dataAvailable()) {
      myTouch.read();
      x=myTouch.getX();
      y=myTouch.getY();
    
        if ((x>=10) && (x<=60) &&(y>=10) && (y<=36)) {
          drawFrame(10, 10, 60, 36);
          currentPage = '0'; // Indicates we are at home screen
          myGLCD.clrScr();
          drawHomeScreen(); // Draws the home screen
        }
    }
}
//==== HomeScreen2
if (currentPage == '4') {
  if (myTouch.dataAvailable()) {
    myTouch.read();
    x=myTouch.getX();
    y=myTouch.getY();
      // If we press the Distance Sensor Button 
      if ((x>=35) && (x<=285) && (y>=90) && (y<=130)) {
        drawFrame(35, 90, 285, 130); // Custom Function -Highlighs the buttons when it's pressed
        currentPage = '5'; // Indicates that we are the first example
        myGLCD.clrScr(); // Clears the screen
        drawDistanceSensor(); // It is called only once, because in the next iteration of the loop, this above if statement will be false so this funtion won't be called. This function will draw the graphics of the first example.
      }
      // If we press the RGB LED Control Button 
      if ((x>=35) && (x<=285) && (y>=140) && (y<=180)) {
        drawFrame(35, 140, 285, 180);
        currentPage = '6';
        myGLCD.clrScr();
        drawLedControl();
      }  
      // If we press the Android Control
      if ((x>=35) && (x<=285) && (y>=190) && (y<=230)) {
        drawFrame(35, 190, 285, 230);
        currentPage = '7';
        myGLCD.clrScr();
        drawAndroidControl();
      }
      // If we press the Next
      if ((x>=258) && (x<=58) && (y>=308) && (y<=82)) {
        drawFrame(258, 308, 58, 82);
        currentPage = '4';
        myGLCD.clrScr();
        drawHomeScreen2();
      }
      //If we press the prev
      if ((x>=10) && (x<=58) && (y>=60) && (y<=82)) {
        drawFrame(10, 60, 58, 82);
        currentPage = '0';
        myGLCD.clrScr();
        drawHomeScreen();
      }
    
  }
}
  // Robot Avoiding Example
  if (currentPage == '5') {    
      getDistance(); // Gets distance from the sensor and this function is repeatedly called while we are at the first example in order to print the lasest results from the distance sensor
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
  if (currentPage == '6') {
    getDistance();
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

        }
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
  myGLCD.drawLine(0,32,319,32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("by HowToMechatronics.com", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("Main Menu", CENTER, 64);
  
  // Button - Distance Sensor
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 90, 285, 130); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 90, 285, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("DISTANCE SENSOR", CENTER, 102); // Prints the string
  
  // Button - LINE FOLLOWER
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 140, 285, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 140, 285, 180);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("LINE FOLLOWER", CENTER, 152);

  // Button - Android
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 190, 285, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 190, 285, 230);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("ANDROID CONTROL", CENTER, 202);

  //Button - Prev
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 58, 60, 82);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 58, 60, 82);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 62);  
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
void drawHomeScreen2() {
  // Title
  myGLCD.setBackColor(0,0,0); // Sets the background color of the area where the text will be printed to black
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(BigFont); // Sets font to big
  myGLCD.print("ARDUINO SMART ROBOT", CENTER, 10); // Prints the string on the screen
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0,32,319,32); // Draws the red line
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.setFont(SmallFont); // Sets the font to small
  myGLCD.print("by HowToMechatronics.com", CENTER, 41); // Prints the string
  myGLCD.setFont(BigFont);
  myGLCD.print("Main Menu", CENTER, 64);
  
  // Button - Distance Sensor
  myGLCD.setColor(16, 167, 103); // Sets green color
  myGLCD.fillRoundRect (35, 90, 285, 130); // Draws filled rounded rectangle
  myGLCD.setColor(255, 255, 255); // Sets color to white
  myGLCD.drawRoundRect (35, 90, 285, 130); // Draws rounded rectangle without a fill, so the overall appearance of the button looks like it has a frame
  myGLCD.setFont(BigFont); // Sets the font to big
  myGLCD.setBackColor(16, 167, 103); // Sets the background color of the area where the text will be printed to green, same as the button
  myGLCD.print("SUB MENU 1", CENTER, 102); // Prints the string
  
  // Button - RGB LED Control
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 140, 285, 180);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 140, 285, 180);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("SUB MENU 2", CENTER, 152);

  // Button - Android
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRoundRect (35, 190, 285, 230);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (35, 190, 285, 230);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.print("SUB MENU 3", CENTER, 202);

  //Button - Prev
  myGLCD.setColor(100, 155, 203);
  myGLCD.fillRoundRect (10, 58, 60, 82);
  myGLCD.setColor(255, 255, 255);
  myGLCD.drawRoundRect (10, 58, 60, 82);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(100, 155, 203);
  myGLCD.print("<-", 18, 62);
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
  myGLCD.print("Developed by Arief Setyo Nugroho", CENTER, 220);  
}
//====================================================
///SETTINGAN PROGRAM
//===== getDistance - Custom Function
void getDistance() {
  //relay nyala
  digitalWrite(relay1, LOW);
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

//====================================================
void drawAndroidControl() {
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
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0,70,319,70); // Draws the red line
  myGLCD.setColor(255, 255,255);
  myGLCD.setFont(BigFont);
  myGLCD.print("ANDROID ROBOT", CENTER, 50);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(BigFont);
  myGLCD.print("ANDROID PRESS", CENTER , 110);
  myGLCD.setColor(0,255,0);
  myGLCD.fillRoundRect (35, 140, 285, 200);
  myGLCD.setColor(255,255,255);
  myGLCD.drawRoundRect(35,140,285,200);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Developed by Arief Setyo Nugroho", CENTER, 220); 
}
//bluetoothconnect
void drawbluetoothconnect() {
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  myGLCD.print("CONNECTED", CENTER, 80);
}
//maju
void maju() {
  myGLCD.setColor(255,0,0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0,255,0);
  myGLCD.print("FORWARD", CENTER, 162);
}
//mundur
void mundur() {
  myGLCD.setColor(255,0,0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0,255,0);
  myGLCD.print("BACK", CENTER, 162);
}
//belok kanan
void kanan() {
  myGLCD.setColor(255,0,0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0,255,0);
  myGLCD.print("TURN RIGHT", CENTER, 162);
}
//belok kiri
void kiri() {
  myGLCD.setColor(255,0,0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0,255,0);
  myGLCD.print("TURN LEFT", CENTER, 162);
}
//program android control
void androidControl(){
    if(bluetooth.available()) {
      drawbluetoothconnect();
 
    char c = (char)bluetooth.read();
    
    cmd[cmdIndex] = c;  
    if(cmdIndex<99) cmdIndex++;
      
    if(c=='\n') {   // each command ends with '\n'
      exeCmd();
      cmdIndex = 0;
    }  
  
  }
}

//==============================================
void drawLineFollower() {
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
  myGLCD.setColor(255, 0, 0); // Sets color to red
  myGLCD.drawLine(0,70,319,70); // Draws the red line
  myGLCD.setColor(255, 255,255);
  myGLCD.setFont(BigFont);
  myGLCD.print("LINE FOLLOWER", CENTER, 50);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(BigFont);
  myGLCD.print("READ SENSOR", CENTER , 80);
  myGLCD.setColor(0,255,0);
  myGLCD.fillRoundRect (35, 140, 285, 200);
  myGLCD.setColor(255,255,255);
  myGLCD.drawRoundRect(35,140,285,200);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.setFont(SmallFont);
  myGLCD.print("Developed by Arief Setyo Nugroho", CENTER, 220);
}
//maju
void linemaju(){
  myGLCD.setColor(255,0,0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0,255,0);
  myGLCD.print("   FORWARD   ", CENTER, 162);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(0,0,0);
  myGLCD.print("Sensor Kiri Putih & Sensor Kanan Putih", CENTER, 110);
}
//kiri
void linekiri() {
  myGLCD.setColor(255,0,0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0,255,0);
  myGLCD.print(" TURN LEFT ", CENTER, 162);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(0,0,0);
  myGLCD.print("Sensor Kiri Hitam & Sensor Kanan Putih", CENTER, 110);
}
//kanan
void linekanan() {
  myGLCD.setColor(255,0,0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0,255,0);
  myGLCD.print("  TURN RIGHT  ", CENTER, 162);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(0,0,0);
  myGLCD.print("Sensor Kiri Putih & Sensor Kanan Hitam", CENTER, 110);
}
//mundur
void linemundur() {
  myGLCD.setColor(255,0,0);
  myGLCD.setFont(BigFont);
  myGLCD.setBackColor(0,255,0);
  myGLCD.print(" !!BACK!!", CENTER, 162);
  myGLCD.setColor(255,255,255);
  myGLCD.setFont(SmallFont);
  myGLCD.setBackColor(0,0,0);
  myGLCD.print("Sensor Kiri Hitam & Sensor Kanan Hitam", CENTER, 110);
}

//Program Line Follower
void setLineFollower() {
 
  int kiri = digitalRead(A5);
  int kanan = digitalRead(A6);

  if ((kiri==HIGH)&&(kanan==LOW)) {
    linekiri();   
    digitalWrite(kiri1,HIGH);
    digitalWrite(kiri2,LOW);
    digitalWrite(kanan1,LOW);
    digitalWrite(kanan2,HIGH);
    
  }
  else if ((kiri==LOW)&&(kanan==HIGH))
  {
    linekanan();
    digitalWrite(kiri1,LOW);
    digitalWrite(kiri2,HIGH);
    digitalWrite(kanan1,HIGH);
    digitalWrite(kanan2,LOW);
  }
  else if ((kiri==HIGH)&&(kanan==HIGH)) {
    linemundur();
    digitalWrite(kiri1, HIGH);
    digitalWrite(kiri2, LOW);
    digitalWrite(kanan1, HIGH);
    digitalWrite(kanan2, LOW);
  }
  else 
  {linemaju();
    digitalWrite(kiri1, LOW);
    digitalWrite(kiri2, HIGH);
    digitalWrite(kanan1, LOW);
    digitalWrite(kanan2, HIGH);
  }


  
}

