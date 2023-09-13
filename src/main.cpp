#include <Arduino.h>
#include "pin.h"
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

void ENA_ON()
{
  digitalWrite(ENA_pin, LOW);
}

void ENA_OFF()
{
  digitalWrite(ENA_pin, HIGH);
}

void stepper(int axis, int direc, int kecepatan)
{
  ENA_ON();
  if (axis == 0 && reverseX)
  {
    direc = -1 * direc;
  }
  if (axis == 1 && reverseY)
  {
    direc = -1 * direc;
  }

  if (axis == 2 && reverseZ)
  {
    direc = -1 * direc;
  }

  if (direc == -1)
  {
    direc = 0;
  }

  digitalWrite(xyz_dirPin[axis], direc);
  digitalWrite(xyz_stepPin[axis], HIGH);
  delayMicroseconds(kecepatan);
  digitalWrite(xyz_stepPin[axis], LOW);
  delayMicroseconds(kecepatan);
}

boolean serialRead()
{
  if (Serial.available() > 0) // program untuk parsing data
  {
    String input = Serial.readStringUntil('\n');
    input.trim();

    int tempIndex = 0;
    for (int i = 0; i < data; i++)
    {
      int index = input.indexOf(":", tempIndex);
      arrayData[i] = input.substring(tempIndex, index).toFloat();
      tempIndex = index + 1;
    }
    input = "";

    return true;
  }
  return false;
}

String checkSerial()
{
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil('\n');
    input.trim();
    return input;
  }
  return "";
}

void z_backtohome()
{
  int jarak;
  int z;
  bool stt1 = false;
  for (int i = 0; i < 50; i++)
  {
    stepper(2, 1, z_speed_home);
  }

  for (int a = 0; a <= 4; a++)
  {
    while (digitalRead(z_home) == HIGH || !stt1)
    {
      /* code */
      z = digitalRead(z_home);
      if (digitalRead(z_home) == HIGH)
      {
        stepper(2, -1, z_speed_home);
      }

      if (z == 0)
      {
        stt1 = true;
      }
      else
      {
        stt1 = false;
        jarak += 1;
      }
    }
  }
  koorXYZ[2] = 0;
}

void backtohome()
{
  bool stt1 = false;
  int x, y;

  z_backtohome();

  for (int i = 0; i < 200; i++)
  {
    stepper(0, 1, kecepatanHome);
    stepper(1, 1, kecepatanHome);
  }

  for (int x = 0; x <= 5; x++)
  {
    while (digitalRead(y_home) == HIGH || digitalRead(x_limit) == HIGH || !stt1)
    {

      x = digitalRead(x_limit);
      y = digitalRead(y_home);
      if (x == 1)
      {
        stepper(1, -1, kecepatanHome);
      }
      if (y == 1)
      {
        stepper(0, -1, kecepatanHome);
      }

      if (y == 0 && x == 0)
      {
        stt1 = true;
      }
      else
      {
        stt1 = false;
      }
    }
  }

  koorXYZ[1] = 0;
  koorXYZ[0] = 0;
}

int setPoints(int x)
{
  int set = x;
  if (x < 0)
  {
    set = x * -1;
  }
  return set * moveXY;
}

