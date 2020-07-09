//Transmitter Code
#include <elapsedMillis.h>
#define DELAY     6
const unsigned long INTERVAL = 10000;
int i=0;
elapsedMicros timer0;

void setup() {
  int transPin = 8;
  int baudrate = 9600; 
  pinMode(transPin, OUTPUT);//light output port
  
  Serial.begin(baudrate);
  Serial.print("Transmission Pin: ");
  Serial.println(transPin);
  char fileName[] = "m1"; 
  delay(10);
  Serial.println("Sending...");
}
int  sendFile (int input)
{
   //int lengthName =strlen(input);
   //sendValue((byte)lengthName);
   //for(i=0;i<lengthName;i++)
   sendValue(input);
   return 0; 
}
void loop() {
  char data[] = "data1";
  int mode = 5;
  
  sendValue(mode);
  Serial.println("data Sent!");
  
  delay(50);                 //data 하나 보내고 대기
}
void sendValue(int valToSend)
{
  //Send start bit
  digitalWrite(8, HIGH);          //LED OFF
  timer0 = 0;
  byte b;
  //Send actual value
  delay(DELAY);
  for(int i = 0; i < 3; i++)
  {
      b = valToSend%2;
      valToSend=valToSend/2;
      //Delay until time to send
      //delay(20);//while(timer0 < INTERVAL){}
      if(b==0)
      {
        digitalWrite(8, HIGH);  //LED OFF
      }
      else
      {
        digitalWrite(8, LOW);   //LED ON
      }
      delay(DELAY);
      timer0 = 0;
  }
  //delay(20);//while(timer0< INTERVAL){}//Busy wait on last bit
  //Send stop bit
  digitalWrite(8, LOW);
  timer0 = 0;
  delay(DELAY);//while(timer0 < INTERVAL)
  {
  }//Delay on stop bit
}
