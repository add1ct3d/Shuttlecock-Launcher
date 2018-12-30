#include <Servo.h>
#include <SoftwareSerial.h>
#define swit1 A0
#define swit2 A1
#define IN1 8
#define IN2 9
SoftwareSerial ser(2, 3); 
Servo servoKanan;          // Define left servo
Servo servoKiri;         // Define right servo

void setup() 
{
pinMode (swit1, INPUT_PULLUP);
pinMode (swit2, INPUT_PULLUP);
pinMode (IN1 , OUTPUT);
pinMode (IN2 , OUTPUT);
Serial.begin(115200);
ser.begin(9600);
servoKanan.attach(12);  // Set left servo to digital pin 10
servoKiri.attach(13);  // Set right servo to digital pin 9
servoBuka();
delay(100);

} 
String dataTerima;
String Kondisi = "NAIK";
void loop() 
{
 int s1=digitalRead (swit1);
 int s2=digitalRead (swit2);  
 if(ser.available()>0)
 {
  dataTerima = ser.readStringUntil('\n');
 }
 Serial.print(Kondisi);Serial.println("\t");
 Serial.println(dataTerima);
 if (dataTerima.substring(0,1) == "S")
 {  
   //Serial.println(Kondisi);
   if (s1 == LOW )
   {
    servoTutup();
    Kondisi = "TURUN";  
   }
   if (s2 == LOW)
   {
   servoBuka();
    Kondisi = "NAIK";
   }
   
   if (Kondisi == "NAIK")
   {
    motorNaik();
   }
   if (Kondisi == "TURUN")
   {
    motorTurun();
   }
 }
 else if (dataTerima.substring(0,1) == "T")
 {
  Stop();
 }
} 


void motorNaik()
{
 digitalWrite(IN1, HIGH);
 digitalWrite(IN2, LOW);
}

void motorTurun()
{

 digitalWrite(IN1, LOW);
 digitalWrite(IN2, HIGH);
}

void Stop()
{
 digitalWrite(IN1, LOW);
 digitalWrite(IN2, LOW);
 servoTutup();
}

void servoBuka()
{
  servoKanan.write(45);
  servoKiri.write(5);
  delay(100);
}
void servoTutup()
{
  servoKanan.write(5);
  servoKiri.write(45);
  delay(100);
}


    

