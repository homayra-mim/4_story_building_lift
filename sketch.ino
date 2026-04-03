#include <AccelStepper.h>
#include <LiquidCrystal_I2C.h>

#define dirPin 2
#define stepPin 3
#define enablePin 4

int currentFloor = 1;

AccelStepper stepper(1, stepPin, dirPin);
LiquidCrystal_I2C lcd(0x27,16,2);

// Buttons
const int buttonPins[4] = {6, 7, 8, 9};

// LEDs
const int ledPins[4] = {10, 11, 12, 13};

// Floor positions (adjust if needed)
int floorSteps[4] = {0, 20, 40, 60};

void setup() {
  lcd.begin(16,2);
  lcd.backlight();
  Serial.begin(9600);

  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW);

  // Setup buttons & LEDs
  for(int i=0; i<4; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }

  stepper.setMaxSpeed(1000);

  updateDisplay();
  updateLEDs();
}

void loop() {
  for(int i=0; i<4; i++){
    if(digitalRead(buttonPins[i]) == LOW && currentFloor != (i+1)){
      moveToFloor(i+1);
    }
  }
}

// Function to move lift
void moveToFloor(int targetFloor){
  stepper.moveTo(floorSteps[targetFloor-1]);

  while(stepper.distanceToGo() != 0){
    stepper.run();
  }

  currentFloor = targetFloor;

  updateDisplay();
  updateLEDs();
}

// Update LCD
void updateDisplay(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Floor: ");
  lcd.print(currentFloor);
}

// Update LEDs
void updateLEDs(){
  for(int i=0; i<4; i++){
    if(i == currentFloor-1){
      digitalWrite(ledPins[i], HIGH);
    } else {
      digitalWrite(ledPins[i], LOW);
    }
  }
}