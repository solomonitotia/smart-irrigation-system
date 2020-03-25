
#include <SPI.h>// library for SPI communication (pre-loaded in arduino)
#include <dht.h> //Library for dht11 temperature and humidity sensor
int soggyLEDPin=9;
int moistsoilLEDPin=6;
int drysoilLEDPin=5;
int PumpPin=3;
int moistureSensorValue;
int a   =800;
int chipSelect=4;
File myFile;
dht DHT; //sensor object named as DHT
#define DHT11_PIN 7//sensor output pin connected to pin 7
DS3231  rtc(SDA, SCL);
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Initialize_SDcard();
  Initialize_RTC();
  Initialize_PlxDaq();
  pinMode(soggyLEDPin, OUTPUT);
  pinMode (moistsoilLEDPin, OUTPUT);
  pinMode(drysoilLEDPin, OUTPUT);
  pinMode(PumpPin, OUTPUT);
  //Serial.println("Type,\tStatus,\tHumidity(%),\tTemperature(C)");
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
//DHTAcq();
//sdCardWrite("test3.txt");
//sdCardRead("test3.txt");

}
void Initialize_SDcard()
{
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
   // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("LoggerCD.txt", FILE_WRITE);
  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println("Date,Time,Temperature,Humidity"); //Write the first row of the excel file
    Serial.print(",\t");
    dataFile.close();
  }
}

void Initialize_RTC()
{
   // Initialize the rtc object
  rtc.begin();

//#### The following lines can be uncommented to set the date and time for the first time###  
/*
rtc.setDOW(FRIDAY);     // Set Day-of-Week to SUNDAY
rtc.setTime(18, 46, 45);     // Set the time to 12:00:00 (24hr format)
rtc.setDate(2, 26, 2019);   // Set the date to feb 26th, 2019 
*/
}
void Initialize_PlxDaq()
{
Serial.println("CLEARDATA"); //clears up any data left from previousprojects
//Serial.print(",\t");
Serial.println("LABEL,__Date,___Time,___Temp,__Humidity");//always write LABEL, to indicate it as first line
//Serial.println("LABEL");
//Serial.println("DATE");

//Serial.print(",\t");
//Serial.print(",\t");
}
/*void DHTAcq()
  {
    Serial.println("DHT11, \t");
    int chk = DHT.read11(DHT11_PIN);
    
    Serial.print(DHT.humidity, 1);
    Serial.print(",\t");
    Serial.print(DHT.temperature, 1);
    delay(1000);  
  }

void sdCardWrite(String fileNameStr )
  {
      Serial.println("Initializing SD card");
  
    if (!SD.begin(4)) 
      {
        Serial.println( "Initilization failed.");
        return;
      }
    Serial.println("Initilization done!");
  
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    myFile = SD.open(fileNameStr, FILE_WRITE);
  
    // if the file opened okay, write to it:
    if (myFile) 
      {
       /* myFile.println("DHT11, \t");
        int chk = DHT.read11(DHT11_PIN);
        
        myFile.print(DHT.humidity, 1);
        myFile.print(",\t");
        myFile.print(DHT.temperature, 1);
        myFile.close();
        Serial.println("done!");
        delay(5000);*/
       
       
     /* } 
    else 
      {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
      }
  }*/

 /* void sdCardRead(String fileName)
  {
     // re-open the file for reading:
    myFile = SD.open(fileName);
    if (myFile) 
      {
        Serial.println("test.txt:");
    
        // read from the file until there's nothing else in it:
        while (myFile.available())
          {
            Serial.write(myFile.read());
          }
        // close the file:
        myFile.close();
      } 
    else 
      {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
      }
  }*/

void loop() {
moistureSensorValue = analogRead(3); // reads the sensor
 if(moistureSensorValue < 300){
 
 //LED for Soggy lights up
 digitalWrite(drysoilLEDPin,HIGH);
 digitalWrite(moistsoilLEDPin,LOW);
 digitalWrite(PumpPin,LOW);
 digitalWrite(soggyLEDPin,LOW);


 Serial.println("DONE WATERING");
 } // in case of moist soil:
 if((moistureSensorValue < 800) && (moistureSensorValue >300)){
 

 //LED for moist lights up
 digitalWrite(drysoilLEDPin,HIGH);
 digitalWrite(moistsoilLEDPin,LOW);
 digitalWrite(soggyLEDPin,HIGH);
 digitalWrite(PumpPin,HIGH);
 Serial.println("Moisture ok");
 } //in case of dry soil:
 if(moistureSensorValue > 800){
 //LED for DRY lights up
 digitalWrite(drysoilLEDPin,LOW);
 digitalWrite(moistsoilLEDPin,HIGH);
 digitalWrite(soggyLEDPin,LOW);
 digitalWrite(PumpPin,HIGH);
 
 Serial.println(" WATERING");}
 
 Read_DHT11();
  Write_SDcard();
  Write_PlxDaq();
  delay(5000);//wait for 5 sec before witting the next data
 }
 void Read_DHT11()
{
int chk = DHT.read11(DHT11_PIN);
}
void Write_SDcard()
{
    // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("LoggerCD.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.print(rtc.getDateStr()); //Store date on SD card
    //Serial.print(",\t");
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(rtc.getTimeStr()); //Store date on SD card
    //Serial.print(",\t");
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(DHT.temperature); //Store date on SD card
    //Serial.print(",\t");
    dataFile.print(","); //Move to next column using a ","

    dataFile.print(DHT.humidity); //Store date on SD card
    //Serial.print(",\t");
    dataFile.print(","); //Move to next column using a ","

    dataFile.println(); //End of Row move to next row
    //Serial.print(",\t");
    dataFile.close(); //Close the file
  }
  else
  Serial.println("OOPS!! SD card writing failed");
}

 void Write_PlxDaq()
  {
    Serial.print("DATA"); //always write "DATA" to Indicate the following as Data
    Serial.print(",\t");
   // Serial.print(","); //Move to next column using a ","

    Serial.print("DATE"); //Store date on Excel
    Serial.print(",\t");
    //Serial.print(","); //Move to next column using a ","

    Serial.print("TIME"); //Store date on Excel
    Serial.print(",\t");
    //Serial.print(","); //Move to next column using a ","

    Serial.print(DHT.temperature); //Store date on Excel
    Serial.print(",\t");
    //Serial.print(","); //Move to next column using a ","

    Serial.print(DHT.humidity); //Store date on Excel
    Serial.print(",\t");
    //Serial.print(","); //Move to next column using a ","

    Serial.println(); //End of Row move to next row
    Serial.print(",\t");
  }

 /*int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);
 delay (1000);*/
