enum I2CKBD_MODE {
  I2CKBD_NONE=0, I2CKBD_FACESKBD, I2CKBD_CARDKBD, I2CKBD_NUM
};

#ifdef __cplusplus
extern "C" {
#endif

  void i2ckbd_init();
  int i2ckbd_get();
  void i2ckbd_setmode(int mode);
  int i2ckbd_getmode();

#ifdef __cplusplus
};
#endif
