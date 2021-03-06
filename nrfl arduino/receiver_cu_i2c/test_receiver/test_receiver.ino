#include "RF24Network.h"
#include "RF24.h"
#include <SPI.h>
#include <Wire.h>

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
#define SENSOR_PRESSURE_NAME 4
#define GAS_DANGER 9
#define GAS_NO_DANGER 10
#define GAS_THRESHHOLD 1

/* I2C defines */
#define SLAVE_ADDR 0x04 /* connect through i2c with raspberry pi */
#define TYPE_CMD 1
#define TYPE_DATA 2
#define TEMP_IDX 0
#define HUMID_IDX 1
#define GAS_IDX 2
#define MOTION_IDX 3
#define PRESSURE_IDX 4

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
  float val1; /* gas or humidity or pressure */
  float val2; /* temp */
}RX_data;

RX_data data;

int names[4];
float values[4];
void setup(){

  /* verbose, for direct debugging connection arduino-pc */
  //Serial.begin(115200);

names[0]=1;names[1]=2;
names[2]=3;
names[3]=4;

values[0] = 21.23;
values[1] = 41.43;
values[2] = 61.63;
values[3] = 81.83;

  /* i2c initializations */
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  //Serial.println("Setup ready !");
}

float latest_temperature=0;
float latest_humidity=0;
float latest_gas=0;
int latest_pir_motion=0;
float latest_pressure=0;

char str_latest_temperature[6];
char str_latest_humidity[6];
char str_latest_gas[6];
char str_latest_pir_motion[6];
char str_latest_pressure[6];

int integer_part;
float fraction_part;
int trunc_fraction_part;

int name_idx = 0;
void loop(){
  
    data.name = names[name_idx];
    data.val1 = values[name_idx];
    data.val2 = values[name_idx++];
   
   if(name_idx > 4){
    name_idx = 0; 
   }
    
    //Serial.print("Got from");
    
    if(data.name == SENSOR_GAS_NAME){
      //Serial.print(" gas sensor : ");
      //Serial.println(data.val1);
      latest_gas = data.val1+0.01;
      integer_part = latest_gas;
      fraction_part = latest_gas - integer_part;
      trunc_fraction_part = trunc(fraction_part * 100);
      sprintf(str_latest_gas,"%d.%.2d",integer_part,trunc_fraction_part);
      
    }
    
    if(data.name == SENSOR_PRESSURE_NAME){
      //Serial.print(" pressure sensor : ");
      latest_pressure = data.val1+0.01;  
      /* convert to string so we can send over i2c to raspberry pi */
      integer_part = latest_pressure;
      fraction_part = latest_pressure - integer_part;
      trunc_fraction_part = trunc(fraction_part * 100);
      sprintf(str_latest_pressure,"%d.%.2d",integer_part,trunc_fraction_part);
     // Serial.println(data.val1);
    }
    
    if(data.name == SENSOR_DHT_NAME){
      //Serial.print(" dht sensor :(humidity) ");
      //Serial.print(data.val1);
      //Serial.print(" (temp)");
      //Serial.println(data.val2);
      latest_humidity = data.val1+0.01;
      latest_temperature = 2*data.val2+0.01;  
      
      integer_part = latest_humidity;
      fraction_part = latest_humidity - integer_part;
      trunc_fraction_part = trunc(fraction_part * 100);
      sprintf(str_latest_humidity,"%d.%.2d",integer_part,trunc_fraction_part);
      
      integer_part = latest_temperature;
      fraction_part = latest_temperature - integer_part;
      trunc_fraction_part = (fraction_part * 100);
      sprintf(str_latest_temperature,"%d.%.2d",integer_part,trunc_fraction_part);
    }
    
    if(data.name == SENSOR_PIR_NAME){
      //Serial.print(" pir sensor: ");
      if(data.val1 < PIR_MOTION_DETECTED){
        //Serial.println("motion detected");
      }
      if(data.val1 >= PIR_MOTION_STOPPED){
        //Serial.println("motion stopped");
      }
      latest_pir_motion = data.val1; /* either PIR_MOTION_DETECTED or STOPPED */ 
      sprintf(str_latest_pir_motion,"%d",data.val1);
    }
    
    //delay(3000);
}//end loop


