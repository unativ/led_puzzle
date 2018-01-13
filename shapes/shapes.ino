#include "LedControl.h"

const byte TOTAL_SHAPES = 11;

// -----------------------------
int score;
boolean inGame = false;
bool startNewGame = false;

const int RED_PIN     = 10;
const int YELLOW_PIN  = 11;
const int GREEN_PIN   = 12;

const int number_of_displays = 4;
LedControl lc = LedControl(4, 6, 8, number_of_displays); //Pin 4 = DIN, Pin 6 = CLK, Pin 8 = CS. 5 = number of displays

unsigned long time;


class Button {
  byte pin;
  byte lastState;
  
  public:
    Button(byte pinParam) {
      pin = pinParam;
      lastState = HIGH;
    }
    
    byte getPin() {
      return pin;
    }

};

const byte NEW_GAME_BUTTON_PIN = 9;
byte newGameButtonState = LOW;
byte lastNewGameButtonState = HIGH;

const int leftButtonPin = 3;
int leftButtonState = 0;  
int lastLeftButtonState = HIGH;

const int downButtonPin = 5;
int downButtonState = 0;
int lastDownButtonState = HIGH;

const int rightButtonPin = 2;
int rightButtonState = 0;
int lastRightButtonState = HIGH;

const int upButtonPin = 7;
int upButtonState = 0;
int lastUpButtonState = HIGH;

// --------------- Shapes -------------------------

