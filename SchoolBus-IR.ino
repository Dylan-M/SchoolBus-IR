#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremote.h>

#define IRpin 11
#define DEBUG false
IRrecv irrecv(IRpin);
decode_results results;

void setup()
{
  if (DEBUG) {
    Serial.begin(9600);
  }
  irrecv.enableIRIn(); // Start the receiver
}

void loop() 
{
  if (irrecv.decode(&results)) {
    if (DEBUG) {
      Serial.println(results.value, DEC); // Print the Serial 'results.value'
    }
    irrecv.resume();   // Receive the next value
  }
}
