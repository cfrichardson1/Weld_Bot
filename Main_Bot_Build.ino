#include <Stepper.h>
#include <LiquidCrystal.h>

// Stepper
const int loops_2_complete_cycle = 500; // loops needed to complete one cycle/loop
// ###################################### CHANGE THIS ############################################
const int reset_steps = 1000
const int stepsPerRevolution = 13000; // STEP VALUE FOR GEAR!!!
// ###############################################################################################
int steps = 0; // holds how many steps per loop dependending on above values (stepsPerRevolution/loops_2_complete_rev)
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
const int STOP_N_RESET_PIN = 0;  // Stops and resets current run

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
    delay(delay_value*1000);
    myStepper.setSpeed(speed_value*12);

for(int loop_count =1 ; loop_count <= 4; loop_count++){
  myStepper.step(-step_counter);
}
// Steps divided by loops_2_complete will  be  how many times this loop runs
    for(int loop_count = 1; loop_count <= loops_2_complete_rev; loop_count++){ // microstep revolution loop
      // Run stepper motor
      steps = (stepsPerRevolution/loops_2_complete_rev);
      myStepper.step(steps);
      step_counter += steps;

      if(digitalRead(PAUSE_BUTTON_PIN) == 0){ // PAUSE FN

        while(digitalRead(ON_BUTTON_PIN) != 0){
          delay(500);
          if(digitalRead(STOP_N_RESET_PIN) == 0){ // set STOP N RESET TO ON
            stop_n_reset = 1;
            break;
          }
        }
      }
      else if(digitalRead(STOP_N_RESET_PIN) == 0){ // set STOP N RESET TO ON
        stop_n_reset = 1;
      }

      if(stop_n_reset == 1) { // checks if STOP N RESET is ON
        break;
      }

    } // EO microstep loop

    // Increase speed for return to 0 degrees & reset values
    myStepper.setSpeed(180);
    myStepper.step(-step_counter);
    step_counter = 0;  // reset
    stop_n_reset = 0; // reset
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
