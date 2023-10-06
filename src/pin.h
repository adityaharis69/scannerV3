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

int kecepatanHome = 500;
int kecepatanXY = 500;
int z_speed_home = 250;

int moverow = 40;
int movecol = 40;
int moveY = 1;
int moveXY = 40;

int currkoorX = 1;
int currkoorY = 1;

char action;
bool stop;
char kuadranStart;
char kuadranEnd;

int setkoorX = 0;
int setkoorY = 0;

int col;
int row;

int width;
int high;

int img;
bool reverseX = true;
bool reverseY = false;
bool reverseZ = true;

unsigned long previousMillis = 0; // Waktu sebelumnya
const long interval = 50;         // Interval waktu dalam milidetik (1 detik)
bool continuous = false;
int delayCapture = 500;
int delayAfterCapture = 500;
// volatile boolean geser = false;
int lastCol;
int lastRow;
int XX = 1;
bool active = true;

int z_pas = 10100;

int iterX = 0;
int iterY = 1;