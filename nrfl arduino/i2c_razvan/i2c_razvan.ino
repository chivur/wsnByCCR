
#include <Wire.h>

#define SLAVE_ADDRESS 0x04
int number = 0;
int state = 0;

byte data[4];

union u_tag{
  byte b[4];
  float fval;  
} u;



void setup() {

  u.b[0] = data[0] = 2;
  u.b[1] = data[1] = 0;
  u.b[2] = data[2] = 0;
  u.b[3] = data[3] = 1;


  pinMode(13, OUTPUT);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  Serial.println("Ready");
  Serial.print("sizeof(float) = ");Serial.println(sizeof(float));
}

void loop() {
  delay(100);
}

// callback for received data
void receiveData(int byteCount){

  while(Wire.available()) {
    number = Wire.read();
    Serial.print("Data received : ");
    Serial.println(number);
    
    if (number == 1){
    
      if (state == 0){
        digitalWrite(13, HIGH); // set the LED on
        state = 1;
      }
      else{
        digitalWrite(13, LOW); // set the LED off
        state = 0;
      }
    }
  }
}

float fv = 123.76;
uint8_t i2cdata[4];
int idx = 0;
int cmdidx = 0;

char s[5] = "23.73";  
// callback for sending data
void sendData(){
    
  if(cmdidx==1){
    Wire.write(s[idx++]);
    if(idx>3){idx=0;cmdidx++;}
  }
  if(cmdidx==0){
    Wire.write(0);//sid
    cmdidx++;
  }
  
  /*
  Wire.write(data[idx++]);
  if(idx > 3){
    idx = 0;  
  }*/
  
  //Wire.write(i2cdata,4);
  
  //Wire.write(0);
  //Wire.write(33);
  /*delay(100);
  Wire.write(1);
  Wire.write(998);
  delay(100);
  Wire.write(2); //sid for motion
  Wire.write(0); //code for motion detected
  delay(100);
  Wire.write(2);
  Wire.write(1); //code for door opened
  */
}
