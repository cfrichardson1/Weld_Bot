#include <Stepper.h>
#include <LiquidCrystal.h>
// Pins
const int ON_BUTTON_PIN = 11;
const int PAUSE_BUTTON_PIN = 1;
const int POTENTIOMETER_PIN = A1;

// Stepper
const int steps  = 10; // microstep value
const int loops_2_complete_rev = 1300; // loops needed to complete one revoultion of 13k steps
Stepper myStepper(steps, 2, 3);

// Default Values for
float delay_value = 0.0;  // delay time before start
float speed_value = 10.0; // speed value for rotation
int pause_time = 0; // default pause time

// LCD
LiquidCrystal lcd(8,9,4,5,6,7);
// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// Reads the LCD button input
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


  if (digitalRead(ON_BUTTON_PIN) == 0) { //ON FN
    delay(delay_value*1000); // set delay start value
    myStepper.setSpeed(speed_value*12); // set motor speed

    // run loop 1300 times to complete full revolution
    for(int loop_count = 0; loop_count <= loops_2_complete_rev; loop_count++){ // microstep revolution loop
      // Run stepper motor @ 10 steps
      myStepper.step(steps);

      if(digitalRead(PAUSE_BUTTON_PIN) == 0){ // PAUSE FN
        pause_time = 6; // int value represents 1 second

        // -----PAUSE LOOP MATH-----
        // (pause_time*10 seconds) / seconds per minute = total pause time
        // (pause_time*10) / 60 = x minutes

        // runs 10 steps per loop and checks for PAUSE_BUTTON activation
        for(int pause_loop = 0; pause_loop <= pause_time; pause_loop++){ // CHECK FN
          delay(10000); // pause for 10 seconds
          if (digitalRead(PAUSE_BUTTON_PIN) == 0) {
            break;
          }
        } // ef CHECK FN
      } // ef PAUSE FN
    } // ef microstep revolution loop

    myStepper.setSpeed(180);  // Increase speed for return to 0
    myStepper.step(-steps*1300);  // returns motor back to start position
  } // ef ON FN
}// ef VOID LOOP


void setup() {
  // Initialize Pins
  pinMode(ON_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PAUSE_BUTTON_PIN, INPUT_PULLUP);


  lcd.begin(16, 2); // start the library

  // Speed
  lcd.setCursor(0,0);
  lcd.print("Set Speed:");

  //Delay
  lcd.setCursor(0,1);
  lcd.print("Set Delay:");
}
