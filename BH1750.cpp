#include <Wire.h>
#include <BH1750_WE.h>
#define BH1750_ADDRESS 0x23

BH1750_WE myBH1750 = BH1750_WE(BH1750_ADDRESS); 
// You may also pass a TwoWire object: 
//BH1750_WE myBH1750 = BH1750_WE(&Wire, BH1750_ADDRESS);
// If you don't pass any parameter, Wire and 0x23 will be applied

void setup(){
  Serial.begin(9600);
  Wire.begin();
  if(!myBH1750.init()){ // sets default values: mode = CHM, measuring time factor = 1.0
    Serial.println("Connection to the BH1750 failed");
    Serial.println("Check wiring and I2C address");
    while(1){}
  }
  else{
    Serial.println("BH1750 is connected");
  }
  // myBH1750.setMeasuringTimeFactor(0.45); // uncomment for selection of value between 0.45 and 3.68
}

void loop(){ 
  myBH1750.setMode(OTH); // sets mode and starts measurement
  /* An OTH and OTH_2 measurement takes ~120 ms. I suggest to wait 
     140 ms to be on the safe side. 
     An OTL measurement takes about 16 ms. I suggest to wait 20 ms
     to be on the safe side. */
  delay(140); // wait for measurement to be completed, change for OTL
  float lightIntensity = myBH1750.getLux();
  Serial.print(F("Light intensity: "));
  Serial.print(lightIntensity);
  Serial.println(F(" Lux"));
  delay(1000);
}