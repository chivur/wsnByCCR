#include <SPI.h>
#include <Wire.h>
#include "RF24Network.h"
#include "RF24.h"
#include "BMP280.h"

/* PINOUT */
#define CE_PIN 2
#define CSN_PIN 3

/* BMP connection */
BMP280 bmp;; /* I2C */

/* other defines*/
#define SENSOR_PRESSURE_NAME 4

/* radio class declarations */
RF24 radio(CE_PIN,CSN_PIN);
RF24Network network(radio);

/* mesh topology adrresses */
const uint16_t my_address = 04; /* address depending on node layout */
const uint16_t base_address = 00; 

/* structure to send */
typedef struct{
  int name;
  double pressure;/* mBar */
  float temp;
}TX_data;


TX_data data;
void setup(){
 
  radio.begin();
  radio.setDataRate(RF24_1MBPS);/* data rate 1MB/s */
  radio.setPALevel(RF24_PA_LOW);/* tx power -- mention: low power = less tx range */

  network.begin(108,my_address);/* 108 represents the channel -- it has least interference with 2.4GHz wifi */
  
  bmp.begin();
  bmp.setOversampling(4);
  data.name = SENSOR_PRESSURE_NAME;   
  delay(2000);
}

double fake;
void loop(){
  
  network.update();/* keep network layer information updated */

  //data.pressure = bmp.readPressure();
  bmp.getTemperatureAndPressure(fake,data.pressure);

  RF24NetworkHeader header(base_address);/* whom to send to */
  network.write(header,&data,sizeof(data));/* what data to send and what size */

  delay(2000);
}
