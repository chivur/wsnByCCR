#include "RF24.h"
#include "RF24Network.h"
#include <SPI.h>


/* PINOUT */
#define CE_PIN 2 /* already wired on pcb <-> for atmega8 -- PB2(pin4) */
#define CSN_PIN 3 /* already wired on pcb <-> for atmega8 -- PB3(pin5) */
#define GAS_SENSOR_PIN A0 /* on pcb <-> pin 2 of sensor header */

/* other defines*/
#define SENSOR_GAS_NAME 1

/* radio class declarations */
RF24 radio(CE_PIN,CSN_PIN);
RF24Network network(radio);/* class for the mesh network connections */


/* mesh topology adrresses */
const uint16_t my_address = 02; /* address depending on the layout of the nodes that form the WSN */
const uint16_t base_address = 00;/* gateway(coordinator) addr */
/* routing is handled using the logical node addresses: when sending a message using RF24Network::write(), 
 you fill in the header with the logical node address; the network layer figures out the right path to find that node,
 and sends it through the system until it gets to the right place. This works even if the two nodes are far separated, 
 as it will send the message down to the base node, and then back out to the final destination */

/* structure to send */
typedef struct{
  int name;
  float val;
  float none;
}TX_data;

TX_data data;

void setup(){

  radio.begin();
  radio.setDataRate(RF24_1MBPS); /* data rate 1MB/s */
  radio.setPALevel(RF24_PA_LOW); /* tx power -- mention: low power = less tx range */

  network.begin(108,my_address); /* 108 represents the channel -- it has least interference with 2.4GHz wifi */

  pinMode(GAS_SENSOR_PIN,INPUT);
  data.name = SENSOR_GAS_NAME;
  data.none = -1;
}


void loop(){

  network.update(); /* keep network layer information updated */

  data.val = digitalRead(GAS_SENSOR_PIN);

  RF24NetworkHeader header(base_address); /* specify whom to send to */
  network.write(header,&data,sizeof(data)); /* what data to send and what size */

  delay(1000);
}

