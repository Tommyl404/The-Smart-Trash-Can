#include <RadioHead.h>
const int n = 2;
const int 
const int mySerial[log()] = {0,0};
const int RX = A0;
const int TX = 2;

const int startBit[] = {1,0};
const int stopBit[] = {0,1};

bool isMaster = true;

void setup() {
  pinMode(RX, INPUT);
  pinMode(TX, OUTPUT);
  for(int i=0;i<n;i++)
  {
    if(mySerial[n] != 0)
    {
      isMaster = false;
      break;
    }
  }
}

void loop() {
  if(isMaster)
    masterProcedure();
  else
    slaveProcedure();
}

void masterProcedure()
{
  typedef enum {checkStatus, getAnswer} states;
  bool parkings[n-1];
  const bool isAvailable = true;
  int currentParking = 1;
  states state = checkStatus;

  switch(state)
  {
    case checkStatus
      checkParking(currentParking);
      break;

    case getAnswer
      break;
  }
  
}

void checkParking(int currentParking)
{
  
  sendStart();
  sendSerial();
  sendStop();
}

void sendSerial()
{
  
}

void sendStart()
{
  
}

void sendStop()
{
  
}
void slaveProcedure()
{
  
}
