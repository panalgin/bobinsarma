#ifndef LcdTemplate_h
#define LcdTemplate_h

#ifndef Arduino_h
  #include "Arduino.h"
#endif

#include <LiquidCrystal.h>

typedef struct Job {
  unsigned long Revolutions;
  float Length;
  float Diameter;
} Job_t;

class LcdTemplate { 
  private:
    
    
  public:
    LcdTemplate();
    LcdTemplate(short type);
    

    Job Job;
    short Type;
    char Data[2][16];
    char LastKey;
    bool IsNewKey;
    bool IsVisible;
    bool HasFiredMotor;
    char Out[6];
    
    void Deter();
    void HookKey(char key, LiquidCrystal &lcd);
    void Print(LiquidCrystal &lcd);
    void printNumber(char key, LiquidCrystal &lcd);
    void deleteNumber(LiquidCrystal &lcd);
    void updateCursor(LiquidCrystal &lcd);
};

#endif

