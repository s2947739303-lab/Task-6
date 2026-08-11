#include "DHT.h"
#include "math.h" //include this header

#define DHTPIN 23
#define DHTTYPE DHT22

#define THERMISTORPIN 34 //define this pin

DHT dht(DHTPIN,DHTTYPE);

//Circuit Constants
const float SeriesResistor=10000; //10k resistor
const float NominalResistance=10000; //resistance at 25 deg C
const float NominalTemperature=25.0;
const float BCoefficient=3950.0; //should be from spec sheet
const float ADCMax=4095.0; //from esp32 ADC resolution
 
void setup() {
  // put your setup code here, to run once:

 Serial.begin(115200);
 //Serial.println("initializing");
 dht.begin();

 //analogSetCycles(8); //ESP32 ADC reading cycles

}

void loop() {
  // put your main code here, to run repeatedly:

 delay(2000);

 float humidity=dht.readHumidity();
 float temperature=dht.readTemperature();

 float AdcVal = analogRead(THERMISTORPIN);

 if(AdcVal >=ADCMax) AdcVal = ADCMax - 1;
 if(AdcVal <=0) AdcVal=1;
 //to avoid issues with  /0 - NAN

 //Thermistor Resistance value calculation

 float resistance = SeriesResistor / ((ADCMax/AdcVal)-1.0);

 //Steinhart-Hart Equation - resistance to Kelvin - degree

 float steinhart;
 steinhart=resistance / NominalResistance;
 steinhart=log(steinhart);
 steinhart /= BCoefficient;
 steinhart += 1.0/(NominalTemperature+273.15);
 steinhart = 1.0/steinhart;

 float tempC=steinhart - 273.15;

 if(!isnan(humidity) && !isnan(temperature))
 {
  //Serial.println("not reading data from DHT 22");
  Serial.print(humidity,2);
  Serial.print(",");
  Serial.print(temperature,2);
  Serial.print(",");
  Serial.println(tempC,2);
 }

//  Serial.println("Humidity in %:");
//  Serial.println(humidity);
//  Serial.print("Temperature in deg C:");
//  Serial.println(temperature);

 

//  Serial.print("Thermistor Temp in deg C:");
//  Serial.println(tempC);

// delay(1000)


}