//https://rodrigosc.github.io/ArduinoLedMatrix/char_builder/builder.html
//https://xantorohara.github.io/led-matrix-editor/#1c08080808080c08|3e0408102020221c|1c2220201820221c|20203e2224283020|1c2220201e02023e|1c2222221e02221c|040404081020203e|1c2222221c22221c|1c22203c2222221c|1c2222222222221c
byte empty[] = {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
byte A[] = {B00000000,B00011000,B00100100,B00100100,B00111100,B00100100,B00100100,B00000000};
byte B[] = {B01111000,B01001000,B01001000,B01110000,B01001000,B01000100,B01000100,B01111100};
byte C[] = {B00000000,B00011110,B00100000,B01000000,B01000000,B01000000,B00100000,B00011110};
byte D[] = {B00000000,B00111000,B00100100,B00100010,B00100010,B00100100,B00111000,B00000000};
byte E[] = {B00000000,B00111100,B00100000,B00111000,B00100000,B00100000,B00111100,B00000000};
byte F[] = {B00000000,B00111100,B00100000,B00111000,B00100000,B00100000,B00100000,B00000000};
byte G[] = {B00000000,B00111110,B00100000,B00100000,B00101110,B00100010,B00111110,B00000000};
byte H[] = {B00000000,B00100100,B00100100,B00111100,B00100100,B00100100,B00100100,B00000000};
byte I[] = {B00000000,B00111000,B00010000,B00010000,B00010000,B00010000,B00111000,B00000000};
byte J[] = {B00000000,B00011100,B00001000,B00001000,B00001000,B00101000,B00111000,B00000000};
byte K[] = {B00000000,B00100100,B00101000,B00110000,B00101000,B00100100,B00100100,B00000000};
byte L[] = {B00000000,B00100000,B00100000,B00100000,B00100000,B00100000,B00111100,B00000000};
byte M[] = {B00000000,B00000000,B01000100,B10101010,B10010010,B10000010,B10000010,B00000000};
byte N[] = {B00000000,B00100010,B00110010,B00101010,B00100110,B00100010,B00100010,B00000000};
byte O[] = {B00000000,B00111100,B01000010,B01000010,B01000010,B01000010,B00111100,B00000000};
byte P[] = {B00000000,B00111000,B00100100,B00100100,B00111000,B00100000,B00100000,B00000000};
byte Q[] = {B00000000,B00111100,B01000010,B01000010,B01000010,B01000110,B00111110,B00000001};
byte R[] = {B00000000,B00111000,B00100100,B00100100,B00111000,B00100100,B00100100,B00000000};
byte S[] = {B00000000,B00111100,B00100000,B00111100,B00000100,B00000100,B00111100,B00000000};
byte T[] = {B00000000,B01111100,B00010000,B00010000,B00010000,B00010000,B00010000,B00000000};
byte U[] = {B00000000,B01000010,B01000010,B01000010,B01000010,B00100100,B00011000,B00000000};
byte V[] = {B00000000,B00100010,B00100010,B00100010,B00010100,B00010100,B00001000,B00000000};
byte W[] = {B00000000,B10000010,B10010010,B01010100,B01010100,B00101000,B00000000,B00000000};
byte X[] = {B00000000,B01000010,B00100100,B00011000,B00011000,B00100100,B01000010,B00000000};
byte Y[] = {B00000000,B01000100,B00101000,B00010000,B00010000,B00010000,B00010000,B00000000};
byte Z[] = {B00000000,B00111100,B00000100,B00001000,B00010000,B00100000,B00111100,B00000000};

byte P1[] = {B00000000,
             B00000011,
             B00000010,
             B00000010,
             B00000011,
             B00000010,
             B00000010,
             B00000000};

byte P2[] = {B00000000,
             B10000000,
             B01000000,
             B01000000,
             B10000000,
             B00000000,
             B00000000,
             B00000000};

byte happy[8]={0x7e, 0x81, 0xa5, 0x81, 0xa5, 0x99, 0x81, 0x7e};
byte normal[8]={0x7e, 0x81, 0xa5, 0x81, 0x81, 0x81, 0x81, 0x7e};
byte sad[8]={0x7e, 0x81, 0xa5, 0x81, 0x99, 0xa5, 0x81, 0x7e};

const byte digits[][8] = {
{
  B00000000,
  B00111100,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B00111100
},
{
  B00000000,
  B00011000,
  B00011000,
  B00111000,
  B00011000,
  B00011000,
  B00011000,
  B01111110
},{
  B00000000,
  B00111100,
  B01100110,
  B00000110,
  B00001100,
  B00110000,
  B01100000,
  B01111110
},{
  B00000000,
  B00111100,
  B01100110,
  B00000110,
  B00011100,
  B00000110,
  B01100110,
  B00111100
},{
  B00000000,
  B00001100,
  B00011100,
  B00101100,
  B01001100,
  B01111110,
  B00001100,
  B00001100
},{
  B00000000,
  B01111110,
  B01100000,
  B01111100,
  B00000110,
  B00000110,
  B01100110,
  B00111100
},{
  B00000000,
  B00111100,
  B01100110,
  B01100000,
  B01111100,
  B01100110,
  B01100110,
  B00111100
},{
  B00000000,
  B01111110,
  B01100110,
  B00001100,
  B00001100,
  B00011000,
  B00011000,
  B00011000
},{
  B00000000,
  B00111100,
  B01100110,
  B01100110,
  B00111100,
  B01100110,
  B01100110,
  B00111100
},{
  B00000000,
  B00111100,
  B01100110,
  B01100110,
  B00111110,
  B00000110,
  B01100110,
  B00111100
}};

byte x1[] = {0x0, 0x0, 0x0, 0x0, 0x18, 0x24, 0x7e, 0xff};
byte x2[8]={0x0, 0x0, 0x0, 0x0, 0x3c, 0x7e, 0xdb, 0xff};
byte x3[8]={0x24, 0x24, 0x24, 0x24, 0x66, 0x66, 0x66, 0xe7};
byte ii[8]={0x0, 0xee, 0x44, 0x44, 0x44, 0x44, 0xee, 0x0};

byte shapes[][4][8]={
  // yinyang 0
  {                                                   // open brace
    {0x7, 0x1f, 0x3f, 0x7c, 0x7c, 0xff, 0xff, 0xff},  // up left
    {0xe0, 0xf8, 0xfc, 0xf6, 0xf2, 0xf1, 0xe1, 0xc1}, // up right
    {0xff, 0xf8, 0xf0, 0x70, 0x70, 0x38, 0x1e, 0x7},  // down left
    {0x81, 0x1, 0x1, 0xc2, 0xc6, 0xc, 0x18, 0xe0}     // down right    
  },                                                  // close brace and comma between shapes
  // heart 1
  {
    {B00000000, B00011100, B00111110, B01111110, B01111111, B01111111, B01111111, B01111111},
    {B00000000, B00111000, B01111100, B01111110, B11111110, B11111110, B11111110, B11111110},
    {B01111111, B00111111, B00011111, B00001111, B00000111, B00000011, B00000001, B00000000},
    {B11111110, B11111100, B11111000, B11110000, B11100000, B11000000, B10000000, B00000000}
  },
  // Torii 2
  {
    {0xfe, 0xfe, 0x7e, 0x3e, 0x6, 0xe, 0xe, 0xe},
    {0x7f, 0x7f, 0x7c, 0x78, 0x60, 0x70, 0x70, 0x70},
    {0xe, 0xe, 0xe, 0xe, 0xe, 0xe, 0xc, 0x8},
    {0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x30, 0x10}
  },
  // alien 3
  {
    {0x0, 0x0, 0x0, 0x4, 0x2, 0x7, 0xd, 0x1d},
    {0x0, 0x0, 0x0, 0x20, 0x40, 0xe0, 0xb0, 0xb8},    
    {0x17, 0x14, 0x6, 0x0, 0x0, 0x0, 0x0, 0x0},
    {0xe8, 0x28, 0x60, 0x0, 0x0, 0x0, 0x0, 0x0}
  },
   // ghost 4
   {
  {0x0, 0x0, 0xf, 0x1f, 0x32, 0x60, 0x79, 0x79},
  {0x0, 0x0, 0xc0, 0xe0, 0x38, 0x1c, 0x9c, 0x9c},
  {0x62, 0x7f, 0x7f, 0x7f, 0x77, 0x63, 0x41, 0x0},
  {0x3c, 0xfc, 0xfc, 0xfc, 0xcc, 0x84, 0x0, 0x0}
   },
   {
    // home 5
    {0x1, 0x3, 0x7, 0xf, 0x1f, 0x3f, 0x7f, 0xff},
    {0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff},
    {0x3f, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c},
    {0xfc, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c}
   },
   // question mark 6
   {
   {0x0, 0xf, 0x1f, 0x3c, 0x78, 0x70, 0x70, 0x70},
   {0x0, 0xf0, 0xf8, 0x7c, 0x1c, 0x1c, 0x38, 0x70},
   {0x0, 0x3, 0x3, 0x0, 0x3, 0x7, 0x3, 0x0},
   {0x60, 0xc0, 0xc0, 0x0, 0xc0, 0xe0, 0xc0, 0x0}
    },
// toad 7
    {
    {0x7, 0x19, 0x21, 0x43, 0x46, 0xfc, 0xcc, 0x84},
    {0xe0, 0xf8, 0xe4, 0xf2, 0x1a, 0xf, 0xf, 0xd},
    {0x86, 0xcf, 0xff, 0x72, 0x22, 0x20, 0x10, 0xf},
    {0x19, 0xf9, 0xfd, 0x4f, 0x44, 0x4, 0x8, 0xf0}
    },
   // ski 8
   {
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3},
    {0x0, 0x0, 0x0, 0x0, 0x0, 0x7c, 0x7c, 0xfc},
    {0x15, 0x7a, 0xf5, 0xff, 0x40, 0x7f, 0x0, 0x0},
    {0xfc, 0x7c, 0xfe, 0xfa, 0x2, 0xfc, 0x0, 0x0}
   },
   // tahel 9
   {
   {0x0, 0x0, 0x3, 0x2, 0x2, 0x2, 0xe, 0x0},
   {0x0, 0x0, 0xc0, 0x40, 0x40, 0x40, 0x40, 0x0},
   {0x0, 0x20, 0x3c, 0x4, 0x4, 0x8, 0x10, 0x0},
   {0x0, 0x0, 0x7c, 0x4, 0x4, 0x44, 0x44, 0x0} 
   },
   // cube 10
   {
   {0x0, 0x0, 0x0, 0xf, 0x10, 0x20, 0x3f, 0x20},
   {0x0, 0x0, 0x0, 0xfc, 0xc, 0x14, 0xe4, 0x24},
   {0x20, 0x20, 0x20, 0x20, 0x20, 0x3f, 0x0, 0x0},
   {0x24, 0x24, 0x24, 0x28, 0x30, 0xe0, 0x0, 0x0}
  },
  // cherry 11  
  {
  {0x0, 0x0, 0x7, 0x7, 0x3, 0x0, 0x0, 0x1},
  {0x0, 0x0, 0x0, 0x80, 0x80, 0x80, 0x80, 0x40},
  {0x2, 0x2, 0x6, 0x9, 0x9, 0x6, 0x0, 0x0},
  {0x20, 0x20, 0x30, 0x48, 0x48, 0x30, 0x0, 0x0}
  }
  // angry 11
  //{0x7, 0xf, 0x3f, 0x47, 0xf1, 0xf9, 0xff, 0xff},
  //{0xe0, 0xf0, 0xfc, 0xe2, 0x8e, 0x9f, 0xff, 0xff},
 
};

