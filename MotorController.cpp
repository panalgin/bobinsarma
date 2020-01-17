#include "MotorController.h"

MotorController::MotorController() {

}

void MotorController::Initialize() {
  this->UseRamp = true;
}

void MotorController::Move(float steps, Motor& motor) {
  float absolute = motor.GetCurrentPosition() + steps;
  this->MoveTo(absolute, motor);
}

void MotorController::MoveTo(float absolute, Motor& motor) {
  float curPos = motor.GetCurrentPosition();

  if (curPos != absolute) {
    float delta = abs(curPos - absolute);
    motor.PrepareTo(absolute);

    float accelIndex = 0;

    Motor * mPtr = &motor;

    while (motor.StepsRemaining >= 1) {
      if (motor.UseRamping)
        this->CalculateRamp(delta, accelIndex, mPtr);

      motor.Step();
      accelIndex++;
    }

    motor.SetSpeed(motor.MaxSpeed);
  }
}

void MotorController::LinearMove(float steps1, float steps2, Motor *first, Motor *second, float windingCount) {
  float pos1 = first->GetCurrentPosition() + steps1;
  float pos2 = second->GetCurrentPosition() + steps2;

  this->LinearMoveTo(pos1, pos2, first, second, windingCount);
}

void MotorController::LinearMoveTo(float pos1, float pos2, Motor *first, Motor *second, float windingCount) {
  first->PrepareTo(pos1);
  second->PrepareTo(pos2);

  float m_DeltaX = first->StepsRemaining;
  float m_DeltaY = second->StepsRemaining;

  float m_Index;
  float m_Over = 0;
  unsigned long l_Index = 0;
  unsigned long l_Count = (unsigned long)windingCount;

  if (m_DeltaX > m_DeltaY) {
    l_Index = 0;
    for (m_Index = 0; m_Index < m_DeltaX; ++m_Index) {
      if (first->UseRamping)
        this->CalculateRamp(m_DeltaX, m_Index, first);

      first->Step();

      m_Over += m_DeltaY;

      if (m_Over >= m_DeltaX) {
        m_Over -= m_DeltaX;
        second->Step();

        l_Index++;

        if (l_Index >= l_Count) {
          if (l_Index % l_Count == 0) {
            second->ToggleDirection();
          }
        }
      }
    }

    first->SetSpeed(first->MaxSpeed);
  }
  else {
    l_Index = 0;
    for (m_Index = 0; m_Index < m_DeltaY; ++m_Index) {
      if (second->UseRamping)
        this->CalculateRamp(m_DeltaY, m_Index, second);

      second->Step();

      m_Over += m_DeltaX;

      if (m_Over >= m_DeltaY) {
        m_Over -= m_DeltaY;
        first->Step();
        
        l_Index++;

        if (l_Index >= l_Count) {
          if (l_Index % l_Count == 0) {
            second->ToggleDirection();
            //delay(1000);
          }
        }
      }
    }

    second->SetSpeed(second->MaxSpeed);
  }
}

void MotorController::RunToEnd(int8_t _direction) {
  uint8_t x_Index = 0;

  for (uint8_t i = 0; i < MOTORS_COUNT; i++) {
    if (this->Motors[i].Axis == 'X')
      x_Index = i;
  }

  Motor & x_Motor = this->Motors[x_Index];

  x_Motor.PrepareConstant(_direction);
  Motor* x_Ptr = &x_Motor;

  uint8_t xSwitchState = LOW;

  bool x_Offsetted = false;

  unsigned long x_AccelIndex = 0;

  xSwitchState = digitalRead(x_Motor.MinPin);

  if (xSwitchState == HIGH) {
    while (x_Offsetted == false) {
      if (x_Offsetted == false) {
        if (xSwitchState == HIGH) {
          xSwitchState = digitalRead(x_Motor.MinPin);
          this->CalculateRamp(x_AccelIndex, x_Ptr);
          x_Motor.Step();
          x_AccelIndex++;
        }
        else {
          x_Offsetted = true;
          x_Motor.RampDown();
        }
      }
    }
  }

  x_Motor.Offset();
  x_Motor.SetCurrentPosition(6600);
}

void MotorController::CalculateRamp(float index, Motor* motor) {
  this->CalculateRamp(50000000, index, motor);
}

void MotorController::CalculateRamp(float delta, float index, Motor* motor) {
  //unsigned long delta = _delta;
  //unsigned long index = _index;

  if (this->UseRamp) {
    float currentRpm = 1.0;
    float accelerationEndsAt = motor->ShortDistance / 2; //(long)(m_DeltaY * 0.10);
    float decelerationStartsAt = delta - (motor->ShortDistance / 2); //(long)(m_DeltaY * 0.90);
    int maxRpm = max(1, motor->MaxSpeed);
    int minRpm = max(1, motor->RampStartsAt);

    if (delta < motor->ShortDistance) {
      if (index <= 0) { // performance reasons
        motor->SetSpeed(motor->DwellSpeed);
      }
    }
    else {
      if ((unsigned long)index % 5 == 0) {
        if (index <= accelerationEndsAt || index >= decelerationStartsAt) {
          if (index <= accelerationEndsAt) {
            currentRpm = max(1, minRpm + ((maxRpm - minRpm) * (index / accelerationEndsAt)));
            motor->SetSpeed(currentRpm);
          }
          else if (index >= decelerationStartsAt) {
            currentRpm = max(1, minRpm + ((maxRpm - minRpm) * (1.0f - (index / delta)) / (1.0f - (decelerationStartsAt / delta))));
            motor->SetSpeed(currentRpm);
          }
          else
            motor->SetSpeed(maxRpm);
        }
      }
    }
  }
}
