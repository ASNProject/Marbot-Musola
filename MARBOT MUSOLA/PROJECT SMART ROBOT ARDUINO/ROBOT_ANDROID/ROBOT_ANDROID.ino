#include <SoftwareSerial.h>
#include <Servo.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(13,12,11,10,9,8);
Servo servo1;
Servo servo2;
int bluetoothTx = 1; // TX-O pin of BT module to Arduino pin2
int bluetoothRx = 0; // RX-I pin of B module to Arduino pin3

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

int kiri1 = 7;
int kiri2 = 6;
int kanan1 = 5;
int kanan2 = 4;
char cmd[100];
int cmdIndex;


void exeCmd() {
  
  if(cmd[0]=='f') {  // maju
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, HIGH); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, HIGH); 
    servo1.write(0);
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("      MAJU");
  } 
  
  if(cmd[0]=='b') {  // mundur
    digitalWrite(kiri1, HIGH);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, HIGH);  
    digitalWrite(kanan2, LOW);
    servo1.write(0);
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     MUNDUR"); 
  } 
   
  if(cmd[0]=='l') { // kiri
    digitalWrite(kiri1, HIGH);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, HIGH);
    servo1.write(0);
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("   BELOK KIRI");
  } 
  
  if(cmd[0]=='r') {  // kanan
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, HIGH); 
    digitalWrite(kanan1, HIGH);  
    digitalWrite(kanan2, LOW);
    servo1.write(0);
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("  BELOK  KANAN");
  } 
  
  if(cmd[0]=='s') {  // stop
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);
    servo1.write(0);
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(" NGAPAK POENYA!");
    lcd.setCursor(0,1);
    lcd.print("    WAITING!");
  } 
   if(cmd[0]=='a') {  // ambil
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);
    servo1.write(0);
    servo2.write(90);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     AMBIL!");
   }
    if(cmd[0]=='p') {  // taruh
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW);
    servo1.write(0);
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     TARUH!");
    }
     if(cmd[0]=='z'){
          
       servo1.write(30); 
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW); 
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     SERVO!");     
     }
        if(cmd[0]=='x'){
          
       servo1.write(60); 
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW); 
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     SERVO!");     
     }
        if(cmd[0]=='c'){
          
       servo1.write(90); 
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW); 
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     SERVO!");     
     }
        if(cmd[0]=='v'){
          
       servo1.write(120); 
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW); 
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     SERVO!");     
     }
        if(cmd[0]=='n'){
          
       servo1.write(160); 
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW); 
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     SERVO!");     
     }
        if(cmd[0]=='m'){
          
       servo1.write(180); 
    digitalWrite(kiri1, LOW);  
    digitalWrite(kiri2, LOW); 
    digitalWrite(kanan1, LOW);  
    digitalWrite(kanan2, LOW); 
    servo2.write(0);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    ANDROID:");
    lcd.setCursor(0,1);
    lcd.print("     SERVO!");     
     }

  
  
}
void setup(){   // at power on
  
  delay(500); // wait for bluetooth module to start
  
  pinMode(kiri1, OUTPUT);
  pinMode(kiri2, OUTPUT);
  pinMode(kanan1, OUTPUT);
  pinMode(kanan2, OUTPUT);
  servo1.attach(3); // servo pada pin3
  servo2.attach(2);
  
  // change Bluetooth module baud rate to 9600:
  
  bluetooth.begin(115200); // Bluetooth default baud is 115200
  
  bluetooth.print("$");
  bluetooth.print("$");
  bluetooth.print("$"); // enter cmd mode
  delay(250);  
  bluetooth.println("U,9600,N"); // change baud to 9600
  
  bluetooth.begin(9600);
  servo1.attach(3,1000,2000);
  
  
  cmdIndex = 0;

  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print(" NGAPAK POENYA!");
  lcd.setCursor(0,1);
  lcd.print("  SET ON READY");

}

void loop() {
  
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

