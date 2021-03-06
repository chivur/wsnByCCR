#include "RF24Network.h"
#include "RF24.h"
#include <SPI.h>

/* PINOUT */
#define CE_PIN 9 
#define CSN_PIN 10 

/* radio class declarations */
RF24 radio(CE_PIN,CSN_PIN);
RF24Network network(radio);

/* other defines */
#define SENSOR_GAS_NAME 1
#define SENSOR_DHT_NAME 2
#define SENSOR_PIR_NAME 3

/* PIR_MESSAGES */
#define PIR_MOTION_DETECTED 5
#define PIR_MOTION_STOPPED 6

/* mesh topology adrresses */
const uint16_t my_address = 00;/* base node (coordinator) */
const uint16_t send_address = 04;/* addr of a node i want to send data to */
/* routing is handled using the logical node addresses: when sending a message using RF24Network::write(), 
 you fill in the header with the logical node address; the network layer figures out the right path to find that node,
 and sends it through the system until it gets to the right place. This works even if the two nodes are far separated, 
 as it will send the message down to the base node, and then back out to the final destination */

typedef struct{
  int name;
  float val1; /* gas or humidity */
  float val2; /* temp */
}RX_data;


RX_data data;
//float data = -2;

void setup(){

  Serial.begin(115200);

  radio.begin();
  radio.setDataRate(RF24_1MBPS);/* data rate 1MB/s */
  radio.setPALevel(RF24_PA_LOW);/* tx power -- mention: low power = less tx range */

  network.begin(108,my_address);/* 108 represents the channel -- it has least interference with 2.4GHz wifi */

}


void loop(){

  /* keep network layer information updated */
  network.update();
  
  /* see if there is something for me */
  while(network.available()){
  
    RF24NetworkHeader header;
    network.read(header,&data,sizeof(data));
    //Serial.println(data);
    Serial.print("Got from");
    if(data.name == SENSOR_GAS_NAME){
      Serial.print(" gas sensor :");
      Serial.println(data.val1);  
    }
    if(data.name == SENSOR_DHT_NAME){
      Serial.print(" dht sensor :(humidity)");
      Serial.print(data.val1);
      Serial.print(" (temp)");
      Serial.println(data.val2);  
    }
    if(data.name == SENSOR_PIR_NAME){
      Serial.print(" pir sensor: ");
      if(data.val1 == PIR_MOTION_DETECTED){
        Serial.println("motion detected");
      }
      if(data.val1 == PIR_MOTION_STOPPED){
        Serial.println("motion stopped");
      }
      
    }
    
  }

}

