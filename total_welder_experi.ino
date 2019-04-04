#include <Stepper.h>
#include <LiquidCrystal.h>

// Stepper
const int loops_2_complete_rev = 100; // loops needed to complete one revoultion
const int stepsPerRevolution = 13000; // microstep value
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
int pause_time = 7200; // pause time 2 hour pause time *note 60 == 1 minute
int self_destruct = 0; // 1 will allow the weldbot to stop & reset to 0 degrees

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
const int POTENTIOMETER_PIN = A1;
const int PAUSE_BUTTON_PIN = 1;
const int SELF_DESTRUCT_PIN = 0;  // Stops and resets current run

void loop() {
  lcd_key = read_LCD_buttons(); // read the buttons
  self_destruct = 0;

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
    delay(delay_value*1000);
    myStepper.setSpeed(speed_value*12);

    for(int loop_count = 1; loop_count <= loops_2_complete_rev; loop_count++){ // microstep revolution loop
      // Run stepper motor
      myStepper.step(stepsPerRevolution/loops_2_complete_rev);
      if(digitalRead(PAUSE_BUTTON_PIN) == 0){ // PAUSE FN
        // -----PAUSE LOOP MATH-----
        // (pause_time*10 seconds) / seconds per minute = total pause time
        // (pause_time*10) / 60 = x minutes

        for(int timer_count = 1; timer_count <= pause_time; timer_count++){
          if(digitalRead(ON_BUTTON_PIN) == 0) {
            break; // break out of loop
          }
          else if(digitalRead(SELF_DESTRUCT_PIN) == 0) {
            self_destruct = 1; // sets self_destruct to ON! Wah hahahah =)
            break; // break out of loop
          }
          delay(1000);
        }

      }

      if(self_destruct == 1) { // checks to see if user stopped the run
        self_destruct = 0; // sets self_destruct to OFF!
        break;
      }
    }
    // Increase speed for return to 0
    myStepper.setSpeed(180);
    myStepper.step(-stepsPerRevolution);
  }
}

void setup() {
  // Initialize Pins
  pinMode(ON_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PAUSE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELF_DESTRUCT_PIN, INPUT_PULLUP);

  lcd.begin(16, 2); // start the library

  // Speed
  lcd.setCursor(0,0);
  lcd.print("Set Speed:");

  //Delay
  lcd.setCursor(0,1);
  lcd.print("Set Delay:");
}
