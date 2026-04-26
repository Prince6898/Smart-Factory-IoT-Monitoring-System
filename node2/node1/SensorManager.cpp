#include "SensorManager.h"
#include <DHT.h>

static DHT dht(DHTPIN, DHTTYPE);

void sensor_begin(void)
{
  pinMode(PIN_BUTTON, INTPUT_PULLUP);
  dht.begin();
}