// -------------------------------------------

byte UL[8];
byte UR[8];
byte DL[8];
byte DR[8];

byte perm[TOTAL_SHAPES];

void random_shapes()
{
  // fill array with numbers
  for (int i = 0; i < TOTAL_SHAPES; i++) {
    perm[i] = i;
  }
  
  // create a permutation
  for (int i = 0; i < TOTAL_SHAPES; i++) {
    byte randNumber = random(TOTAL_SHAPES - i);
    byte tmp = perm[i];
    perm[i] = perm[randNumber];
    perm[randNumber] = tmp;
  }
    
  for (int i = 0; i < TOTAL_SHAPES; i++) {  
    Serial.println(perm[i]);
  }
}

byte ledsPerm[] = {0, 1, 2, 3};

void random_leds()
{
  // fill array with numbers
  for (int i = 0; i < 4; i++) {
    ledsPerm[i] = i;
  }
  
  // create a permutation
  for (int i = 0; i < 4; i++) {
    byte randNumber = random(4 - i);
    byte tmp = ledsPerm[i];
    ledsPerm[i] = ledsPerm[randNumber];
    ledsPerm[randNumber] = tmp;
  }
}


byte current_shape;

//**************
// Arduino Setup
//**************
void setup()
{
  //start serial communication
  Serial.begin(9600);

  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  randomSeed(analogRead(0));

  random_shapes();
  current_shape = perm[0];

  pinMode(NEW_GAME_BUTTON_PIN, INPUT);
  
  pinMode(leftButtonPin, INPUT);
  pinMode(downButtonPin, INPUT);
  pinMode(upButtonPin, INPUT);
  pinMode(rightButtonPin, INPUT);

  //initialize displays (configure light intensity and clear all)
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
  
  lc.shutdown(1,false);
  lc.setIntensity(1,1);
  lc.clearDisplay(1);
  
  lc.shutdown(2,false);
  lc.setIntensity(2,1);
  lc.clearDisplay(2);
  
  lc.shutdown(3,false);
  lc.setIntensity(3,1);
  lc.clearDisplay(3);
  
  memcpy(UL, L, 8 * sizeof(byte));
  memcpy(UR, E, 8 * sizeof(byte));
  memcpy(DL, P1, 8 * sizeof(byte));
  memcpy(DR, P2, 8 * sizeof(byte));

  drawLeds();
  delay(500);

}

