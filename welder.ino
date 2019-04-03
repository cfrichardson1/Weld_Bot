#include <Stepper.h>
#include <LiquidCrystal.h>

// Stepper
const int stepsPerRevolution = 13000; // microstep value
Stepper myStepper(stepsPerRevolution, 2, 3);
// Speed range 0-120
int motorSpeed = 60;


// LCD
LiquidCrystal lcd(8,9,4,5,6,7);

// Pins
const int ON_BUTTON_PIN = 11;
const int POTENTIOMETER_PIN = A1;

void loop() {
  if (digitalRead(ON_BUTTON_PIN) == 0) {
    myStepper.setSpeed(motorSpeed);
    lcd.setCursor(7,0);
    lcd.print(float(float(motorSpeed)/float(12)),1);

    // Run stepper motor
    myStepper.step(stepsPerRevolution);
    myStepper.step(-stepsPerRevolution);
  }
}

void setup() {
  // Initialize Pins
  pinMode(ON_BUTTON_PIN, INPUT_PULLUP);

  // Initialize LCD
  lcd.begin(16, 2); // (columns, rows) aka size of lcd
  lcd.setCursor(1, 0); // left side of lcd
  lcd.print("Speed:");
  lcd.setCursor(12,0); // right side of lcd
  lcd.print("RPM");
}
