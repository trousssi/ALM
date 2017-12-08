/*
#include <stdlib.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/cdc.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/flash.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/pwr.h>
*/

#include <stdio.h>                       // for NULL definition
#include "cold_startup.h"
#include "identity.h"

#include <libopencm3/usb/usbd.h>
#include <libopencm3/usb/usbstd.h>

#ifndef DEFAULT_CONTROL_BUFFER_SIZE
#define DEFAULT_CONTROL_BUFFER_SIZE 512
#endif

#ifndef FEATURES_CONTROL_BUFFER_SIZE
#define FEATURES_CONTROL_BUFFER_SIZE 256
#endif

#ifndef SERIALNO_MAX_SIZE
#define SERIALNO_MAX_SIZE 128
#endif

#define TTY_TOP_0_EPOUT		0x1
#define TTY_TOP_0_EPIN  	0x82

#define TTY_BOTTOM_1_EPOUT	0x3
#define TTY_BOTTOM_1_EPIN  	0x84

#define SAMPLES_EPIN  	        0x85

static const struct usb_interface_descriptor tty_top_0_iface;
static const struct usb_endpoint_descriptor tty_top_0_endp[];
static const struct usb_interface_descriptor features_top_0_iface;

static const struct usb_interface_descriptor tty_bottom_1_iface;
static const struct usb_endpoint_descriptor tty_bottom_1_endp[];
static const struct usb_interface_descriptor features_bottom_1_iface;

static const struct usb_interface_descriptor samples_iface;
static const struct usb_endpoint_descriptor samples_endp;


static const struct usb_interface ifaces[];
static const char *usb_strings[];

//char serialno_buffer[SERIALNO_MAX_SIZE]=SERIAL_NO_PREFIX;

static const struct usb_device_descriptor dev = {
    .bLength = USB_DT_DEVICE_SIZE,
    .bDescriptorType = USB_DT_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0,
    .bDeviceSubClass = 0,
    .bDeviceProtocol = 0,
    .bMaxPacketSize0 = 64,
    .idVendor = VENDOR_ID,
    .idProduct = PRODUCT_ID,
    .bcdDevice = 0x0200,
    .iManufacturer = 1,
    .iProduct = 2,
    .iSerialNumber = 0,
    .bNumConfigurations = 1,
};

static const struct usb_config_descriptor config = {
    .bLength = USB_DT_CONFIGURATION_SIZE,
    .bDescriptorType = USB_DT_CONFIGURATION,
    .wTotalLength = 0,
    .bNumInterfaces = 5,
    .bConfigurationValue = 1,
    .iConfiguration = 0,
    .bmAttributes = 0x80,
    .bMaxPower = 0x32,
    .interface = ifaces,
};

static const struct usb_interface ifaces[] = {
{
    .num_altsetting = 1,
    .altsetting = &tty_top_0_iface,
},
{
    .num_altsetting = 1,
    .altsetting = &features_top_0_iface,
},
{
    .num_altsetting = 1,
    .altsetting = &tty_bottom_1_iface,
},
{
    .num_altsetting = 1,
    .altsetting = &features_bottom_1_iface,
},
{
    .num_altsetting = 1,
    .altsetting = &samples_iface,
},
};

static const struct usb_interface_descriptor tty_top_0_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 1,
    .bAlternateSetting = 1,
    .bNumEndpoints = 2,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 0,
    .endpoint = tty_top_0_endp,
};

static const struct usb_endpoint_descriptor tty_top_0_endp[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = TTY_TOP_0_EPOUT ,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 64,
     // 8ms polling rate : 8 Kbytes/s bandwith >  57600 baud rate
    .bInterval = 8,
}, {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = TTY_TOP_0_EPIN,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 64,
     // 16ms polling rate : 4 Kbytes/s bandwith < 57600 baud rate
    .bInterval = 16,
}
};

static const struct usb_interface_descriptor features_top_0_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 2,
    .bAlternateSetting = 2,
    .bNumEndpoints = 0,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 0,
    .endpoint = NULL,
};

static const struct usb_interface_descriptor tty_bottom_1_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 3,
    .bAlternateSetting = 3,
    .bNumEndpoints = 2,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 0,
    .endpoint = tty_bottom_1_endp,
};