void newGame() {
  memcpy(UL, X, 8 * sizeof(byte));
  memcpy(UR, X, 8 * sizeof(byte));
  memcpy(DL, X, 8 * sizeof(byte));
  memcpy(DR, X, 8 * sizeof(byte));
  drawLeds();
  
  // light up red, yellow, green 
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  delay(2000);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, HIGH);
  delay(2000);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  
  time = millis(); // start the timer
  random_leds();

  inGame = true;
}

bool isSolved() {
  bool solved = true;
  for (int i = 0; i < 4; i++) {
    if (ledsPerm[i] != i) {
      solved = false;
    }
  }
  return solved;
}

// --------------------------------------------------------------
// Times
// --------------------------------------------------------------
// Show the green/yellow/red lights according to the time left
bool checkTimer() {
  int times[] = { 25000, 20000, 10000, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 0 };
  //int times[] = { 5000, 2000, 1000, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 0 };
  // green, yellow, red
  byte lights[][3] = {
    { HIGH, LOW, LOW },
    { LOW, HIGH, LOW },
    { LOW, LOW, HIGH },
    
    { HIGH, HIGH, HIGH },
    { LOW, LOW, LOW },
    { HIGH, HIGH, HIGH },
    { LOW, LOW, LOW },
    { HIGH, HIGH, HIGH },
    { LOW, LOW, LOW },
    { HIGH, HIGH, HIGH },
    { LOW, LOW, LOW },
    { HIGH, HIGH, HIGH },
    { LOW, LOW, LOW }
  };
  
  unsigned long newTime = millis();
  unsigned long totalTime = times[0];
  bool found = false;
  byte i = 0;
  byte light[3];
  
  while (!found) {
    if (times[i] == 0 || newTime - time < totalTime) {
      memcpy(light, lights[i], 3 * sizeof(int));
      found = true;
    } else {
      i++;
      totalTime += times[i];
    }
  }
  digitalWrite(GREEN_PIN, light[0]);
  digitalWrite(YELLOW_PIN, light[1]);
  digitalWrite(RED_PIN, light[2]);

  if (times[i] == 0) {
    return true;
  }
  else {
    return false;
  }
}



