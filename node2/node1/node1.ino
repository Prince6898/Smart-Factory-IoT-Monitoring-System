//include required libraries
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

#include "config.h"
#include "SensorManager.h"





void setup() 
{
  Serial.begin(9600);

  // put your setup code here, to run once:
   // initilase sensor 
   sensor_begin();
   //initialise actutors
   actuator_begin();
   // coonect board to internet and mqtt

}

void loop() 
{
  // put your main code here, to run repeatedly:
  //read data from the sensors 
  //publish to the cloud 

}