void inputSerial()
{

  String readSerial = checkSerial();

  if (readSerial == "U")
  {
    for (int a = 0; a <= moveXY; a++)
    {
      stepper(1, -1, kecepatanHome);
    }
    koorXYZ[1] += 1;
  }

  else if (readSerial == "D")
  {
    for (int a = 0; a <= moveXY; a++)
    {
      stepper(1, 1, kecepatanHome);
    }
    koorXYZ[1] -= 1;
  }

  else if (readSerial == "L")
  {
    for (int a = 0; a <= moveXY; a++)
    {
      stepper(0, 1, kecepatanHome);
    }
    koorXYZ[0] -= 1;
  }

  else if (readSerial == "R")
  {
    for (int a = 0; a <= moveXY; a++)
    {
      stepper(0, -1, kecepatanHome);
    }
    koorXYZ[0] += 1;
  }

  else if (readSerial == "T")
  {
    for (int a = 0; a <= 1; a++)
    {
      stepper(2, 1, z_speed_home);
    }
    koorXYZ[2] += 1;
  }

  else if (readSerial == "B")
  {
    if (koorXYZ[2] >= 1)
    {
      for (int a = 0; a <= 1; a++)
      {
        stepper(2, -1, z_speed_home);
      }
      koorXYZ[2] -= 1;
    }
  }

  else if (readSerial == "H")
  {
    backtohome();
    delay(500);
    for (int a = 0; a <= 4590; a++)
    {
      stepper(0, 1, kecepatanHome);
      stepper(1, 1, kecepatanHome);
    }
  }

  else if (readSerial == "<")
  {
    start[0] = koorXYZ[0]; // koordinat x start
    start[1] = koorXYZ[1]; // koordinat y start

    if (start[0] > 0 && start[1] > 1)
    {
      kuadranStart = 'A';
    }

    else if (start[0] < 0 && start[1] > 0)
    {
      kuadranStart = 'B';
    }

    else if (start[0] < 0 && start[1] < 0)
    {
      kuadranStart = 'C';
    }
    else if (start[0] > 0 && start[1] < 0)
    {
      kuadranStart = 'D';
    }
    Serial.println(kuadranStart);
  }

  else if (readSerial == ">")
  {
    end[0] = koorXYZ[0];
    end[1] = koorXYZ[1];
  }
  else if (readSerial == "O")
  {
    z_backtohome();
    for (int i = 0; i < 10050; i++)
    {
      stepper(2, 1, z_speed_home);
    }
    koorXYZ[2] = 10050;
  }

  else if (readSerial == "C")
  {
    backtohome();
    // Serial.println(kuadranStart);
    int x = setPoints(start[0]); // left right
    int y = setPoints(start[1]); // up down
    Serial.println(x);
    Serial.println(y);

    for (int a = 0; a <= 4590; a++)
    {
      stepper(0, 1, kecepatanHome);
      stepper(1, 1, kecepatanHome);
    }
    delay(100);
    if (kuadranStart == 'A')
    {
      Serial.println("active A");
      for (int a = 1; a <= y; a++) // u
      {
        stepper(1, -1, kecepatanHome);
      }

      for (int a = 1; a <= x; a++) // r
      {
        stepper(0, -1, kecepatanHome);
      }
      koorXYZ[0] = start[0];
      koorXYZ[1] = start[1];
    }

    else if (kuadranStart == 'B')
    {
      Serial.println("active B");
      for (int a = 1; a <= y; a++) // up
      {
        stepper(1, -1, kecepatanHome);
      }
      for (int a = 1; a <= x; a++) // left
      {
        stepper(0, 1, kecepatanHome);
      }
      koorXYZ[0] = start[0];
      koorXYZ[1] = start[1];
    }

    else if (kuadranStart == 'C')
    {
      Serial.println("active C");
      for (int a = 1; a <= y; a++) // down
      {
        stepper(1, 1, kecepatanHome);
      }
      for (int a = 1; a <= x; a++) // left
      {
        stepper(0, 1, kecepatanHome);
      }
      koorXYZ[0] = start[0];
      koorXYZ[1] = start[1];
    }

    else if (kuadranStart == 'D')
    {
      Serial.println("active D");
      for (int a = 1; a <= y; a++) // down
      {
        stepper(1, 1, kecepatanHome);
      }
      for (int a = 1; a <= x; a++) // right
      {
        stepper(0, -1, kecepatanHome);
      }
      koorXYZ[0] = start[0];
      koorXYZ[1] = start[1];
    }
  }
}

void run()
{
}

void microscanner()
{
  switch (action)
  {
  case 'A':
    /* code */
    break;

  case 'B':
    break;

  default:
    break;
  }
}
void setup()
{
  // EEPROM.begin(EEPROM_SIZE);
  // Tidak diperlukan pengaturan tambahan pada setup
  Serial.begin(115200);
  pinMode(ENA_pin, OUTPUT);
  pinMode(y_limit, INPUT);
  pinMode(y_home, INPUT);
  pinMode(x_limit, INPUT);
  pinMode(x_home, INPUT);
  pinMode(z_limit, INPUT);
  pinMode(z_home, INPUT);

  pinMode(xyz_dirPin[0], OUTPUT);
  pinMode(xyz_dirPin[1], OUTPUT);
  pinMode(xyz_dirPin[2], OUTPUT);

  pinMode(xyz_stepPin[0], OUTPUT);
  pinMode(xyz_stepPin[1], OUTPUT);
  pinMode(xyz_stepPin[2], OUTPUT);
  backtohome();
  delay(500);
  for (int a = 0; a <= 4590; a++)
  {
    stepper(0, 1, kecepatanHome);
    stepper(1, 1, kecepatanHome);
  }
}

void loop()
{
  inputSerial();
  Serial.print(koorXYZ[0]);
  Serial.print("  ");
  Serial.print(koorXYZ[1]);
  Serial.print("  ");
  Serial.print(koorXYZ[2]);

  Serial.print("     ");
  Serial.print(start[0]);
  Serial.print("  ");
  Serial.print(start[1]);
  Serial.print("     ");
  Serial.print("  ");
  Serial.print(end[0]);
  Serial.print("  ");
  Serial.println(end[1]);
}