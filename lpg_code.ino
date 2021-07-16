#include<stdio.h>
#include<math.h>
#include <EEPROM.h>

  int address =0;
  double ppm=0;
  float ratio; //Define variable for ratio
  float sensor_volt; //Define variable for sensor voltage
  int callibrate_button = 8;
  int led = 9; //power LED pin when system gets on
  int buzzer = 10; //Buzzer pin
  int red_light_pin= 11;
  int green_light_pin = 12;
  int blue_light_pin = 13;
  int gas_sensor = A0; //Sensor pin
  float m = -0.42452; //Slope
  float c = 1.27; //Y-Intercept
  float RS_gas;
  float sensorValue=0.0;
  double R0=0,R0_offset=0;

void setup()
{
  Serial.begin(9600); //Baud rate
  pinMode(led, OUTPUT); //Set LED as output
  digitalWrite(led, LOW); //Turn LED off
  pinMode(buzzer, OUTPUT); //Set buzzer as output
  digitalWrite(buzzer, LOW); // Turn buzzer off
  pinMode(callibrate_button, INPUT); //Set callibrate_button as input
 
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  Serial.print("START DEVICE...\r\n");
  
    
  digitalWrite(led, HIGH); // turn the LED on (HIGH is the voltage level)
  Serial.print("POWER LED ON...\r\n");

 }
 
void loop()
{
  digitalWrite(13,HIGH);
   digitalWrite(12,LOW);
   digitalWrite(11,HIGH);
   
  if (digitalRead(callibrate_button))
  {
  digitalWrite(13,LOW);
  digitalWrite(12,HIGH);
  digitalWrite(11,HIGH);
   Serial.print("sensorValue: ");
   sensorValue = analogRead(gas_sensor);
   Serial.println(sensorValue);
   Serial.print("RS_gas: ");
   RS_gas = (1023/sensorValue-1)*22000;
   Serial.print("s: ");
   double s=log10(1000* (pow(RS_gas,2.38))/1076.46);
   Serial.println(s);
   Serial.print("n: ");
   double n=(s/2.38);
   Serial.println(n);
   R0=pow(10,n);
   Serial.println(R0);
   Serial.print("CALCULATION DONE FOR CLEAN AIR!! \r\n");
   delay(500);
   EEPROM.put(address, R0);
   Serial.print("STORE SENSOR VALUE IN EEPROM : \r\n");
   Serial.println(EEPROM.put(address, R0));
   delay(500);
  digitalWrite(13,HIGH);
  digitalWrite(12,HIGH);
  digitalWrite(11,HIGH);
  }
  //else
   
     R0_offset=EEPROM.get(address,R0);
     Serial.print("READ SENSOR VALUE IN EEPROM : \r\n");
     Serial.println(R0_offset);
     delay(1000);
    {
  
    Serial.print("CALCULATION START FOR LPG GAS!! \r\n");
    sensorValue = analogRead(gas_sensor); //Read analog values of sensor
    Serial.print("sensorValue: ");
    Serial.println(sensorValue);

    RS_gas = (1023/sensorValue-1)*22000; //Get value of RS(sensor resistance) in a gas
    Serial.print("RS_gas:");
    Serial.println(RS_gas);
 
    ratio = RS_gas/R0_offset;  // Get ratio RS_gas/RS_air
    Serial.print("ratio:");
    Serial.println(ratio);

    ppm = 1076.46*(pow(ratio,-2.38));//Get ppm value in linear scale according to the the ratio value
    Serial.print("ppm:");
    Serial.println(ppm);

   

    Serial.print("COMPARE PPM AND DETECT LPG!! \r\n");
    if((ppm>1010)&(ppm>1060))
   { 
    digitalWrite(buzzer, HIGH); //Turn buzzer on
    digitalWrite(13,HIGH);
    digitalWrite(12,HIGH);
    digitalWrite(11,LOW);
    delay(500);
  
      digitalWrite(13,HIGH);
      digitalWrite(12,HIGH);
      digitalWrite(11,HIGH);

   }
     else
     { 
      digitalWrite(buzzer, LOW); //Turn buzzer off  
     
     }
   }
}
/*
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}
*/
