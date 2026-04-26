//include required libraries
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>
#include <DHT.h>

#include "config.h"
#include "SensorManager.h"
#include "actuator.h"
#include "network.h"
#include "telemetry.h"
#include "rpc.h"

static char sharedBuf[200];


// creat  object of type ethernet pubsubclient
static EthernetClient ethClient;
static PubSubClient mqttClient(ethClient);

unsigned long lastTelemetry = 0;


void setup() {
  Serial.begin(9600);
  Serial.print("Node 1 is booting... ");


  // put your setup code here, to run once:
  // initilase sensor
  sensor_begin();
  //initialise actutors
  actuators_begin();

  telemetry_init(sharedBuf, sizeof(sharedBuf));
  rpc_init(&mqttClient, sharedBuf, sizeof(sharedBuf)); // initializing rpc request

  // upon reciving the data which function needs to call back 
  mqttClient.setCallback(rpc_mqttCallback);
 




  // coonect board to internet and mqtt
  network_begin(&mqttClient);
  mqttClient.setCallback(rpc_mqttCallback);

  digitalWrite(PIN_LED_RED, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  //keep chaking if board connecte to things board
  network_maintain();



    //read data from the sensors
    SensorData data;
  sensors_read(&data);  // humidity, tem , lm35 , vib , se_error , machin_status

  //publish to the cloud
  //check if network is awelable than push the data to the thinks board
  if (network_isConnected()) {
    unsigned long now = millis();
    //publish data to thiks board every 5 sec
    if (now - lastTelemetry >= TELEMETRY_INTERVAL) {   // if 5 sec over
      lastTelemetry = now;
      telemetry_publishTelemetry(&data,actuators_getRelayState); // converting sensor dta into json and publish
    }
  }

  actuators_updateStatusLEDs(network_isConnected(),data.sensorError);
}
