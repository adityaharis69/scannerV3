// Definisikan jumlah langkah per rotasi dan pin yang akan digunakan 3290
// posisi z home ke objek xample
int xyz_dirPin[] = {32, 12, 27}; // Y,X,Z
int xyz_stepPin[] = {2, 25, 33}; // Y,X,Z

int z_limit = 13;
int z_home = 14;

int y_limit = 15;
int y_home = 23;

int x_limit = 19;
int x_home = 21;

const int ENA_pin = 26; // enable pin

const int rgb = 22;

const int data = 2;
float arrayData[data] = {0};

int LEDRGB = 4;
int NUMPIXELS = 8;

int koorXYZ[3]; // xy

int start[2], end[2];

#define EEPROM_SIZE 1

const int addIsRun = 1;

boolean reverseX = true;
boolean reverseY = false;
boolean reverseZ = true;

int kecepatanHome = 1000;
int kecepatanHomeXY = 1000;
int z_speed_home = 500;

int moveXY = 50;

int currkoorX = 0;
int currkoorY = 0;

char action;
bool stop;
char kuadranStart;
char kuadranEnd;
