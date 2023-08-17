//NICEEEEE 

#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>
#include <SD.h>
#include <SPI.h>
#include <Servo.h>


virtuabotixRTC myRTC(6, 7, 8); //Initialize rtc

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C address 0x3F, 16 column and 2 rows

Servo s1, s2;
#define ADC_VREF_mV    5000.0 // in millivolt
#define ADC_RESOLUTION 1024.0
#define I2C_ADDR 0x27 


File myFile; //--> File Initialization
const int chipSelect = 4; //--> chipSelect (CS) Pin
const int sensor_pin = A0;
float temp;  
float output;

void setup() {
  Serial.begin(9600);
    s1.attach(9);
    s2.attach(5);

  pinMode(sensor_pin,INPUT);
  pinMode(chipSelect, OUTPUT);
   myRTC.setDS1302Time(18, 44, 11, 3, 14, 2, 2023); 
  // SD Card Initialization
if (!SD.begin(chipSelect)) {
    Serial.println("Initialization failed!");
    while (1);
  }

  Serial.println("Successfully Initializing Micro SD Card");
  Serial.println();
  delay(100);

  lcd.init();      // initialize the lcd
  lcd.backlight(); // open the backlight 

}

void loop() {
  myRTC.updateTime();
  delay(1000);
  
  
  // get the ADC value from the LM35 temperature sensor
  int adcVal = analogRead(sensor_pin);
  // convert the ADC value to voltage in millivolt
  float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
  // convert the voltage to the temperature in Celsius
  float temp = milliVolt / 10;
  lcd.clear();

  
  Serial.print("Current date/Time: ");
  Serial.print(myRTC.dayofmonth);
  lcd.setCursor(0,0);
  lcd.print(myRTC.dayofmonth);
  Serial.print('/');
  lcd.setCursor(2,0);
  lcd.print("/");
  Serial.print(myRTC.month);
  lcd.setCursor(3,0);
  lcd.print(myRTC.month);
  Serial.print('/');
  lcd.setCursor(4,0);
  lcd.print("/");
  Serial.print(myRTC.year);
  lcd.setCursor(5,0);
  lcd.print(myRTC.year);
  Serial.print(" ");

  Serial.print(myRTC.hours);
  lcd.setCursor(10,0);
  lcd.print(myRTC.hours);
  Serial.print(':');
  lcd.setCursor(12,0);
  lcd.print(":");
  Serial.print(myRTC.minutes);
  lcd.setCursor(13,0);
  lcd.print(myRTC.minutes);
  Serial.print(':');
  Serial.print(myRTC.seconds);
     
  Serial.print(" Temperature: ");
  lcd.setCursor(0,1);
  lcd.print("T:");
  Serial.println(temp);
  lcd.print(temp);
  lcd.print((char)223);
  lcd.print("C");

 
  myFile = SD.open("datalog.txt", FILE_WRITE);
  if (myFile) {    
    Serial.println("Writing to datalog.txt...");
        
        //----------------------------------------------Writing Date Data to Micro SD Card
        myFile.print("Date:");
        myFile.print(myRTC.dayofmonth);
        myFile.print("-");
        myFile.print(myRTC.month);
        myFile.print("-");
        myFile.print(myRTC.year);
        //----------------------------------------------
        
        myFile.print(", "); //--> Write the delimiter between the data in the form of a comma character to the Micro SD Card

        //----------------------------------------------Writing Time Data to Micro SD Card
        myFile.print("Time:");
        myFile.print(myRTC.hours);
        myFile.print(":");
        myFile.print(myRTC.minutes);
        myFile.print(":");
        myFile.print(myRTC.seconds);
        //----------------------------------------------
        myFile.print(", ");
        myFile.print("Temp:");       
        myFile.print(temp); //--> Writing Temperature Data to Micro SD Card
       if(temp >= 30)
         {
          s1.write(180);
          Serial.println("Open Window");
          lcd.setCursor(11,1);
          lcd.print("Open");
          myFile.print("Open Window");
          s2.write(180);
          Serial.println("Open Window");
//          lcd.setCursor(11,1);
//          lcd.print("Open");
          myFile.print("Open Window");
         }else{
          s1.write(90);
          Serial.println("Close Window");
          lcd.setCursor(10,1);
          lcd.print("Close");
          myFile.print(" Close Window");
          s2.write(90);
          Serial.println("Close Window");
//          lcd.setCursor(10,1);
//          lcd.print("Close");
          myFile.print(" Close Window");

         }
          
        myFile.close(); //--> close the file
        
        Serial.println("Successfully writing Data to Micro SD Card");
        Serial.println("----");
        Serial.println();
  }
  // if the file didn't open, print an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(3000);
}
