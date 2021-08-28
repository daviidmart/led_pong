#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define B1 1
#define B2 2

#define PIN 0        // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 51 // Popular NeoPixel ring size
#define MEDIUMLED 25 // led central

#define GREEN pixels.Color(20, 255, 20)
#define RED pixels.Color(255, 3, 3)
#define BLUE pixels.Color(20, 20, 255)
#define ORANGE pixels.Color(255, 40, 0)

int game = 0;
float speed = 40;
int actual = 1;
int last = 0;
bool forward = true;
int level = 0;

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  pinMode(B1, INPUT);
  pinMode(B2, INPUT);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void reset(int g)
{
  game = g;
  speed = 40;
  actual = 1;
  last = 0;
  forward = true;
  level = 0;
}

void nextLed()
{
  if (actual == NUMPIXELS && forward)
  {
    forward = false;
  }

  if (actual == 0 && !forward)
  {
    forward = true;
  }

  last = actual;
  if (forward)
  {
    actual++;
  }
  else
  {
    actual--;
  }
}

void changeSpeed()
{
  if (speed > 10)
  {
    speed -= 1;
  }
  else if (speed <= 10 && speed > 5)
  {
    speed -= 0.5;
  }
  else if (speed >= 5 && speed > 1)
  {
    speed -= 0.25;
  }
  else if (speed <= 1 && speed >= 0.1)
  {
    speed -= 0.1;
  }
}

void endGame(int winner)
{
  pixels.clear();
  if (winner == 1)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      if (i < MEDIUMLED)
      {
        pixels.setPixelColor(i, GREEN);
      }

      if (i > MEDIUMLED)
      {
        pixels.setPixelColor(i, RED);
      }
    }
  }

  if (winner == 2)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      if (i < MEDIUMLED)
      {
        pixels.setPixelColor(i, RED);
      }

      if (i > MEDIUMLED)
      {
        pixels.setPixelColor(i, GREEN);
      }
    }
  }

  pixels.show();
  for (int x = 0; x < 35; x++)
  {
    pixels.setPixelColor(actual, RED);
    pixels.show();
    delay(100);
    pixels.setPixelColor(actual, pixels.Color(0,0,0));
    pixels.show();
    delay(100);
  }
  pixels.clear();
  pixels.show();
  reset(0);
}

byte stateOne = LOW;
void statusButtonOne()
{
  if (digitalRead(B1) == HIGH && stateOne == LOW)
  {
    stateOne = HIGH;
  }

  if (digitalRead(B1) == LOW && stateOne == HIGH)
  {
    stateOne = LOW;
  }
}

byte stateTwo = LOW;
void statusButtonTwo()
{
  if (digitalRead(B2) == HIGH && stateTwo == LOW)
  {
    stateTwo = HIGH;
  }

  if (digitalRead(B2) == LOW && stateTwo == HIGH)
  {
    stateTwo = LOW;
  }
}

void unpresed()
{
  if (NUMPIXELS - level == actual)
  {
    endGame(1);
  }

  if (0 + level == actual)
  {
    endGame(2);
  }
}

void demo()
{
  if (stateOne == HIGH || stateTwo == HIGH)
  {
    reset(1);
    pixels.clear();
    pixels.show();
    delay(500);
    return;
  }

  if (actual != MEDIUMLED)
  {
    pixels.setPixelColor(last, pixels.Color(0, 0, 0));
    pixels.setPixelColor(actual, ORANGE);
    delay(speed);
  }
  else
  {
    pixels.setPixelColor(last, pixels.Color(0, 0, 0));
  }

  pixels.setPixelColor(MEDIUMLED, ORANGE);
  pixels.setPixelColor(0, ORANGE);
  pixels.setPixelColor(NUMPIXELS - 1, ORANGE);
  pixels.show();

  nextLed();
}

void gameOne()
{
  if (stateOne == HIGH && forward)
  {
    if (actual > 25)
    {
      forward = !forward;
    }
    else
    {
      endGame(1);
    }
  }

  if (stateOne == HIGH && !forward)
  {
    if (actual < 25)
    {
      endGame(1);
    }
  }

  if (stateTwo == HIGH && !forward)
  {
    if (actual < 25)
    {
      forward = !forward;
    }
    else
    {
      endGame(2);
    }
  }

  if (stateTwo == HIGH && forward)
  {
    if (actual > 25)
    {
      endGame(2);
    }
  }

  if (actual != MEDIUMLED)
  {
    pixels.setPixelColor(last, pixels.Color(0, 0, 0));
    pixels.setPixelColor(actual, BLUE);
    delay(speed);
  }
  else
  {
    pixels.setPixelColor(last, pixels.Color(0, 0, 0));
    changeSpeed();
  }

  pixels.setPixelColor(MEDIUMLED, RED);
  pixels.setPixelColor(0, RED);
  pixels.setPixelColor(NUMPIXELS - 1, RED);
  pixels.show();

  nextLed();
  unpresed();
}

void loop()
{
  statusButtonOne();
  statusButtonTwo();

  switch (game)
  {
  case 1:
    gameOne();
    break;

  default:
    demo();
    break;
  }
}