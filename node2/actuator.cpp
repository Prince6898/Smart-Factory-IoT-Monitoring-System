#include "actuator.h"
static uint8_t relay_status;

static uint8_t blinkstate = 0;
static unsigned long lastBlinkTime = 0;

void actuators_begin(void) {
  pinMode(PIN_LED_RED, OUTPUT);
  pinMode(PIN_LED_GREEN, OUTPUT); // on everithing is normal
  pinMode(PIN_LED_YELLOW, OUTPUT); // on-> warning , blinking -> critical
  pinMode(PIN_RELAY, OUTPUT); // contol ventilation , if more humidity 



  digitalWrite(PIN_LED_GREEN, LOW);
  // digitalWrite(PIN_LED_RED, LOW);
  digitalWrite(PIN_LED_YELLOW, LOW);
  digitalWrite(PIN_RELAY, LOW);
}


//from rpc request is to set relay then turn on the relay 
void    actuators_setRelay(uint8_t on){
  relay_status = on ;
  digitalWrite(PIN_RELAY, on ? HIGH : LOW);

}

uint8_t actuators_getRelayState(void){
  return relay_status;

}


void actuators_updateStatusLEDs(uint8_t mqttConnected, uint8_t sensorError) {
  // if board ot connected to thkis board then turn on red led
  if (!mqttConnected) {
    digitalWrite(PIN_LED_RED, HIGH);
    digitalWrite(PIN_LED_GREEN, LOW);
    digitalWrite(PIN_LED_YELLOW, LOW);
    return;
  }
  // if mqtt is up turn off  red led
  digitalWrite(PIN_LED_RED, LOW);


  // if sensor error then turn on yellow led
  if (sensorError) {
    digitalWrite(PIN_LED_GREEN, HIGH);
    unsigned long now = millis();
    if (now - lastBlinkTime > 500) {  // delay 5 sec..
      lastBlinkTime = now;
      blinkstate = !blinkstate;
      digitalWrite(PIN_LED_YELLOW, blinkstate ? HIGH : LOW);
    }
  } else {
    //all good

    digitalWrite(PIN_LED_GREEN, HIGH);
    digitalWrite(PIN_LED_YELLOW, LOW);
  }


  // if every think is normal turn on green led
}