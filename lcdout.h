#include <Wire.h>
uint8_t lfl = 0;
void send(uint8_t data)
{
  Wire.beginTransmission(0x3F);
  Wire.write(data);
  Wire.endTransmission();
  
}
void lcd_init() 
{
  Wire.begin();
  delay(50);

  //send(0x08); //optional
  //delay(1000);

 //send 0x30
 send(0x38);
send(0x3C);
 delayMicroseconds(1);
send(0x38);
delayMicroseconds(50);

delayMicroseconds(4500);

//send 0x03
 send(0x38);
send(0x3C);
 delayMicroseconds(1);
send(0x38);
delayMicroseconds(50);

delayMicroseconds(4500);

//send 0x30
 send(0x38);
send(0x3C);
 delayMicroseconds(1);
send(0x38);
delayMicroseconds(50);

delayMicroseconds(150);

//send 0x02
 send(0x28);
send(0x2C);
 delayMicroseconds(1);
send(0x28);
delayMicroseconds(50);

//send 0x20
 send(0x28);
send(0x2C);
 delayMicroseconds(1);
send(0x28);
delayMicroseconds(50);

//send 0x08
 send(0x88);
send(0x8C);
 delayMicroseconds(1);
send(0x88);
delayMicroseconds(50);

//send 0x00
 send(0x08);
send(0x0C);
 delayMicroseconds(1);
send(0x08);
delayMicroseconds(50);

//send 0x0C
 send(0xC8);
send(0xCC);
 delayMicroseconds(1);
send(0xC8);
delayMicroseconds(50);


//send 0x00
 send(0x08);
send(0x0C);
 delayMicroseconds(1);
send(0x08);
delayMicroseconds(50);

//send 0x01
 send(0x18);
send(0x1C);
 delayMicroseconds(1);
send(0x18);
delayMicroseconds(50);


delayMicroseconds(2000);

//send 0x00
 send(0x08);
send(0x0C);
 delayMicroseconds(1);
send(0x08);
delayMicroseconds(50);

//send 0x06
 send(0x68);
send(0x6C);
 delayMicroseconds(1);
send(0x68);
delayMicroseconds(50);
}

void clear()
{
//send 0x00
 send(0x08);
send(0x0C);
 delayMicroseconds(1);
send(0x08);
delayMicroseconds(50);

//send 0x01
 send(0x18);
send(0x1C);
 delayMicroseconds(1);
send(0x18);
delayMicroseconds(50);

delayMicroseconds(2000);

}


void pos(uint8_t row,uint8_t col)
{
   if(row == 0)
   {
     send(0x88);
  send(0x8C);
  delayMicroseconds(1);
  send(0x88);
  delayMicroseconds(50);

     send((col<<4)|0x08);
  send((col<<4)|0x0C);
   delayMicroseconds(1);
  send((col<<4)|0x08);
  delayMicroseconds(50);

  delayMicroseconds(2000);
  }

  else if(row == 1)
   {
     send(0xC8);
  send(0xCC);
  delayMicroseconds(1);
  send(0xC8);
  delayMicroseconds(50);

     send((col<<4)|0x08);
  send((col<<4)|0x0C);
   delayMicroseconds(1);
  send((col<<4)|0x08);
  delayMicroseconds(50);

  delayMicroseconds(2000);
  }

}

void lprint(char c[])
{
     uint8_t k,i=0;
     while((k=c[i++])!='\0')
   {
      send((k&0xF0)|0x09);
      send((k&0xF0)|0x0D);
    delayMicroseconds(1);
    send((k&0xF0)|0x09);
    delayMicroseconds(50);

      send((k<<4)|0x09);
      send((k<<4)|0x0D);
    delayMicroseconds(1);
    send((k<<4)|0x09);
    delayMicroseconds(50);

    delayMicroseconds(50);

  }
}
void lprintnum(uint32_t k)
{
    char c[8];
    uint8_t inc = 0;
    uint8_t t;

    t  = k/1000000;
    if(t!=0)
    lfl = 1;
    if(lfl)
    c[inc++] = 48+t;
    k = k%1000000;

    t  = k/100000;
    if(t!=0)
    lfl = 1;
    if(lfl)
    c[inc++] = 48+t;
    k = k%100000;

    t  = k/10000;
    if(t!=0)
    lfl = 1;
    if(lfl)
    c[inc++] = 48+t;
    k = k%10000;

    t  = k/1000;
    if(t!=0)
    lfl = 1;
    if(lfl)
    c[inc++] = 48+t;
    k = k%1000;

    t  = k/100;
    if(t!=0)
    lfl = 1;
    if(lfl)
    c[inc++] = 48+t;
    k = k%100;

    t  = k/10;
    if(t!=0)
    lfl = 1;
    if(lfl)
    c[inc++] = 48+t;
    k = k%10;

    t  = k;
    if(t!=0)
    lfl = 1;
    if(lfl)
    c[inc++] = 48+t;
   
    c[inc] = '\0';

    lprint(c);
    lfl = 0;

}