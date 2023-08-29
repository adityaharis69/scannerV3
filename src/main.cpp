#include <Stepper.h>
#include <Arduino.h>
#include "pin.h"
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels(NUMPIXELS, LEDRGB, NEO_GRB + NEO_KHZ800);

// Buat objek Stepper
Stepper z_stepper(stepCw, z_stepPin, z_dirPin);
Stepper x_stepper(stepCw, x_stepPin, x_dirPin);
Stepper y_stepper(stepCw, y_stepPin, y_dirPin);

void stepper(char stage, char dir, int speed) // x y z, - + ,speed
{

  switch (stage)
  {
  case 'X':
    if (dir == '+')
    {
      x_stepper.setSpeed(speed); // Atur kecepatan motor dalam langkah per detik
      x_stepper.step(stepCw);    // Putar sebanyak 1 rotasi
      Serial.print(stepCw);
    }
    else if (dir == '-')
    {
      x_stepper.setSpeed(speed); // Atur kecepatan motor dalam langkah per detik
      x_stepper.step(-stepCw);   // Putar sebanyak 1 rotasi
      Serial.print(stepCcw);
    }
    break;

  case 'Y':
    if ('+')
    {
      y_stepper.setSpeed(speed); // Atur kecepatan motor dalam langkah per detik
      y_stepper.step(stepCw);    // Putar sebanyak 1 rotasi
      Serial.println("y kanan");
    }
    else if (('-'))
    {

      y_stepper.setSpeed(speed); // Atur kecepatan motor dalam langkah per detik
      y_stepper.step(-stepCcw);  // Putar sebanyak 1 rotasi
      Serial.println("y kiri");
    }
    break;

  case 'Z':
    if ('+')
    {
      z_stepper.setSpeed(speed); // Atur kecepatan motor dalam langkah per detik
      z_stepper.step(stepCcw);   // Putar sebanyak 1 rotasi
      Serial.println("z kanan");
    }
    else if (('-'))
    {
      z_stepper.setSpeed(speed); // Atur kecepatan motor dalam langkah per detik
      z_stepper.step(-stepCcw);  // Putar sebanyak 1 rotasi
      Serial.println("z kiri");
    }
    break;
  }
}

void ENA_ON()
{
  pinMode(ENA_pin, LOW);
}

void ENA_OFF()
{
  pinMode(ENA_pin, HIGH);
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

boolean checkSerial(String value)
{
  if (Serial.available() > 0)
  {
    String input = Serial.readStringUntil(':');
    input.trim();
    if (input == value)
    {
      return true;
    }
    return false;
  }
  return false;
}

int nilai = 20;

void setup()
{
  EEPROM.begin(EEPROM_SIZE);
  // Tidak diperlukan pengaturan tambahan pada setup
  Serial.begin(115200);
  pinMode(ENA_pin, OUTPUT);
  bool check = checkSerial("s");
  y_stepper.setSpeed(speedX); // Atur kecepatan motor dalam langkah per detik

  int ko_x = 0;

  // while (ko_x != jarakX)
  // {
  //   x_stepper.step(stepCw); // Putar sebanyak 1 rotasi
  //   ko_x += 1;
  //   koorXY[0] = ko_x;
  //   Serial.println(koorXY[0]);
  // }

  // while (ko_x != 0)
  // {
  //   x_stepper.step(stepCcw); // Putar sebanyak 1 rotasi
  //   ko_x -= 1;
  //   koorXY[0] = ko_x;
  //   Serial.println(koorXY[0]);
  // }
}

void loop()
{
   y_stepper.step(stepCw);
   Serial.println("sukses");
}
