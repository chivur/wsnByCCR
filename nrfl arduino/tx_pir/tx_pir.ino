#include <SPI.h>
#include "RF24Network.h"
#include "RF24.h"
#include "DHT.h"

/* PINOUT */
#define PIR_SENSOR_PIN 6
#define CE_PIN 2
#define CSN_PIN 3

/* other defines */
#define SENSOR_PIR_NAME 3
#define MOTION_DETECTED 5
#define MOTION_STOPPED 6

/* radio class declarations */
RF24 radio(CE_PIN,CSN_PIN);
RF24Network network(radio);

/* mesh topology adrresses */
const uint16_t my_address = 03; /* address depending on node layout */
const uint16_t base_address = 00; 

/* structure to send*/
typedef struct{
  int name;
  float motion;
  float none;
}TX_data;

TX_data data;
void setup(){
  pinMode(PIR_SENSOR_PIN,INPUT);

  radio.begin();
  radio.setDataRate(RF24_1MBPS);/* data rate 1MB/s */
  radio.setPALevel(RF24_PA_LOW);/* tx power -- mention: low power = less tx range */

  network.begin(108,my_address);/* 108 represents the channel -- it has least interference with 2.4GHz wifi */
  
  data.name = SENSOR_PIR_NAME;
  data.none = -1;
  
  delay(1000);
}

int val = 0;/* by default no motion detected */
int state = LOW;/* store sensor status */

void loop(){
  network.update();
  
  val = digitalRead(PIR_SENSOR_PIN);
  if(val==HIGH){
    delay(100);
    if(state==LOW){
      state = HIGH; /* motion detected */
    
      data.motion = MOTION_DETECTED;  
      RF24NetworkHeader header(base_address);/* whom to send to */
      network.write(header,&data,sizeof(data));/* what data to send and what size */
    }     
  }
  else{ 
    delay(200);
    if(state==HIGH){
      state = LOW; /* motion stopped */  
      data.motion = MOTION_STOPPED;  
      RF24NetworkHeader header(base_address);/* whom to send to */
      network.write(header,&data,sizeof(data));/* what data to send and what size */
      delay(1000);
    }  
  }
  
  
}
