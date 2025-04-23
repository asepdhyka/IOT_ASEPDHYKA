#include <Arduino.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   
Servo servoIn;
Servo servoOut;

int IRIn = 2;
int IROut = 4;
int maxSlots = 4;         // Total parking
int availableSlots = maxSlots;
bool flag1 = false;
bool flag2 = false;
bool displayUpdated = false;

void setup() {
  Serial.begin(9600); 
  lcd.init();            
  lcd.backlight();       

  pinMode(IRIn, INPUT);
  pinMode(IROut, INPUT);
  
  servoIn.attach(3);
  servoOut.attach(5);
  servoIn.write(90);       
  servoOut.write(90);      

  lcd.setCursor(0, 0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor(0, 1);
  lcd.print(" PARKING SYSTEM ");
  delay(2000);
  lcd.clear();  
}

void loop() {
  // Mobil masuk ke tempat parkir
  if (digitalRead(IRIn) == LOW && !flag1 && availableSlots > 0) {
    flag1 = true;                
    availableSlots -= 1;         // Mengurangi Slot
    servoIn.write(0);            // Open 
    delay(1000);                 
    servoIn.write(90);           // Close
    displayUpdated = false;      
  }

  // Mobil keluar tempat parkir
  if (digitalRead(IROut) == LOW && !flag2 && availableSlots < maxSlots) {
    flag2 = true;                
    availableSlots += 1;         // Increase available slot
    servoOut.write(180);         // Open 
    delay(1000);                 
    servoOut.write(90);          // Close 
    displayUpdated = false;      
  }

  // Reset Flag
  if (flag1 && digitalRead(IRIn) == HIGH) {
    flag1 = false;
  }
  if (flag2 && digitalRead(IROut) == HIGH) {
    flag2 = false;
  }

  // Update LCD display kalau ada yang berubah
  if (!displayUpdated) {
    lcd.clear();
    if (availableSlots == 0) {    // Parking full
      lcd.setCursor(0, 0);
      lcd.print("    MAAF :(    ");  
      lcd.setCursor(0, 1);
      lcd.print("  Parking Full  ");
    } else {                      // Parking available
      lcd.setCursor(0, 0);
      lcd.print("    WELCOME!    ");
      lcd.setCursor(0, 1);
      lcd.print("Slot Left: ");
      lcd.print(availableSlots);
    }
    displayUpdated = true;       
  }

  delay(100); // Small delay
}