/* callback for received data on i2c */
void receiveData(int byteCount){
  
}



int i2c_data_type=TYPE_CMD; /* selects if i am sending a command(TYPE_CMD) or data(TYPE_DATA);; first we have to send TYPE_CMD */
int i2c_cmd_idx = 0; /* index 0 coresponds to temperature, etc */
int i2c_data_idx = 0; /* index for the string of data to be send over i2c */
/* callback for sending data on i2c */
void sendData(){
  
  /* TODO : verifica formatul datelor (ex. temperatura : 27.23 - 5 caractere de trimis ; umiditate : cate caractere de trimis ? etc */
  if(i2c_data_type == TYPE_CMD){
    Wire.write(i2c_cmd_idx);/* TEMP_IDX for temperature etc */
    //Serial.print("[i2c] wrote cmd for "); Serial.println(i2c_cmd_idx);
    i2c_data_type = TYPE_DATA; /* set this because next thing we have to do is send data coresponding to the type of sensor id */ 
  }
  else{ /* assume i2c_data_type = TYPE_DATA */
    switch(i2c_cmd_idx){ /* first find out what kind of data you have to send */
      case TEMP_IDX:
        Wire.write(str_latest_temperature[i2c_data_idx++]);
      //  Serial.print("[i2c] wrote temp data");Serial.println(str_latest_temperature[i2c_data_idx-1]);
        if(i2c_data_idx>4){
          /* i am done sending temperature data, move on to next command */
          i2c_data_idx=0;
          i2c_cmd_idx++;
          i2c_data_type = TYPE_CMD;
        //  Serial.println("[i2c] next cmd after temp");  
        }
        break;
      case HUMID_IDX:
        Wire.write(str_latest_humidity[i2c_data_idx++]);
        //Serial.print("[i2c] wrote temp data");Serial.println(str_latest_humidity[i2c_data_idx-1]);
        if(i2c_data_idx>4){
          /* i am done sending temperature data, move on to next command */
          i2c_data_idx=0;
          i2c_cmd_idx++;
          i2c_data_type = TYPE_CMD;
          //Serial.println("[i2c] next cmd after humid");  
        }
        break;
      case GAS_IDX:
        if(latest_gas >= GAS_THRESHHOLD){
         /* danger detected */
          Wire.write(GAS_DANGER);
          //Serial.println("[i2c] wrote gas danger");
        }
        else{
          Wire.write(GAS_NO_DANGER); 
          //Serial.println("[i2c] wrote gas NO danger");
        }
        i2c_cmd_idx++;
        i2c_data_idx=0;
        i2c_data_type = TYPE_CMD;
        //Serial.println("[i2c] next cmd after gas");
        /* uncomment below if you want to send adc read from gas sensor */
        //Wire.write(str_latest_gas[i2c_data_idx++]);
        //if(i2c_data_idx>5){
          /* i am done sending temperature data, move on to next command */
          /*i2c_data_idx=0;
          i2c_cmd_idx++;
          i2c_data_type = TYPE_CMD;  
        }*/
        break;
      case MOTION_IDX:
        Wire.write(PIR_MOTION_DETECTED);/* only have to send 1 int */
        //Serial.print("[i2c] wrote motion ");Serial.println(latest_pir_motion);
        i2c_cmd_idx++;
        i2c_data_type = TYPE_CMD;Serial.println("[i2c] next cmd after motion");
        break;
      case PRESSURE_IDX:
        Wire.write(str_latest_pressure[i2c_data_idx++]);
        Serial.print("[i2c] wrote pressure data");Serial.println(str_latest_pressure[i2c_data_idx-1]);
        if(i2c_data_idx>4 ){
          /* i am done sending temperature data, move on to next command */
          i2c_data_idx=0;
          i2c_cmd_idx=0;/* last update, have to start all over again */
          i2c_data_type = TYPE_CMD;  
          //Serial.println("[i2c] next cmd after pressure");
        }
        break; 
    }//end switch    
  }//end else

  
}//end send data
