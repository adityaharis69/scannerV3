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

void valueCheck()
{
    if (Serial.available() > 0)
    {
        String input = Serial.readStringUntil('\n');
        int index0 = input.indexOf(":");
        int index1 = input.indexOf(":", index0 + 1);

        String perintah = input.substring(0, index0);
        double nilai1 = (input.substring(index0 + 1, index1)).toInt();

        if (perintah == "E")
        {
            kecepatanXY = nilai1;
        }
        else if (perintah == "P")
        {
            delayCapture = nilai1;
        }
    }
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
}

void backtohome()
{
    bool stt1 = false;
    int x, y;
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
    if (x <= 0)
    {
        set = x * -1;
        return set;
    }
    else
    {
        return set;
    }
}

void startPosition()
{
    backtohome();
    int x = setPoints(start[0]); // left right
    int y = setPoints(start[1]); // up down
    int iter_y = 1;
    int iter_x = 1;

    for (int a = 0; a <= 4590; a++)
    {
        stepper(0, 1, kecepatanHome);
        stepper(1, 1, kecepatanHome);
    }

    if (kuadranStart == 'A')
    {
        // Serial.println("active A");
        // for (int a = 0; a <= y; a++) // u
        // {
        //     stepper(1, -1, kecepatanHome);
        // }

        // for (int a = 1; a <= x; a++) // T:
        // {
        //     stepper(0, -1, kecepatanHome);
        // }

        while (iter_y <= y)
        {
            for (int a = 1; a <= moveXY; a++) // u
            {
                stepper(1, -1, kecepatanHome);
            }
            iter_y += 1;
        }

        while (iter_x <= x)
        {
            for (int a = 1; a <= moveXY; a++) // u
            {
                stepper(0, -1, kecepatanHome);
            }
            iter_x += 1;
        }

        koorXYZ[0] = start[0];
        koorXYZ[1] = start[1];
    }

    else if (kuadranStart == 'B')
    {
        // Serial.println("active B");
        // for (int a = 1; a <= y; a++) // up
        // {
        //     stepper(1, -1, kecepatanHome);
        // }
        // for (int a = 1; a <= x; a++) // left
        // {
        //     stepper(0, 1, kecepatanHome);
        // }

        while (iter_y <= y)
        {
            for (int a = 1; a <= moveXY; a++) // u
            {
                stepper(1, -1, kecepatanHome);
            }
            iter_y += 1;
        }

        while (iter_x <= x)
        {
            for (int a = 1; a <= moveXY; a++) // u
            {
                stepper(0, 1, kecepatanHome);
            }
            iter_x += 1;
        }
        koorXYZ[0] = start[0];
        koorXYZ[1] = start[1];
    }

    else if (kuadranStart == 'C')
    {
        // Serial.println("active C");
        // for (int a = 1; a <= y; a++) // down
        // {
        //     stepper(1, 1, kecepatanXY);
        // }
        // for (int a = 1; a <= x; a++) // left
        // {
        //     stepper(0, 1, kecepatanXY);
        // }

        while (iter_y <= y)
        {
            for (int a = 1; a <= moveXY; a++) // u
            {
                stepper(1, 1, kecepatanXY);
            }
            iter_y += 1;
        }

        while (iter_x <= x)
        {
            for (int a = 1; a <= moveXY; a++) // u
            {
                stepper(0, 1, kecepatanXY);
            }
            iter_x += 1;
        }
        koorXYZ[0] = start[0];
        koorXYZ[1] = start[1];
    }

    else if (kuadranStart == 'D')
    {
        // Serial.println("active D");
        // for (int a = 1; a <= y; a++) // down
        // {
        //     stepper(1, 1, kecepatanXY);
        // }
        // for (int a = 1; a <= x; a++) // right
        // {
        //     stepper(0, -1, kecepatanXY);
        // }

        while (iter_y <= y)
        {
            for (int a = 1; a <= moveXY; a++) // u
            {
                stepper(1, 1, kecepatanXY);
            }
            iter_y += 1;
        }

        while (iter_x <= x)
        {
            for (int a = 1; a <= moveXY; a++) // u
            {
                stepper(0, -1, kecepatanXY);
            }
            iter_x += 1;
        }
        koorXYZ[0] = start[0];
        koorXYZ[1] = start[1];
    }
}

