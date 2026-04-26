#include "SensorManager.h"
#include "config.h"
#include <DHT.h>

// #define DHTPIN 2
// #define DHTTYPE DHT22
static DHT dht(PIN_DHT, DHT_TYPE);

void sensor_init(SensorManager* sm) {
  sm->lastTemp        = 0.0f;
  sm->lastHumidity    = 0.0f;
  sm->motionActive    = false;
  sm->motionClearTime = 0;
}

void sensor_begin(void) {
  pinMode(PIN_PIR, INPUT);
  pinMode(PIN_DOOR, INPUT_PULLUP);
  dht.begin();
}

// read sensor  of PIR 
static bool readPIR(SensorManager * sm){
  //read value fro  motion sensor , chake if it is true for next 10 sec.. 
  const bool pirHigh = (digitalRead(PIN_PIR) == HIGH);
  const unsigned long now = millis();
  if(pirHigh){
    sm->motionActive = true;
    sm->motionClearTime = now + MOTION_CLEAR_MS;
  }
  else if(sm->motionActive && (now > sm->motionClearTime)){
    sm->motionActive = false ; 
  }
  return sm->motionActive ;  // 1 -> motion , 0 -> no motion (no one is their in wearhouse )
}


// read door status 
static bool readDoor(void){
  return (digitalRead(PIN_DOOR) == LOW); // 0 if door is open, 1 for close 


}

// read ldr sensor  (chake the light present or not in the wearhouse - needed for the cammer)
static uint16_t readLDR(void){
  uint16_t sum =0;
   uint8_t i;
  for(i =0 ; i<4;i++){ // read the avrage of 4 readings 
  sum += (uint16_t)analogRead(PIN_LDR);
  delay(2);  // 2ms


  }
  return sum/4 ; // wear house is dark or not , 300 > val ->dark , else light persent 
}


//read the data from the sensor , return the satus of DHT22 -> return 0 on error
bool sensor_read(SensorData* out , SensorManager * sm) {
  //resd DHT22 Sensor
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // if error in DHT sensor
  if (isnan(t) || isnan(h)) {

    out->sensorError = 1 ;
    out->temperature = sm->lastTemp;
    out->humidity = sm->lastHumidity;

  } else {
    out->sensorError = 0;
    out->temperature = round(t * 10.0f) * 0.1f;
    out->humidity = round(h * 10.0f) * 0.1f;
    sm->lastTemp = out->temperature;
    sm->lastHumidity = out->humidity;
  }

  //read LDRsensor //read door status //read motion detected 

  out->motionDetected = readPIR(sm);
  out->doorOpen = readDoor();
  out->ldrValue = readLDR();
  out->isDark =  (out->ldrValue < LDR_DARK_THRESH);// wear house is dark or not , 300 > val ->dark , else light persent 
  

  return !out->sensorError;
}