#ifndef MotorController_h
#define MotorController_h

#ifndef Motor_h
  #include "Motor.h"
#endif;

#define MOTORS_COUNT 2

const char Axes[] = { 'X', 'Y' };

class MotorController {
  public:
    MotorController();
    Motor Motors[2];
    bool UseRamp;
    void Initialize();
    void Delete(Motor motor);
    void Move(float steps, Motor& motor);
    void MoveTo(float pos, Motor& motor);
    void RunToEnd(int8_t _direction);
    void LinearMove(float steps1, float steps2, Motor *first, Motor *second, float windingCount);
    void LinearMoveTo(float steps1, float steps2, Motor *first, Motor *second, float windingCount);
    void CalculateRamp(float index, Motor* motor);
    void CalculateRamp(float delta, float index, Motor* motor);

};

#endif
