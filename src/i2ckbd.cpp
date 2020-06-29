#include <Wire.h>
#include "i2ckbd.h"

#define FACES_KEYBOARD_I2C_ADDR 0x08
#define CARDKB_I2C_ADDR 0x5F
#define I2CKBD_MAXBUF 128

static uint8_t i2ckbd_buffer[I2CKBD_MAXBUF];
static int i2ckbd_readpos = 0;
static int i2ckbd_writepos = 0;
static enum I2CKBD_MODE i2ckbd_mode = I2CKBD_NONE;

const char *convert_faceskbd(int c);
const char *convert_cardkbd(int c);

void i2ckbd_init()
{
  Wire.begin();
  pinMode(5, INPUT);
  digitalWrite(5,HIGH);
}

void i2ckbd_setmode(int mode)
{
  i2ckbd_mode = (enum I2CKBD_MODE) mode;
}

int i2ckbd_getmode()
{
  return (int) i2ckbd_mode;
}

int i2ckbd_readchar()
{
  if (i2ckbd_readpos == i2ckbd_writepos) {
    return -1;
  }
  uint8_t c = i2ckbd_buffer[i2ckbd_readpos];
  i2ckbd_readpos = (i2ckbd_readpos + 1) % I2CKBD_MAXBUF;
  return (int) c;
}

void i2ckbd_writechar(uint8_t c)
{
  i2ckbd_buffer[i2ckbd_writepos] = c;
  i2ckbd_writepos = (i2ckbd_writepos + 1) % I2CKBD_MAXBUF;
}

void i2ckbd_convertchar(uint8_t c)
{
  const char *pattern = NULL;
  switch (i2ckbd_mode) {
  case I2CKBD_FACESKBD:
    pattern = convert_faceskbd(c);
    break;
  case I2CKBD_CARDKBD:
    pattern = convert_cardkbd(c);
    break;
  default:
    break;
  }
  if (pattern == NULL) {
    i2ckbd_writechar(c);
    return;
  }
  while ((c = *pattern++) > 0) {
    i2ckbd_writechar(c);
  }
}

int i2ckbd_get()
{
  int c = i2ckbd_readchar();
  if (c >= 0) {
    return c;
  }
  switch (i2ckbd_mode) {
  case I2CKBD_FACESKBD:
    if (digitalRead(5) == LOW)
    {
      Wire.requestFrom(FACES_KEYBOARD_I2C_ADDR, 1);
      while (Wire.available())
      {
        i2ckbd_convertchar(Wire.read());
      }
    }
    break;
  case I2CKBD_CARDKBD:
    Wire.requestFrom(CARDKB_I2C_ADDR, 1);
    while (Wire.available())
    {
      int c = Wire.read();
      if (c > 0) {
        i2ckbd_convertchar(c);
      }
    }
    break;
  default:
    break;
  }
  return i2ckbd_readchar();
}
