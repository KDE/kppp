#ifndef _DEVICES_H_
#define _DEVICES_H_

#include "kpppconfig.h"

static const char *devices[] = {
#ifdef __FreeBSD__
  "/dev/cuaa0",
  "/dev/cuaa1",
  "/dev/cuaa2",
  "/dev/cuaa3",
#elif defined (__linux__)
  "/dev/modem",
// Linux from 2.1.x on discourages from using this devices
#if 1
  "/dev/cua0",
  "/dev/cua1",
  "/dev/cua2",
  "/dev/cua3",
#endif
  "/dev/ttyS0",
  "/dev/ttyS1",
  "/dev/ttyS2",
  "/dev/ttyS3",

#ifdef ISDNSUPPORT
  "/dev/ttyI0",
  "/dev/ttyI1",
  "/dev/ttyI2",
  "/dev/ttyI3",
#endif
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