void zDevault(int val)
{
    z_backtohome();
    for (int i = 0; i < val; i++)
    {
        stepper(2, 1, z_speed_home);
    }
    koorXYZ[2] = val;
}

int totalImge(int a, int b, int c, int d)
{

    width = c - a;
    high = d - b;
    if (width < 1)
    {
        width = width * -1;
    }
    if (high < 1)
    {
        high = high * -1;
    }
    width = width + 1;
    high = high + 1;
    return width * high;
}

void inputSerial()
{
    String readSerial = checkSerial();

    if (readSerial == "U")
    {
        for (int a = 1; a <= movecol; a++)
        {
            stepper(1, -1, kecepatanXY);
        }
        koorXYZ[1] += 1;
    }

    else if (readSerial == "D")
    {
        for (int a = 1; a <= movecol; a++)
        {
            stepper(1, 1, kecepatanXY);
        }
        koorXYZ[1] -= 1;
    }

    else if (readSerial == "L")
    {
        for (int a = 1; a <= moveXY; a++)
        {
            stepper(0, 1, kecepatanXY);
        }
        koorXYZ[0] -= 1;
    }

    else if (readSerial == "R")
    {
        for (int a = 1; a <= moveXY; a++)
        {
            stepper(0, -1, kecepatanXY);
        }
        koorXYZ[0] += 1;
    }

    else if (readSerial == "LU")
  {
    // Bergerak serong kiri atas
    for (int a = 0; a <= moveXY; a++)
    {
      stepper(0, 1, kecepatanHome); // Bergerak ke kiri
      stepper(1, -1, kecepatanHome); // Bergerak ke atas
    }
    koorXYZ[0] -= 1;
    koorXYZ[1] += 1;
  }

  else if (readSerial == "LD")
  {
    // Bergerak serong kiri bawah
    for (int a = 0; a <= moveXY; a++)
    {
      stepper(0, 1, kecepatanHome); // Bergerak ke kiri
      stepper(1, 1, kecepatanHome); // Bergerak ke bawah
    }
    koorXYZ[0] -= 1;
    koorXYZ[1] -= 1;
  }

  else if (readSerial == "RU")
  {
    // Bergerak serong kanan atas
    for (int a = 0; a <= moveXY; a++)
    {
      stepper(0, -1, kecepatanHome); // Bergerak ke kanan
      stepper(1, -1, kecepatanHome); // Bergerak ke atas
    }
    koorXYZ[0] += 1;
    koorXYZ[1] += 1;
  }

   else if (readSerial == "RD")
  {
    // Bergerak serong kanan bawah
    for (int a = 0; a <= moveXY; a++)
    {
      stepper(0, -1, kecepatanHome); // Bergerak ke kanan
      stepper(1, 1, kecepatanHome); // Bergerak ke bawah
    }
    koorXYZ[0] += 1;
    koorXYZ[1] -= 1;
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
        z_backtohome();
        backtohome();
        delay(500);
        for (int a = 0; a <= 4590; a++)
        {
            stepper(0, 1, kecepatanHome);
            stepper(1, 1, kecepatanHome);
        }
        zDevault(z_pas);
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
    }

    else if (readSerial == ">")
    {
        end[0] = koorXYZ[0]; // koordinat x start
        end[1] = koorXYZ[1]; // koordinat y start

        img = totalImge(start[0], start[1], end[0], end[1]);

        Serial.print("G");
        Serial.print(";");
        Serial.print(width);
        Serial.print(";");
        Serial.print(high);
        Serial.print(";");
        Serial.println(img);
    }
    else if (readSerial == "O")
    {
        zDevault(z_pas);
        end[0] = koorXYZ[0];
        end[1] = koorXYZ[1];
    }

    else if (readSerial == "C")
    {
        startPosition();
    }
    else if (readSerial == "S")
    {
        startPosition();
        action = 'S';
        active = true;
        stop = false;
    }

    else if (readSerial == "!")
    {
        stop = true;
    }
}

