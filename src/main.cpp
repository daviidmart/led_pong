#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define B1 1
#define B2 2

#define PIN 0        // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 51 // Popular NeoPixel ring size
#define MEDIUMLED 25 // led central

bool end = true;
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

void restart()
{
  pixels.clear();
  pixels.show();
  end = false;
  speed = 40;
  actual = 1;
  last = 0;
  forward = true;
  level = 0;
  delay(500);
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
  else if (speed <= 1 && speed > 0.1)
  {
    speed -= 0.1;
  }
}

void endGame(int winner)
{
  end = true;
  pixels.clear();
  if (winner == 1)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      if (i < MEDIUMLED)
      {
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      }

      if (i > MEDIUMLED)
      {
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      }
    }
  }

  if (winner == 2)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      if (i < MEDIUMLED)
      {
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      }

      if (i > MEDIUMLED)
      {
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      }
    }
  }

  pixels.show();
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

  if (stateOne == HIGH && !forward)
  {
    endGame(1);
  }
  else
  {
    forward = !forward;
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

  if (stateTwo == HIGH && forward)
  {
    endGame(1);
  }
  else
  {
    forward = !forward;
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

void rainbow(int wait)
{
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 150)
  {
    for (int i = 0; i < pixels.numPixels(); i++)
    {
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.setBrightness(100);
    pixels.show(); // Update strip with new contents
    delay(wait);   // Pause for a moment
  }
}

void loop()
{
  if (end == true)
  {
    if (digitalRead(B1) == HIGH && end == true)
    {
      restart();
    }

    //rainbow(1);
  }
  else
  {
    statusButtonOne();
    statusButtonTwo();

    if (actual != MEDIUMLED)
    {
      pixels.setPixelColor(last, pixels.Color(0, 0, 0));
      pixels.setPixelColor(actual, pixels.Color(0, 255, 0));
      delay(speed);
    }
    else
    {
      pixels.setPixelColor(last, pixels.Color(0, 0, 0));
      changeSpeed();
    }

    pixels.setPixelColor(MEDIUMLED, pixels.Color(255, 0, 0));
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.setPixelColor(NUMPIXELS - 1, pixels.Color(255, 0, 0));
    pixels.show();

    nextLed();
    unpresed();
  }
}