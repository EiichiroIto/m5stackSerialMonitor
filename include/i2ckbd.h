enum {
  I2CKBD_NONE=0, I2CKBD_FACESKBD, I2CKBD_NUM=I2CKBD_FACESKBD
};

#ifdef __cplusplus
extern "C" {
#endif

  void i2ckbd_init();
  int i2ckbd_get();
  void i2ckbd_select(int mode);

#ifdef __cplusplus
};
#endif
