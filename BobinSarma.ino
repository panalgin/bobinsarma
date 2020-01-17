#include "MotorController.h"
#include "LcdTemplate.h"

#include <LiquidCrystal.h>
#include <Keypad_I2C.h>
#include <Keypad.h>
#include <Wire.h>

#define I2CADDR 0x27

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3' },
  {'4', '5', '6' },
  {'7', '8', '9' },
  {'*', '0', '#' }
};

byte colPins[COLS] = { 5, 6, 7 }; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = { 0, 1, 2, 4 }; //connect to the row pinouts of the keypad


Keypad_I2C keypad = Keypad_I2C( makeKeymap(keys), rowPins, colPins, ROWS, COLS, I2CADDR );
LiquidCrystal lcd(5, 6, 7, 8, 9, 10);

LcdTemplate m_Main(0);

MotorController m_Controller;
Motor x_Motor(3, 13, 'X');
Motor y_Motor(11, 12, 'Y');

void setup() {
  Serial.begin(9600);

  lcd.begin(16, 2);

  m_Main.Type = 0;
  m_Main.Deter();
  m_Main.Print(lcd);

  //keypad burda başlar
  keypad.begin( );
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad

  x_Motor.IsInverted = false;
  x_Motor.MinPin = 4;
  x_Motor.Initialize();
  m_Controller.Initialize();
  x_Motor.SetMaxSpeed(600);
  x_Motor.SetDwellSpeed(50);
  x_Motor.ShortDistance = x_Motor.BaseMetricInSteps * 150.0f;
  x_Motor.RampStartsAt = x_Motor.MaxSpeed * 0.05f;
  x_Motor.SetSpeed(600);
  m_Controller.Motors[0] = x_Motor;

  y_Motor.Initialize();
  y_Motor.MinPin = 1;
  y_Motor.IsInverted = false;
  y_Motor.StepsPerRevolution = 20;
  y_Motor.MicroStepRatio = 16;
  y_Motor.BaseMetricInSteps = 106.66666f;
  y_Motor.SetMaxSpeed(5000);
  y_Motor.SetDwellSpeed(5000);
  y_Motor.ShortDistance = y_Motor.BaseMetricInSteps * 300.0f;
  y_Motor.RampStartsAt = y_Motor.MaxSpeed * 0.05f;
  y_Motor.SetSpeed(5000);
  m_Controller.Motors[1] = y_Motor;

  delay(2000);

  m_Main.Type = 2;
  m_Main.Deter();
  m_Main.Print(lcd);

  Motor * x_Ptr = &x_Motor;
  Motor * y_Ptr = &y_Motor;
}

volatile unsigned long currentRev, oldRev;
volatile bool hasWrittenToLcd = true;
volatile char text[17];
volatile uint8_t textIndex = 0;

void keypadEvent(KeypadEvent key) {
  switch (keypad.getState()) {
    case RELEASED:
      m_Main.HookKey(key, lcd);
      break;
  }
}

void loop() {
  char key = keypad.getKey();

  if (m_Main.Type == 6) {
    if (m_Main.HasFiredMotor == false) {
      m_Main.HasFiredMotor = true;

      if (m_Main.Job.Revolutions > 0 /*&& m_Main.Job.Length > 0.00f && m_Main.Job.Diameter > 0.00f*/) {
        float x_Destination = m_Main.Job.Revolutions * 800;
        /*uint16_t windingCountInL = (uint16_t)(m_Main.Job.Length / m_Main.Job.Diameter);
        float turningDistance = (windingCountInL * m_Main.Job.Diameter) * y_Motor.BaseMetricInSteps;
        float y_Destination = (m_Main.Job.Diameter * m_Main.Job.Revolutions) * y_Motor.BaseMetricInSteps;*/

        x_Motor.SetCurrentPosition(0.0f);
        //m_Controller.LinearMove(x_Destination, y_Destination, &x_Motor, &y_Motor, turningDistance);
        m_Controller.Move(x_Destination, x_Motor);
      }

      m_Main.Type = 2;
      m_Main.Deter();
      m_Main.Print(lcd);
    }
  }
}

