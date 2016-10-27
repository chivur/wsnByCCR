#include <SPI.h>
#include "RF24Network.h"
#include "RF24.h"
#include "DHT.h"

/* PINOUT */
#define CE_PIN 2
#define CSN_PIN 3
#define DHT_PIN 6

/* other defines*/
#define SENSOR_DHT_NAME 2
#define DHTTYPE DHT11

/* radio class declarations */
RF24 radio(CE_PIN,CSN_PIN);
RF24Network network(radio);

/* mesh topology adrresses */
const uint16_t my_address = 01; /* address depending on node layout */
const uint16_t base_address = 00; 

/* structure to send */
typedef struct{
  int name;
  float humidity;
  float temp;
}TX_data;


DHT dht(DHT_PIN,DHTTYPE);
TX_data data;
void setup(){
 
  radio.begin();
  radio.setDataRate(RF24_1MBPS);/* data rate 1MB/s */
  radio.setPALevel(RF24_PA_LOW);/* tx power -- mention: low power = less tx range */

  network.begin(108,my_address);/* 108 represents the channel -- it has least interference with 2.4GHz wifi */
 
  data.name = SENSOR_DHT_NAME;   
  dht.begin();
  delay(2000);
}

void loop(){
  
  network.update();/* keep network layer information updated */

  data.humidity = dht.readHumidity();
  data.temp = dht.readTemperature();

  RF24NetworkHeader header(base_address);/* whom to send to */
  network.write(header,&data,sizeof(data));/* what data to send and what size */

  delay(2000);
}


