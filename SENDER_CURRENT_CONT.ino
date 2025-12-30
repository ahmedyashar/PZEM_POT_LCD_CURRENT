#include <PZEM004Tv30.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define the UART2 RX and TX pins on ESP32
#define PZEM_RX_PIN 16
#define PZEM_TX_PIN 17

// Initialize the PZEM sensor
PZEM004Tv30 pzem(&Serial2);

// Initialize the LCD (Address 0x27 is common, 16 columns, 2 rows)
// If you have a 20x4 LCD, change to (0x27, 20, 4)
LiquidCrystal_I2C lcd(0x27, 16, 2);


const int potPin = 36;
float potValue = 0;

void setup() {
  Serial.begin(115200);

  // Initialize PZEM Serial
  Serial2.begin(9600, SERIAL_8N1, PZEM_RX_PIN, PZEM_TX_PIN);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("PZEM-004T V3.0");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Read data from the PZEM sensor
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();

  // Clear and Update LCD
  if (isnan(voltage) || isnan(current)) {
    Serial.println("Error reading sensor");
    lcd.setCursor(0, 0);
    lcd.print("Sensor Error    ");
    lcd.setCursor(0, 1);
    lcd.print("Check Wiring    ");
  } else {
    // Output to Serial for debugging
    Serial.printf("V: %.1fV | A: %.2fA | W: %.1fW\n", voltage, current, power);

    // Display on LCD Row 1: Voltage
    lcd.setCursor(0, 0);
    lcd.print("Volt: ");
    lcd.print(voltage, 1);
    lcd.print(" V      ");  // Extra spaces to clear old digits

    // Display on LCD Row 2: Current
    lcd.setCursor(0, 1);
    lcd.print("Curr: ");
    lcd.print(current, 2);
    lcd.print(" A      ");
  }
  potValue = analogRead(potPin);
  Serial.println((potValue/4095)*10,2);
  delay(2000);
}