#ifndef _DEVICES_H_
#define _DEVICES_H_

#include "kpppconfig.h"

static const char *devices[] = {
#ifdef __FreeBSD__
#if __FreeBSD_version > 600004
  "/dev/cuad0",
  "/dev/cuad1",
  "/dev/cuad2",
  "/dev/cuad3",
  "/dev/cuad4",
#else
  "/dev/cuaa0",
  "/dev/cuaa1",
  "/dev/cuaa2",
  "/dev/cuaa3",
  "/dev/cuaa4",
#endif
#elif defined(__NetBSD__)
  "/dev/tty00",                /* "normal" modem lines */
  "/dev/tty01",
  "/dev/tty02",
  "/dev/tty03",
  "/dev/dty00",                /* Dial out devices */
  "/dev/dty01",
  "/dev/dty02",
  "/dev/dty03",
  "/dev/ttyU0",                /* USB stuff modems */
  "/dev/ttyU1",
  "/dev/ttyU2",
  "/dev/ttyU3",
  "/dev/dtyU0",                /* USB stuff, too (dial out device) */
  "/dev/dtyU1",
  "/dev/dtyU2",
  "/dev/dtyU3",
#elif defined (__linux__)
  "/dev/modem",
  "/dev/ttyS0",
  "/dev/ttyS1",
  "/dev/ttyS2",
  "/dev/ttyS3",
  "/dev/ttyS4",
#ifdef ISDNSUPPORT
  "/dev/ttyI0",
  "/dev/ttyI1",
  "/dev/ttyI2",
  "/dev/ttyI3",
#endif
  "/dev/usb/ttyACM0",          /* USB stuff modems */
  "/dev/usb/ttyACM1",
  "/dev/usb/ttyACM2",
  "/dev/usb/ttyACM3",
  "/dev/usb/ttyUSB0",          /* USB stuff modems */
  "/dev/usb/ttyUSB1",
  "/dev/usb/ttyUSB2",
  "/dev/usb/ttyUSB3",
  "/dev/ttyACM0",          /* USB stuff modems  with udev */
  "/dev/ttyACM1",
  "/dev/ttyACM2",
  "/dev/ttyACM3",
  "/dev/ttyUSB0",
  "/dev/ttyUSB1",
  "/dev/ttyUSB2",
  "/dev/ttyUSB3",  
  "/dev/usb/tts/0",          /* USB stuff modems with devfs*/
  "/dev/usb/tts/1",
  "/dev/usb/tts/2",
  "/dev/usb/tts/3",
  "/dev/rfcomm0",	       /* BlueTooth */
  "/dev/rfcomm1",
  "/dev/rfcomm2",
  "/dev/rfcomm3",
  "/dev/ircomm0",	       /* IrDA */
  "/dev/ircomm1",
  "/dev/ircomm2",
  "/dev/ircomm3",
  "/dev/ttySL0",           /* necessary for slmodem driver http://www.smlink.com*/
  "/dev/ttySL1",
  "/dev/ttySL2",
  "/dev/ttySL3",  
  "/dev/ttySHSF0",
  "/dev/ttySHSF1",
  "/dev/ttySHSF2",
  "/dev/ttySHSF3",  /* necessary for conexant modem which use hsfserial commercial module */
#elif defined(__svr4__)
  "/dev/cua/a",
  "/dev/cua/b",
  "/dev/ttya",
  "/dev/ttyb",
#endif
  0};

// default device number from the list above
const int DEV_DEFAULT = 0;

#endif
