#include<SoftwareSerial.h>
int sensor;
int Light_status=10;
int threshold=45;

SoftwareSerial esp8266(3,4);

#define SSID "Bhide"
#define PASS "Shriparshuram1008"

String sendAT(String command, const int timeout)
{
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ((time + timeout) > millis())
  {
    while (esp8266.available())
    {
      char c = esp8266.read();
      response += c;
    }
  }
  Serial.print(response);
  return response;
}

void connectwifi()
{
  sendAT("AT\r\n", 1000);
  sendAT("AT+CWMODE=1\r\n", 1000); 
  sendAT("AT+CWJAP=\""SSID"\",\""PASS"\"\r\n", 2000); 
  while(!esp8266.find("OK")) 
  { 
  } 
  sendAT("AT+CIFSR\r\n", 1000); 
  sendAT("AT+CIPMUX=0\r\n", 1000); 
}

void setup() 
{
  Serial.begin(9600);
  esp8266.begin(9600);
  sendAT("AT+RST\r\n", 2000); 
  connectwifi();
  pinMode(8,OUTPUT);
}

void loop() 
{
  sensor=analogRead(A0);
  sensor=map(sensor,0,1023,0,100);

  if(sensor<threshold)
  {
    digitalWrite(8,HIGH);
    Light_status=100;
  }
  else
  {
    digitalWrite(8,LOW);
    Light_status=10;
  }
  String sensor_value=String(sensor);
  Serial.print("Light Intensity:");
  Serial.println(sensor);

  String Light_status1=String(Light_status);
  Serial.print("Light Status:");
  Serial.println(Light_status);

  String threshold1=String(threshold);
  Serial.print("Light Intensity:");
  Serial.println(threshold);

  updateTS(sensor_value,Light_status1,threshold1);
  delay(3000);
}

void updateTS(String S,String L,String T)
{
  Serial.println("");
  sendAT("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n", 1000);    
  delay(2000);
  String cmdlen;
  String cmd="GET /update?key=R5YK1PYDDJYMU1T4&field1="+S+"&field2="+L+"&field3="+T+"\r\n"; 
  cmdlen = cmd.length();
  sendAT("AT+CIPSEND="+cmdlen+"\r\n", 2000);
  esp8266.print(cmd);
  Serial.println("");
  sendAT("AT+CIPCLOSE\r\n", 2000);
  Serial.println("");
  delay(1000);
}
