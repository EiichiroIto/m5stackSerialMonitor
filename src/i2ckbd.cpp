#include <Wire.h>
#include "i2ckbd.h"

#define FACES_KEYBOARD_I2C_ADDR 0x08
#define I2CKBD_MAXBUF 128

static uint8_t i2ckbd_buffer[I2CKBD_MAXBUF];
static int i2ckbd_readpos = 0;
static int i2ckbd_writepos = 0;

const char *convert_faceskbd(int c);

static const char *(*converter)(int) = convert_faceskbd;

void i2ckbd_init()
{
  Wire.begin();
  pinMode(5, INPUT);
  digitalWrite(5,HIGH);
}

void i2ckbd_select(int mode)
{
  switch (mode) {
  case I2CKBD_FACESKBD:
    converter = convert_faceskbd;
    break;
  default:
    converter = NULL;
    break;
  }
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
  if (converter == NULL) {
    i2ckbd_writechar(c);
    return;
  }
  const char *pattern = (*converter)(c);
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
  if (digitalRead(5) == LOW)
  {
    Wire.requestFrom(FACES_KEYBOARD_I2C_ADDR, 1);
    while (Wire.available())
    {
      i2ckbd_convertchar(Wire.read());
    }
    delay(10);
  }
  return i2ckbd_readchar();
}
