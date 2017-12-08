#ifndef _USB_CONFIGURATION_H
#define _USB_CONFIGURATION_H

#include <libopencm3/usb/usbd.h>

extern usbd_device *usbd_dev;
extern void usb_init();

#endif
