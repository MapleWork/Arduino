/* Define shift register pins used for seven segment display */
/*https://swf.com.tw/?p=715*/
#define LATCH_DIO D15   
#define CLK_DIO D14
#define DATA_DIO D2
#define BUTTON1 BT1
#define BUTTON2 BT2
#define BUTTON3 BT3
#define BUTTON4 BT4
#define BUTTON_A1 A1
#define BUTTON_A2 A2
#define BUTTON_A3 A3

const byte SEGMENT_MAP[] = {0X7F,0x66,0x3F,0x79,0x3F,0X7F,0x3F,0x66};  //840E0804
const byte SEGMENT_SELECT[] = {0xFE,0xFD,0xFB,0xF7};
byte displaybuf[4];
void bufshift(byte dir, byte databyte)//dir=0:shift left, dir=1:shift right
{
  if(dir == 1) //0是左移，1是右移
  {
    for (int j=0; j<3; j++)
    {
      displaybuf[3-j]=displaybuf[3-j-1];
    }
    displaybuf[0]=databyte;
  }
  else
  {
    for (int j=0; j<3; j++)
    {
      displaybuf[j]=displaybuf[j+1];
    }
    displaybuf[3]=databyte;
  }
}
void setup ()
{
/* Set DIO pins to outputs */
Serial.begin(115200);
pinMode(LATCH_DIO,OUTPUT);
pinMode(CLK_DIO,OUTPUT);
pinMode(DATA_DIO,OUTPUT);
//clear data display buffer
for(int i=0; i<4; i++)
  displaybuf[i]=0;
}
byte cnt=0; 
/* Main program */
void loop()
{
  bufshift(0, SEGMENT_MAP[cnt++]);
  for(int i=0; i<4; i++)
  {
      Serial.print(displaybuf[3-i], HEX); Serial.print("\t");
  }
  Serial.println();
  unsigned long starttime=millis();
  while((millis()-starttime)<1500)  //速度快慢 
     show7seg();
  cnt %= 8;
}
//code from https://www.arduino.cc/reference/en/language/functions/advanced-io/shiftout/
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
  uint8_t i;
  for (i = 0; i < 8; i++)  
  {
    if (bitOrder == LSBFIRST)
      digitalWrite(dataPin, !!(val & (1 << i)));
    else
      digitalWrite(dataPin, !!(val & (1 << (7 - i))));
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
  }
}

/* Write a decimal number between 0 and 9 to one of the 4 digits of the display */
/*
void WriteNumberToSegment(byte Segment, byte Value)
{
digitalWrite(LATCH_DIO,LOW);
shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_MAP[Value]);
shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
digitalWrite(LATCH_DIO,HIGH);
}*/
void WriteNumberToSegment(byte Segment, byte Value)
{
digitalWrite(LATCH_DIO,LOW);
shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, Value);
shiftOut(DATA_DIO, CLK_DIO, MSBFIRST, SEGMENT_SELECT[Segment] );
digitalWrite(LATCH_DIO,HIGH);
}
void show7seg()
{
  int i;
  for(i=0; i<4; i++)
  {
    WriteNumberToSegment(i, displaybuf[3-i]);
    delay(2);
  }
}


