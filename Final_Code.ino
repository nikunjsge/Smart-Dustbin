#include<Servo.h>



/* GPS */
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial GPS(RXPin, TXPin);
/*GPS END*/


SoftwareSerial mySerial(8, 9);//Rx=8, Tx=9

Servo srv;
long duration1;
int distance1;

long duration2;
int distance2;

#define echoPin1 5
#define trigPin1 6
#define echoPin2 10
#define trigPin2 11

void setup()
{
  Serial.begin(9600);
  GPS.begin(GPSBaud);
  mySerial.begin(9600);
  delay(1000);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  srv.attach(7);
  delay(1000);
  srv.write(0);
}

void loop()
{

  /*GPS LOCATION READ*/
  while (GPS.available() > 0) {
    gps.encode(GPS.read());
    if (gps.location.isUpdated()) {
      Serial.print("Latitude= ");
      Serial.print(gps.location.lat(), 6);
      Serial.print(" Longitude= ");
      Serial.println(gps.location.lng(), 6);
    }
  }

  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);

  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;

  Serial.print("Distance1: ");
  Serial.print(distance1);
  Serial.println(" cm");


  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  duration2 = pulseIn(echoPin2, HIGH);
  distance2 = duration2 * 0.034 / 2;

  Serial.print("Distance2: ");
  Serial.print(distance2);
  Serial.println(" cm");



  if (distance1 < 30)
  {
    srv.write(90);
    delay(1000);
  }
  else
  {
    srv.write(0);
    delay(1000);
  }

  if (distance2 < 5)
  {
    mySerial.println("AT");
    updateSerial();

    mySerial.println("AT+CMGF=1");
    updateSerial();
    mySerial.println("AT+CMGS=\"+91xxxxxxxxxx\"");
    updateSerial();
    mySerial.print("Dustbin is full | Latitude= " && (gps.location.lat(), 6) && "Longitude= " && (gps.location.lng(), 6));
    updateSerial();
    mySerial.write(26);
  }

}

void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