static const struct usb_endpoint_descriptor tty_bottom_1_endp[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = TTY_BOTTOM_1_EPOUT ,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 64,
     // 8ms polling rate : 8 Kbytes/s bandwith >  57600 baud rate
    .bInterval = 8,
}, {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = TTY_BOTTOM_1_EPIN,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 64,
     // 16ms polling rate : 4 Kbytes/s bandwith < 57600 baud rate
    .bInterval = 16,
}
};

static const struct usb_interface_descriptor features_bottom_1_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 4,
    .bAlternateSetting = 6,
    .bNumEndpoints = 0,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 0,
    .endpoint = NULL,
};

static const struct usb_interface_descriptor samples_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 5,
    .bAlternateSetting = 5,
    .bNumEndpoints = 1,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 0,
    .endpoint = &samples_endp,
};


static const struct usb_endpoint_descriptor samples_endp = {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = SAMPLES_EPIN,
    .bmAttributes = USB_ENDPOINT_ATTR_BULK,
    .wMaxPacketSize = 64,
    .bInterval = 1,
};

//static const char *serial_str = (const char*)0x08004000;

#define USB_STRINGS_SIZE 5

// No string decriptor for interfaces : bug on string 5
static const char *usb_strings[] = {
    "Philippe Waille",                     // Vendor
    "LIG energy monitor",                  // Product
//     serialno_buffer                       // serialno_$n
    "serialno_const",
    "",
    "",
    "",
};

usbd_device *usbd_dev;

// Default pipe and features interface pipe control buffers
uint8_t default_control_buffer [DEFAULT_CONTROL_BUFFER_SIZE] __attribute__((aligned  (16)));
uint8_t features_control_buffer [FEATURES_CONTROL_BUFFER_SIZE] __attribute__((aligned  (16)));

void usb_reset_cb()
{
}

char buffer[64];
int length;
void tty0_out_callback (usbd_device *usbd_dev,uint8_t ep) {
  int i;

  length=usbd_ep_read_packet(usbd_dev,TTY_TOP_0_EPOUT,buffer,64);
  plusplus();
  for(i=0;i<length;i++) {
    if (islower(buffer[i])) { 
      buffer[i] += 'A' - 'a';
    }
  }
  length=usbd_ep_write_packet(usbd_dev,TTY_TOP_0_EPIN,buffer,length);
}

void intin_callback (usbd_device *usbd_dev,uint8_t ep) {
   length=usbd_ep_write_packet(usbd_dev,TTY_TOP_0_EPOUT,"au revoir!",8);
}

void usbdev_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
    (void)wValue;
    usbd_ep_setup(usbd_dev, TTY_TOP_0_EPIN, USB_ENDPOINT_ATTR_INTERRUPT, 64,NULL);
    usbd_ep_setup(usbd_dev, TTY_TOP_0_EPOUT, USB_ENDPOINT_ATTR_INTERRUPT, 64,tty0_out_callback);

    usbd_ep_setup(usbd_dev, TTY_BOTTOM_1_EPIN, USB_ENDPOINT_ATTR_INTERRUPT, 64,NULL);

    usbd_ep_setup(usbd_dev, TTY_BOTTOM_1_EPOUT, USB_ENDPOINT_ATTR_INTERRUPT, 64,NULL);

    usbd_ep_setup(usbd_dev, SAMPLES_EPIN, USB_ENDPOINT_ATTR_BULK, 64, intin_callback);

    usbd_register_reset_callback(usbd_dev, usb_reset_cb);

   length=usbd_ep_write_packet(usbd_dev,SAMPLES_EPIN,"12345678",8);
   length=usbd_ep_write_packet(usbd_dev,TTY_TOP_0_EPIN,"ABCDEFGH",8);
   length=usbd_ep_write_packet(usbd_dev,TTY_BOTTOM_1_EPIN,"abcdefgh",8);
}


void usb_init() {
    uint8_t n;
    // Append board number n to serialno prefix
    n = read_n();    
//    sprintf("%2x",serialno_buffer+sizeof(SERIAL_NO_PREFIX),n);

    usbd_dev = usbd_init(&otgfs_usb_driver, &dev, &config, 
                         usb_strings, USB_STRINGS_SIZE,
                         default_control_buffer, DEFAULT_CONTROL_BUFFER_SIZE);
    usbd_register_set_config_callback(usbd_dev, usbdev_set_config);
}