void gameOver() {
  inGame = false;
  for (int i = 0; i < 3; i++) {
    memcpy(UL, G, 8 * sizeof(byte));
    memcpy(UR, A, 8 * sizeof(byte));
    memcpy(DL, M, 8 * sizeof(byte));
    memcpy(DR, E, 8 * sizeof(byte));
    drawLeds();
    
    delay(1000);
    memcpy(UL, O, 8 * sizeof(byte));
    memcpy(UR, V, 8 * sizeof(byte));
    memcpy(DL, E, 8 * sizeof(byte));
    memcpy(DR, R, 8 * sizeof(byte));
    drawLeds();
    delay(1000);
  }

  // show score
  if (score <= 2) {
    memcpy(UL, sad, 8 * sizeof(byte));
    memcpy(UR, sad, 8 * sizeof(byte));    
  }
  else if (score <= 4) {
    memcpy(UL, sad, 8 * sizeof(byte));
    memcpy(UR, empty, 8 * sizeof(byte));        
  }
  else if (score <= 7) {
    memcpy(UL, normal, 8 * sizeof(byte));
    memcpy(UR, empty, 8 * sizeof(byte));        
  }
  else if (score <= 10) {
    memcpy(UL, happy, 8 * sizeof(byte));
    memcpy(UR, empty, 8 * sizeof(byte));            
  }
  else {
    memcpy(UL, happy, 8 * sizeof(byte));
    memcpy(UR, happy, 8 * sizeof(byte));            
  }
  byte highDigit = score / 10;
  byte lowDigit = score % 10;
  if (highDigit > 0) {
    memcpy(DL, digits[highDigit], 8 * sizeof(byte));   
  }
  else {
    memcpy(DL, empty, 8 * sizeof(byte));
  }
  memcpy(DR, digits[lowDigit], 8 * sizeof(byte));
  drawLeds();
  delay(5000);
  
}

void draw(int matrix, byte data[]) {
  for (int i = 0; i < 8; i++) {
    lc.setRow(matrix, i, data[i]);
  }
}


void drawLeds() {
  draw(0, UL);
  draw(1, DL);
  draw(2, DR);
  draw(3, UR);
}

void handle_buttons() {
  newGameButtonState = digitalRead(NEW_GAME_BUTTON_PIN);
  if (newGameButtonState != lastNewGameButtonState) {
    if (newGameButtonState == HIGH) {
      Serial.println("New game pressed");
      delay(200);
      startNewGame = true;
      return;
    }
    delay(200);
    lastNewGameButtonState = newGameButtonState;
  }
  if (!inGame) {
    return;
  }
  
  leftButtonState = digitalRead(leftButtonPin);
  if (leftButtonState != lastLeftButtonState) {
    if (leftButtonState == HIGH) {
      change(UL, DL, 0, 2);
    }
    delay(200);
    lastLeftButtonState = leftButtonState;
  }

  downButtonState = digitalRead(downButtonPin);
  if (downButtonState != lastDownButtonState) {
    if (downButtonState == HIGH) {
      change(DL, DR, 2, 3);
    }
    delay(200);
    lastDownButtonState = downButtonState;
  }
  
  rightButtonState = digitalRead(rightButtonPin);
  if (rightButtonState != lastRightButtonState) {
    if (rightButtonState == HIGH) {
      change(UR, DR, 1, 3);
    }
    delay(200);
    lastRightButtonState = rightButtonState;
  }

  upButtonState = digitalRead(upButtonPin);
  if (upButtonState != lastUpButtonState) {
    if (upButtonState == HIGH) {
      change(UL, UR, 0, 1);
    }
    delay(200);
    lastUpButtonState = upButtonState;
    
  }
}

void change(byte led1[], byte led2[], byte id1, byte id2) {
  byte tmp[8];
  memcpy(tmp, led1, 8 * sizeof(byte));
  memcpy(led1, led2, 8 * sizeof(byte));
  memcpy(led2, tmp, 8 * sizeof(byte));

  byte tmp2 = ledsPerm[id1];
  ledsPerm[id1] = ledsPerm[id2];
  ledsPerm[id2] = tmp2;  
}



//**********
// Main Loop
//**********
int turn = 0;

void loop()
{ 
  handle_buttons();

  if (startNewGame) {
    newGame();
    score = 0;
    random_leds();
    turn = 0;
    startNewGame = false;
    delay(1000);
  }

  if (!inGame) {
    return;
  }
  
  // show red/yellow/green according to the time left
  if (checkTimer()) {
    gameOver();
  }

  drawLeds();

  if (isSolved()) {    
    score++;
    turn++;
    if (turn >= TOTAL_SHAPES) {
      turn = 0;
    }
    current_shape = perm[turn];
    random_leds();
    delay(500);
  
  }
  else {

    memcpy(UL, shapes[current_shape][ledsPerm[0]], 8 * sizeof(byte));
    memcpy(UR, shapes[current_shape][ledsPerm[1]], 8 * sizeof(byte));
    memcpy(DL, shapes[current_shape][ledsPerm[2]], 8 * sizeof(byte));
    memcpy(DR, shapes[current_shape][ledsPerm[3]], 8 * sizeof(byte));      
  }
}

