//temperature sensor
#include "DHT.h"

#define DPIN 4        //Pin to connect DHT sensor (GPIO number) D2
#define DTYPE DHT11   // Define DHT 11 or DHT22 sensor type
const int sensorIn = A0;
int mVperAmp = 185; // use 185 for 5A, 100 for 20A Module and 66 for 30A Module

//current sensor
double Voltage = 0;
double VRMS = 0;
double AmpsRMS = 0;

DHT dht(DPIN,DTYPE);

void setup(){ 
      //current sesnor
      pinMode(A0, INPUT);
      Serial.begin(115200);
      delay(10);
      Serial.println(F("Init...."));

      //temp sensor
      Serial.begin(9600);
      dht.begin();

}

void loop(){
 Voltage = getVPP();
 VRMS = (Voltage/2.0) *0.707; // sq root
 AmpsRMS = (VRMS * 1000)/mVperAmp;
 float Wattage = (220*AmpsRMS)-18; //Observed 18-20 Watt when no load was connected, so substracting offset value to get real consumption.
 Serial.print(AmpsRMS);
 Serial.println(" Amps RMS ");
 Serial.print(Wattage); 
 Serial.println(" Watt ");
}

float getVPP()
{ //current sensor
  float result;
  
  int readValue;             //value read from the sensor
  int maxValue = 0;          // store max value here
  int minValue = 1024;          // store min value here
  
   uint32_t start_time = millis();

   while((millis()-start_time) < 1000) //sample for 1 Sec
   {
       readValue = analogRead(sensorIn);
       // see if you have a new maxValue
       if (readValue > maxValue) 
       {
           /*record the maximum sensor value*/
           maxValue = readValue;
       }
       if (readValue < minValue) 
       {
           /*record the maximum sensor value*/
           minValue = readValue;
       }
/*       Serial.print(readValue);
       Serial.println(" readValue ");
       Serial.print(maxValue);
       Serial.println(" maxValue ");
       Serial.print(minValue);
       Serial.println(" minValue ");
       delay(1000); */
    }
   
   // Subtract min from max
   result = ((maxValue - minValue) * 5)/1024.0;
   
   

   //temp sensor
  delay(2000);
  
  float tc = dht.readTemperature(false);  //Read temperature in C
  float tf = dht.readTemperature(true);   //Read Temperature in F
  float hu = dht.readHumidity();          //Read Humidity

  Serial.print("Temp: ");
  Serial.print(tc);
  Serial.print(" C, ");
  Serial.print(tf);
  Serial.print(" F, Hum: ");
  Serial.print(hu);
  Serial.println("%");

  return result;
 }

