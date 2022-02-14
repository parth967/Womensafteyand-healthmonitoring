#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include "TinyGPS++.h"
#include <SoftwareSerial.h>
TinyGPSPlus gps;
//Health Monitoring
int op,heartloop;
//for body temperature
int val;
int tempPin = 1;
//for heart beat rate
const int PulseWire = 0; // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13; // The on-board Arduino LED, close to PIN 13.
int Threshold = 550; // Determine which Signal to "count as a beat" and which to ignore.
PulseSensorPlayground pulseSensor; // Creates an instance of the PulseSensorPlayground object called "pulseSensor"


//Location Tracking
SoftwareSerial serial_connection(10, 11); //RX=pin 11, TX=pin 10
float longi;
float latii;//This is the GPS object that will pretty much do all the grunt work with the NMEA data

//Sms Send
SoftwareSerial mySerial(8, 9);//RX=pin 9 ,Tx= in 8
char msg;
char call;

void setup() {
  mySerial.begin(9600);   // Setting the baud rate of GSM Module  
  Serial.begin(9600);
  serial_connection.begin(9600);
  pinMode(13, OUTPUT);//This opens up communications to the GPS
  Serial.println("a: Health Monitoring");
  
  Serial.println("b: For Send location");
  Serial.println("    GSM SIM900A BEGIN");
  Serial.println("   Enter character for control option:");
  Serial.println("   h : to disconnect a call");
  Serial.println("   i : to receive a call");
  Serial.println("   s : to send message");
  Serial.println("   c : to make a call");  
  Serial.println("   e : to redial");

  Serial.println("c: Only Sms");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    switch(Serial.read())
    {
     case 'a':
     Serial.println("a");
     health();
     break;
     case 'b':
     Serial.println("b");
     track_sms();
     break;
     case 'c':
     Serial.println("c");
     sms();
     break;
     default:break;     
    }
  }

}

void health(){
  for(;;){
  if(Serial.available()>0)
   {     
      char data= Serial.read(); // reading the data received from the bluetooth module
      switch(data)
      {
        case 'a': digitalWrite(13, HIGH);
        temp();
        break; // when a is pressed on the app on your smart phone
        case 'b': digitalWrite(13, LOW);
        heart();
        break; // when d is pressed on the app on your smart phone
        default : break;
      }
      data=' ';
      Serial.println(data);
   }
   delay(50);
  }
}
void temp()
{
  float mv,cel,farh;
  for(heartloop=0;heartloop<5;heartloop++){
  val = analogRead(tempPin);
  mv = ( val / 1024.0) * 5000;
  cel = mv / 10;
  farh = (cel * 9) / 5 + 32;}

  Serial.print("TEMPERATURE :          ");
  Serial.print(cel);
  Serial.print(" *C ");
  Serial.print(farh);
  Serial.print(" *F");
}
void heart ()
{int loopvar=0;
pulseSensor.analogInput(PulseWire);
pulseSensor.blinkOnPulse(LED13); //auto-magically blink Arduino's LED with heartbeat.
pulseSensor.setThreshold(Threshold);

// Double-check the "pulseSensor" object was created and "began" seeing a signal.
if (pulseSensor.begin()) {

}
for(loopvar=0;loopvar<=100;loopvar++){
int myBPM = pulseSensor.getBeatsPerMinute(); // Calls function on our pulseSensor object that returns BPM as an "int".
// "myBPM" hold this BPM value now.
if (pulseSensor.sawStartOfBeat()) { // Constantly test to see if "a beat happened". // If test is "true", print a message "a heartbeat happened".
Serial.print("BPM: "); // Print phrase "BPM: "
Serial.println(myBPM); // Print the value inside of myBPM.
//lcd.setCursor(0,2);// If test is "true", print a message "a heartbeat happened".
//lcd.setCursor(5,3);
Serial.print("BPM: "); // Print phrase "BPM: "
Serial.print(myBPM); 
}
delay(20); // considered best practice in a simple sketch.

  }
}


void track_sms(){
  Serial.print("b selected");
  for(;;){
   if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage();
      break;
    case 'c':
      MakeCall();
      break;
    default: break;  
  }
 if (mySerial.available()>0)
 Serial.write(mySerial.read());
  }
 }
 void SendMessage()
{
   while(serial_connection.available())//While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.println("Latitude:");
    latii=gps.location.lat();
    Serial.println(gps.location.lat(), 6);
    Serial.println("Longitude:");
    longi=gps.location.lng();
    Serial.println(gps.location.lng(), 6);
    Serial.println("Speed MPH:");
    Serial.println(gps.speed.mph());
    Serial.println("Altitude Feet:");
    Serial.println(gps.altitude.feet());
    Serial.println("");
  }
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"+917405131728\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println(latii);
  mySerial.println(longi);// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}

void MakeCall()
{
  mySerial.println("ATD+917405131728;"); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}



void sms(){
  Serial.print("C selected");
    for(;;){
   if (Serial.available()>0)
   switch(Serial.read())
  {
    case 's':
      SendMessage_num();
      break;
    case 'c':
      MakeCall_num();
      break;
      default: break;
  }
 if (mySerial.available()>0)
 Serial.write(mySerial.read());
  }b
}
void SendMessage_num()
{
  mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  mySerial.println("AT+CMGS=\"\"\r"); // Replace x with mobile number
  delay(1000);
  mySerial.println("sim900a sms");// The SMS text you want to send
  delay(100);
   mySerial.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
}
void MakeCall_num()
{
  mySerial.println(""); // ATDxxxxxxxxxx; -- watch out here for semicolon at the end!!
  Serial.println("Calling  "); // print response over serial port
  delay(1000);
}

