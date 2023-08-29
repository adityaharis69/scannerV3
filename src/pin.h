// Definisikan jumlah langkah per rotasi dan pin yang akan digunakan
int stepsPerRevolution = 200; // Jumlah langkah per rotasi pada motor stepper

int stepCw = 200;   // cw stepper
int stepCcw = -200; // cw stepper

int speedX=70;

const int z_stepPin = 33; // Pin yang digunakan untuk pulsa (STEP)
const int z_dirPin = 27;  // Pxcin yang digunakan untuk arah (DIR)

const int y_stepPin = 2; // Pin yang digunakan untuk pulsa (STEP)
const int y_dirPin = 32; // Pin yang digunakan untuk arah (DIR)

const int x_stepPin = 25; // Pin yang digunakan untuk pulsa (STEP)
const int x_dirPin = 12;  // Pin yang digunakan untuk arah (DIR)

const int z_limit = 13;
const int z_home = 14;

const int y_limit = 15;
const int y_home = 16;

const int x_limit = 19;
const int x_home = 21;

const int ENA_pin = 26; // enable pin

const int rgb = 22;

const int data = 2;
float arrayData[data] = {0};

int LEDRGB = 4;
int NUMPIXELS = 8;

int koorXY[2]; // x dan y

#define EEPROM_SIZE 1

const int addIsRun = 1;

int jarakX=20;