void timer()
{
    unsigned long currentMillis = millis(); // Ambil waktu saat ini
    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        inputSerial();
        valueCheck();
    }
}

void run()
{
    int totalImg = 1;
    XX = 1;
    int y = 0;
    row = width;
    col = high;

    while (!stop && totalImg <= img)
    {
        if (XX == 1 && currkoorX <= row)
        {
            delay(delayAfterCapture);
            Serial.print("V");
            Serial.print(";");
            Serial.println(totalImg);
            delay(delayCapture);
            String geser = checkSerial();

            while (currkoorX <= row && geser == "Q")
            {

                for (int a = 0; a <= moverow; a++)
                {
                    stepper(0, -1, kecepatanXY);
                }

                iterX += 1;

                geser = "N";
                totalImg += 1;
                currkoorX += 1;
            }
            if (currkoorX > row)
            {
                for (int a = 0; a <= movecol; a++)
                {
                    stepper(1, 1, kecepatanXY);
                }
                XX = 0;
                y = 0;
                iterY += 1;
                currkoorX -= 1;
                for (int a = 1; a <= row; a++)
                {
                    for (int a = 0; a <= moverow; a++)
                    {
                        stepper(0, +1, kecepatanXY);
                    }
                }
            }
        }

        if (XX == 0 && currkoorX >= 1)
        {
            delay(delayAfterCapture);
            Serial.print("V");
            Serial.print(";");
            Serial.println(totalImg);
            delay(delayCapture);
            String geser = checkSerial();

            while (currkoorX >= 1 && geser == "Q")
            {
                for (int a = 0; a <= moverow; a++)
                {
                    stepper(0, -1, kecepatanXY);
                }

                iterX -= 1;
                currkoorX -= 1;
                totalImg += 1;
                geser = "N";
            }
            if (currkoorX < 1)
            {
                for (int a = 0; a <= movecol; a++)
                {
                    stepper(1, 1, kecepatanXY);
                }
                XX = 1;
                y = 0;
                iterY += 1;
                currkoorX += 1;
                for (int a = 1; a <= row; a++)
                {
                    for (int a = 0; a <= moverow; a++)
                    {
                        stepper(0, +1, kecepatanXY);
                    }
                }
            }
        }
    }
    action = 'B';
}

void microscanner()
{
    switch (action)
    {
    case 'S':
        delay(500);
        run();
        break;

    case 'B':
        Serial.println("FN");
        backtohome();
        for (int a = 0; a <= 4590; a++)
        {
            stepper(0, 1, kecepatanHome);
            stepper(1, 1, kecepatanHome);
        }
        action = 'C';
        break;

    default:
        break;
    }
}

void display()
{
    Serial.print("koordinat x :");
    Serial.print(koorXYZ[0]);
    Serial.print("  ");
    Serial.print("koordinat y :");
    Serial.print(koorXYZ[1]);
    Serial.print("  ");
    Serial.print("koordinat z :");
    Serial.print(koorXYZ[2]);
    Serial.print("  ");
    Serial.print("start :");
    Serial.print(start[0]);
    Serial.print("  ");
    Serial.print(start[1]);
    Serial.print("  ");
    Serial.print("end :");
    Serial.print(end[0]);
    Serial.print("  ");
    Serial.print(end[1]);
    Serial.print("  ");
    Serial.print("width :");
    Serial.print(width);
    Serial.print("  ");
    Serial.print("high :");
    Serial.print(high);
    Serial.print("  ");
    Serial.print("row :");
    Serial.print(row);
    Serial.print("  ");
    Serial.print("col :");
    Serial.print(col);
    Serial.print("  ");
    Serial.print("img :");
    Serial.println(img);
}

void setup()
{
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
    z_backtohome();
    backtohome();
    for (int a = 0; a <= 4590; a++)
    {
        stepper(0, 1, kecepatanHome);
        stepper(1, 1, kecepatanHome);
    }
    zDevault(z_pas);
    koorXYZ[2] = z_pas;
}

void loop()
{
    timer();
    microscanner();
    // display();
}