#include "LcdTemplate.h"
#include <EEPROM.h>

LcdTemplate::LcdTemplate() {

}

void LcdTemplate::Deter() {
  switch (this->Type) {
    case 0:
      {
        sprintf(Data[0], "%s", " KARA ELKTRONIK ");
        sprintf(Data[1], "%s", "  BOBIN SARICI  ");

        break;
      }
    case 2:
      {
        sprintf(Data[0], "%s", " -MANUEL SARIM- ");
        sprintf(Data[1], "%s", "TUR:            ");

        break;
      }
    case 3:
      {
        sprintf(Data[0], "%s", " -MANUEL SARIM- ");
        sprintf(Data[1], "%s", "L: 00.00mm      ");

        break;
      }
    case 4:
      {
        sprintf(Data[0], "%s", " -MANUEL SARIM- ");
        sprintf(Data[1], "%s", "D: 00.00mm      ");

        break;
      }

    case 6:
      {
        sprintf(Data[0], "%s", "SARIM ...       ");
        sprintf(Data[1], "%s", "0/0             ");

        break;
      }
  }
}

LcdTemplate::LcdTemplate(short type) {
  this->Type = type;
  this->Deter();
}

short numberCol = 0;

void LcdTemplate::Print(LiquidCrystal &lcd) {
  this->IsVisible = true;

  lcd.clear();

  for (short i = 0; i < 2; i++) {
    lcd.setCursor(0, i);

    for (short j = 0; j < 16; j++) {
      char cur = Data[i][j];

      if (cur > 0) {
        lcd.write(Data[i][j]);
        delay(3);
      }

      delay(1);
    }
  }

  switch (this->Type) {
    case 2:
      { // Manuel Ekran
        this->Out[0] = '\0';
        this->Out[1] = '\0';
        this->Out[2] = '\0';
        this->Out[3] = '\0';
        this->Out[4] = '\0';
        this->Out[5] = '\0';

        numberCol = 0;
        this->updateCursor(lcd);

        break;
      }
    case 3: { //L1 Gir
        this->Out[0] = '0';
        this->Out[1] = '0';
        this->Out[2] = '0';
        this->Out[3] = '0';
        this->Out[4] = '0';
        this->Out[5] = '0';

        numberCol = 0;
        this->updateCursor(lcd);

        break;
      }

    case 4: { //D1 Gir
        this->Out[0] = '0';
        this->Out[1] = '0';
        this->Out[2] = '0';
        this->Out[3] = '0';
        this->Out[4] = '0';
        this->Out[5] = '0';

        numberCol = 0;
        this->updateCursor(lcd);

        break;
      }
  }
}

void LcdTemplate::printNumber(char key, LiquidCrystal &lcd) {
  switch (this->Type) {
    case 2:
      { // Manuel Ekran
        lcd.write(key);
        this->Out[numberCol] = key;
        numberCol++;

        this->updateCursor(lcd);

        break;
      }

    case 3: { //L1 Gir
        lcd.write(key);
        this->Out[numberCol] = key;
        numberCol++;

        this->updateCursor(lcd);

        break;
      }

    case 4: { //D1 Gir
        lcd.write(key);
        this->Out[numberCol] = key;
        numberCol++;

        this->updateCursor(lcd);

        break;
      }
  }
}

void LcdTemplate::updateCursor(LiquidCrystal &lcd) {
  switch (this->Type) {
    case 2:
      { // Manuel Ekran
        if (numberCol > 5 || numberCol < 0)
          numberCol = 0;

        lcd.setCursor(5 + numberCol, 1);
        lcd.blink();

        break;
      }

    case 3: {  //L1 Gir
        if (numberCol > 3 || numberCol < 0)
          numberCol = 0;

        lcd.setCursor(3 + (numberCol > 1 ? numberCol + 1 : numberCol), 1);
        lcd.blink();

        break;
      }

    case 4: { //D1 Gir
        if (numberCol > 3 || numberCol < 0)
          numberCol = 0;

        lcd.setCursor(3 + (numberCol > 1 ? numberCol + 1 : numberCol), 1);
        lcd.blink();

        break;
      }
  }
}

void LcdTemplate::deleteNumber(LiquidCrystal &lcd) {
  switch (this->Type) {
    case 2:
      { // Manuel Ekran
        lcd.write('0');
        this->Out[numberCol] = '0';

        numberCol--;
        this->updateCursor(lcd);

        break;
      }

    case 3:
      { //L1 Gir
        lcd.write('0');
        this->Out[numberCol] = '0';

        numberCol--;
        this->updateCursor(lcd);

        break;
      }

    case 4:
      { //D1 Gir
        lcd.write('0');
        this->Out[numberCol] = '0';

        numberCol--;
        this->updateCursor(lcd);

        break;
      }
  }
}

void LcdTemplate::HookKey(char key, LiquidCrystal &lcd) {
  if (this->IsVisible) {
    lcd.noBlink();
    lcd.noCursor();

    switch (this->Type) {
      case 2: { // MANUEL EKRAN
          this->HasFiredMotor = false;
          this->updateCursor(lcd);

          if (key != '*' && key != '#') {
            this->printNumber(key, lcd);
          }

          else if (key == '*') {
            this->deleteNumber(lcd);
          }

          else if (key == '#') {
            this->Job.Revolutions = atol(this->Out);
            this->Type = 6;
            this->Deter();

            this->Print(lcd);
          }

          break;
        }

      case 3: { //L1 Gir
          this->updateCursor(lcd);

          if (key != '*' && key != '#') {
            this->printNumber(key, lcd);
          }

          else if (key == '*') {
            this->deleteNumber(lcd);
          }

          else if (key == '#') {
            this->Job.Length = atof(this->Out) / 100.00f;
            this->Type = 4;
            this->Deter();

            this->Print(lcd);
          }

          break;
        }

      case 4: {
          this->updateCursor(lcd);

          if (key != '*' && key != '#') {
            this->printNumber(key, lcd);
          }

          else if (key == '*') {
            this->deleteNumber(lcd);
          }

          else if (key == '#') {
            this->Job.Diameter = atof(this->Out) / 100.00f;
            this->Type = 6;
            this->Deter();

            this->Print(lcd);
          }

          break;
        }
    }
  }
}



