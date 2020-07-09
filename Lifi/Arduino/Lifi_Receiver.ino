#include <elapsedMillis.h>

unsigned long INTERVAL  = 10000;
const unsigned long HALF = INTERVAL/2;
const int dsize = 8;
elapsedMicros timer0;
long threshold = 0; //To determine 0s and 1s
bool moveToNextBit = false;
const int precision = 1000;
int sensorPin = A0; //pin for ldr
int sensorValue = 0; //Value read in by sensor

void setup() {
    Serial.begin(9600);
    //Calibrate 1 and 0 threshhold
    for(int i = 0; i < precision; i++)
    {
      threshold += analogRead(sensorPin);
    }
    threshold = threshold / precision;
    threshold = threshold + 100;
    Serial.print("Threshold: ");
    Serial.println(threshold);
    
    //Get file size
    byte buf[4];
    for(int i = 0; i < 4; i++)
    {
      buf[i] = getValue();
    }
    /*unsigned long fileSize = buf[0] | (buf[1] << 8) | ((unsigned long )buf[2] << 16) | ((unsigned long)buf[3] << 24);
    Serial.print("File Size: ");
    Serial.println(fileSize);*/   
}
        

void loop() {
  while(1){
    Serial.println(getValue());
  }
}
int getFile()
{ 
    byte fileNameSize = getValue();
    Serial.print("File Name Size: ");
    Serial.println(fileNameSize);
    if(fileNameSize == 0) //Would cause for loop to never end
    {
      //주행모드
    }
    else if(fileNameSize<7){
      Serial.print("File Name: ");
      char fname[] = "";
      for(byte i = 0; i < fileNameSize; i++)
      {
        fname[i] = getValue();
        Serial.print(fname[i]);
      }
      fname[fileNameSize] = '\0'; //Add null terminator
      Serial.println();
      Serial.println("Done!");
    }
}

byte getValue()
{
  unsigned long interval = INTERVAL;
  int d[dsize]; //array to store incoming 16 bit values
  bool startBitReceived = false;
  while(!startBitReceived)
      {
        sensorValue = analogRead(sensorPin); //Takes 100 microseconds
        if(sensorValue < threshold) //If we detected start bit
        {
          timer0 = 0;
          startBitReceived = true;     
        }
      }

      //Just delay here until we get to the middle of the start bit
      delay(3);//while(timer0 < HALF){}
      timer0 = 0;

      //Get bits based on timer0
      for(unsigned long i = 0; i < 3; i++)
      {
        while(!moveToNextBit)
        {
          delay(6);
          d[i] = analogRead(sensorPin);
          moveToNextBit = true;
          /*if(INTERVAL - timer0 < 10)
          {
            d[i] = analogRead(sensorPin);
            timer0 = 0;
            moveToNextBit = true;
            interval = interval - 100;  //interval = interval - 100;
          }*/
        }  
        moveToNextBit = false;
      }

      byte recVal = convertToDecimal(d);
      
      //Busy wait on stop bit
      timer0 = 0;
      delay(6);//while(timer0 < INTERVAL){}
      return recVal;
}
byte convertToDecimal(int arr[dsize])
{
  int j = dsize;
  byte n;
  byte t = 1;
  byte rec = 0;
  while(j >= 0)
  {
    if(arr[j] > threshold) // Data = 1;
    {
      n = j;
      while(n != 0)
      {
        t=t*2;
        n--; 
      }
      rec = rec + t;
      t = 1;
    }  
    j--;
  }
  return rec;
}
