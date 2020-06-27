#include <Wire.h>
#include "i2ckb.h"

#define FACES_KEYBOARD_I2C_ADDR 0x08
#define I2CKB_MAXBUF 128

static uint8_t i2ckb_buffer[I2CKB_MAXBUF];
static int i2ckb_readpos = 0;
static int i2ckb_writepos = 0;

void i2ckb_init()
{
  Wire.begin();
  pinMode(5, INPUT);
  digitalWrite(5,HIGH);
}

int i2ckb_readchar()
{
  if (i2ckb_readpos == i2ckb_writepos) {
    return -1;
  }
  uint8_t c = i2ckb_buffer[i2ckb_readpos];
  i2ckb_readpos = (i2ckb_readpos + 1) % I2CKB_MAXBUF;
  return (int) c;
}

void i2ckb_writechar(uint8_t c)
{
  i2ckb_buffer[i2ckb_writepos] = c;
  i2ckb_writepos = (i2ckb_writepos + 1) % I2CKB_MAXBUF;
}

int i2ckb_get()
{
  int c = i2ckb_readchar();
  if (c >= 0) {
    return c;
  }
  if (digitalRead(5) == LOW)
  {
    Wire.requestFrom(FACES_KEYBOARD_I2C_ADDR, 1);
    while (Wire.available())
    {
      i2ckb_writechar(Wire.read());
    }
  }
  return i2ckb_readchar();
}
