int maxPeople = 2; // maximum number of people allowed before the alarm goes off
int sensitivity = 2;

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


char auth[] = "-_gi-s2ELS_ELp2eX77uWUlaFf64ni6q";
int currentPeople = 0;
int buzzer = D8;

char ssid[] = "pp";
char pass[] = "password";

WidgetLCD lcd(V1);

int sensor1[] = {D5,D4};
int sensor2[] = {D7,D6};

int sensor1Initial;
int sensor2Initial;

String sequence = "";
int timeoutCounter = 0;

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT);
  
  Blynk.begin(auth, ssid, pass);

  lcd.clear(); 
  lcd.print(0, 0, "No of people: "); 

  delay(500);
  sensor1Initial = measureDistance(sensor1);
  sensor2Initial = measureDistance(sensor2);
}

void loop()
{
  lcd.clear();
  lcd.print(0, 0, "No of people: "); 

  int sensor1Val = measureDistance(sensor1);
  int sensor2Val = measureDistance(sensor2);

  //Process the data
  if(sensor1Val < sensor1Initial - 30 && sequence.charAt(0) != '1'){
    sequence += "1";
  }else if(sensor2Val < sensor2Initial - 30 && sequence.charAt(0) != '2'){
    sequence += "2";
  }
  
  if(sequence.equals("12")){
    currentPeople++;  
    sequence="";
    delay(3000);
  }else if(sequence.equals("21") && currentPeople > 0){
    currentPeople--;  
    sequence="";
    delay(3000);
  }
  
  if(sequence.length() > 2 || sequence.equals("11") || sequence.equals("22") || timeoutCounter > 200){
    sequence="";  
  }

  if(sequence.length() == 1){ //
    timeoutCounter++;
  }else{
    timeoutCounter=0;
  }

  Serial.print("Seq: ");
  Serial.print(sequence);
  Serial.print(" S1: ");
  Serial.print(sensor1Val);
  Serial.print(" S2: ");
  Serial.println(sensor2Val);
  Serial.print("Current people:");
  Serial.println(currentPeople);

  if(currentPeople > maxPeople){
    tone(buzzer, 1700);  
  }else{
    noTone(buzzer);  
  }
  
  lcd.print(7, 1, currentPeople);
  Blynk.run();
}

//Returns the distance of the ultrasonic sensor that is passed in
//a[0] = echo, a[1] = trig
int measureDistance(int a[]) {
  pinMode(a[1], OUTPUT);
  digitalWrite(a[1], LOW);
  
  delayMicroseconds(2);
  digitalWrite(a[1], HIGH);
  
  delayMicroseconds(10);
  digitalWrite(a[1], LOW);
  
  pinMode(a[0], INPUT);
  
  long duration = pulseIn(a[0], HIGH, 100000);
  return duration / 29 / 2;
}
