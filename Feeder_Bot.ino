#include <Stepper.h>
#include <LiquidCrystal.h>

// Stepper
const int stepsPerRevolution = 13000; // STEP VALUE FOR GEAR!!!-
Stepper myStepper(stepsPerRevolution, 2, 3);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Default Values for
float delay_value = 0.0;  // delay time before start
float speed_value = 7.0; // speed value for rotation

// Place Holders
int stop_n_reset = 0; // 1 will allow the weldbot to stop & reset to 0 degrees
int step_counter = 0; // counts how many steps in revolution, allowing motor to reset back to 0

// LCD
LiquidCrystal lcd(8,9,4,5,6,7);

// Reads the button input
int read_LCD_buttons()
{
  adc_key_in = analogRead(0); // read the value from the sensor
  if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
  // For V1.1 us this threshold
  if (adc_key_in < 50)   return btnRIGHT;
  if (adc_key_in < 250)  return btnUP;
  if (adc_key_in < 350)  return btnDOWN;
  if (adc_key_in < 450)  return btnLEFT;
  if (adc_key_in < 650)  return btnSELECT;

  return btnNONE;  // when all others fail, return this...
}


// Pins
const int ON_BUTTON_PIN = 11;
const int PAUSE_BUTTON_PIN = 1;
const int RESET_PIN = 0;  // Stops and resets current run

void loop() {
  lcd_key = read_LCD_buttons(); // read the buttons


  switch (lcd_key) // depending on which button was pushed, we perform an action
  {
    // INCREASE DELAY
    case btnRIGHT:
    {
      if (delay_value <= 10.0) {
        delay_value += 0.1;

        lcd.setCursor(13,1); // move to the begining of the second line
        lcd.print(delay_value);
        delay(280);
        break;
      }
    }
    // DECREASE DELAY
    case btnLEFT:
    {

      if (delay_value >= 0.0) {
        delay_value -= 0.1;

        lcd.setCursor(13,1); // move to the begining of the second line
        lcd.print(delay_value);
        delay(280);
        break;
      }
    }
    // INCREASE SPEED
    case btnUP:
    {
      if (speed_value <= 10.0) {
        speed_value += 0.1;
        lcd.setCursor(13,0); // move cursor to second line "1" and 9 spaces over

        lcd.print(speed_value);
        delay(280);
        break;
      }
    }
    // DECREASE SPEED
    case btnDOWN:
    {
      if (speed_value >= 0.0) {
        speed_value -= 0.1;
        lcd.setCursor(13,0); // move cursor to second line "1" and 9 spaces over
        lcd.print(speed_value);
        delay(280);
        break;
      }
    }
  }

  if (digitalRead(ON_BUTTON_PIN) == 0) {
    delay(delay_value*1000); // delay start time in seconds
    myStepper.setSpeed(speed_value*12); // set motor speed

    while(digtalRead(STOP_N_RESET_PIN) != 0){
      myStepper.step(steps/loops_2_complete_loop);  // Run stepper motor, *as denom increases the responsiveness of the loop increases
    }
    break;
  }
  else if(digitalRead(RESET_PIN) == 0){
    myStepper.step(reset_steps);
  }

} // EO VOID LOOP

void setup() {
  // Initialize Pins
  pinMode(ON_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PAUSE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(STOP_N_RESET_PIN, INPUT_PULLUP);

  lcd.begin(16, 2); // start the library

  // Speed
  lcd.setCursor(0,0);
  lcd.print("Set Speed:");

  //Delay
  lcd.setCursor(0,1);
  lcd.print("Set Delay:");
}
