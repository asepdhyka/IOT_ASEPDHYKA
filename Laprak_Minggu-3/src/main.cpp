#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// These are for the temperature
#define DHTPIN 21
#define DHTTYPE DHT22
#define TIMEDHT 1000

// Pin for LDR sensor
#define LDRPIN 34  // Pin analog untuk LDR (ubah sesuai koneksi)

float humidity, celsius, fahrenheit, ldrValue;
uint32_t timerDHT = TIMEDHT;
DHT dht(DHTPIN,DHTTYPE);

// LCD I2C address 0x27 with a 16x2 display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Code for getting Temperature and Humidity
void getTemperature()
{
  // Wait for a time between measurements
  if ((millis() - timerDHT) > TIMEDHT) {
    // Update the timer
    timerDHT = millis();
    
    // Reading temperature or humidity takes about 250 milliseconds!
    humidity = dht.readHumidity();
    celsius = dht.readTemperature();
    fahrenheit = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again)
    if (isnan(humidity) || isnan(celsius) || isnan(fahrenheit)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
  }
}

// Code for getting LDR value
void getLDR()
{
  ldrValue = analogRead(LDRPIN);  // Read the LDR value
}

void setup() {
  // Setup Serial
  Serial.begin(115200);
  Serial.println("Hello, Asep Dhyka!");

  // Setup DHT sensor
  dht.begin();

  // Setup LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Init sensors...");
  delay(1000);
}

void loop() {
  // Get temperature and humidity
  getTemperature();

  // Get LDR value
  getLDR();

  // Print values to Serial Monitor
  Serial.print("Temp (C): ");
  Serial.println(celsius);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Display values on LCD
  lcd.clear();
  lcd.setCursor(0, 0);  // First row
  lcd.print("T:");
  lcd.print(celsius);
  lcd.print("C H:");
  lcd.print(humidity);
  lcd.print("%");

  lcd.setCursor(0, 1);  // Second row
  lcd.print("LDR:");
  lcd.print(ldrValue);

  // Delay before next reading
  delay(1000);
}
