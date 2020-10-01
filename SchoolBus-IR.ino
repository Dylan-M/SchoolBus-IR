#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>

// IR
#define IRpin       11 // Digital Pin 11
#define DEBUG       true
IRrecv irrecv(IRpin);
decode_results results;

// Modes
#define MODE_OFF    0
#define MODE_YELLOW 1
#define MODE_RED    2
#define Y_LEFT      A0 // Analog Pin 0, Used as Extra Digital Pin
#define Y_RIGHT     9  // Digital Pin 9
#define R_LEFT      A1 // Analog Pin 1, Used as Extra Digital Pin
#define R_RIGHT     10 // Digital Pin 10
int mode = MODE_OFF;
unsigned long modeMillis = 0;
int modeInterval = 800;
bool modeRight = false;

// Strobe
bool strobe = false;
unsigned long strobeMillis = 0;
int strobeInterval = 400;
bool strobeOn = false;
#define STROBE_PIN  A2 // Analog Pin 1, Used as Extra Digital Pin

// General Lights
bool lightsOn = false;
bool blinkers = false;
#define LIGHTS      0 // Digital Pin 0 (RX)
#define BLINKERS    1 // Digital Pin 1 (TX)

void setup()
{
  if (DEBUG) {
    Serial.begin(9600);
  }
  irrecv.enableIRIn(); // Start the receiver
}

void loop() 
{
  // Handle IR Remote Communications
  if (irrecv.decode(&results)) {
    unsigned long value = results.value;
    if (DEBUG) {
      Serial.println(value); // Print the Serial 'results.value'
    }
    // Handle the value
    switch (value) {
      case 16738455: // 0 Key, MODE_OFF
        mode = MODE_OFF;
        if (DEBUG) {
          Serial.println("Entering MODE_OFF");
        }
        break;
      case 16724175: // 1 Key, MODE_YELLOW
        mode = MODE_YELLOW;
        if (DEBUG) {
          Serial.println("Entering MODE_YELLOW");
        }
        break;
      case 16718055: // 2 key, MODE_RED
        mode = MODE_RED;
        if (DEBUG) {
          Serial.println("Entering MODE_RED");
        }
        break;
      case 16743045: // 3 Key, Strobe Toggle
        strobeOn != strobeOn;
        if (DEBUG) {
          Serial.println("Strobe Toggled");
        }
        break;
      case 16716015: // 4 Key, Lights Toggle
        lightsOn != lightsOn;
        if (DEBUG) {
          Serial.println("Lights Toggled");
        }
        break;
      case 16726215: // 5 Key, Blinkers Toggle
        blinkers != blinkers;
        if (DEBUG) {
          Serial.println("Blinkers Toggled");
        }
        break;
    }
    irrecv.resume();   // Receive the next value
  }

  unsigned long currentMillis = millis();

  // Handle Modes
  if (currentMillis - modeMillis >= modeInterval) {
    modeMillis = currentMillis;
    modeRight != modeRight;
    switch (mode) {
      case MODE_OFF:
        digitalWrite(Y_LEFT, HIGH);
        digitalWrite(Y_RIGHT, HIGH);
        digitalWrite(R_LEFT, HIGH);
        digitalWrite(R_RIGHT, HIGH);
        break;
      case MODE_YELLOW:
        digitalWrite(Y_LEFT, modeRight);
        digitalWrite(Y_RIGHT, !modeRight);
        digitalWrite(R_LEFT, HIGH);
        digitalWrite(R_RIGHT, HIGH);
        if (DEBUG) {
          digitalWrite(13, !modeRight);
          Serial.println("Mode Toggle");
        }
        break;
      case MODE_RED:
        digitalWrite(Y_LEFT, HIGH);
        digitalWrite(Y_RIGHT, HIGH);
        digitalWrite(R_LEFT, modeRight);
        digitalWrite(R_RIGHT, !modeRight);
        if (DEBUG) {
          digitalWrite(13, !modeRight);
          Serial.println("Mode Toggle");
        }
        break;
      default:
        // error state should never be reached
        mode = MODE_OFF;
        break;
    }
  }

  // Handle Strobe
  if (currentMillis - strobeMillis >= strobeInterval) {
    strobeMillis = currentMillis;
    if (strobe) {
      digitalWrite(STROBE_PIN, !strobeOn);
      strobeOn != strobeOn;
    }
  }
}

/*
 * CH+      16769565
 * CH       16736925
 * CH-      16753245
 * <<       16720605
 * >>       16712445
 * EQ       16748655
 * -        16769055
 * +        16754775
 * EQ       16748655
 * 0        16738455
 * 100+     16750695
 * 200+     16756815
 * 1        16724175
 * 2        16718055
 * 3        16743045
 * 4        16716015
 * 5        16726215
 * 6        16734885
 * 7        16728765
 * 8        16730805
 * 9        16732845
 * repeat   4294967295
 */
