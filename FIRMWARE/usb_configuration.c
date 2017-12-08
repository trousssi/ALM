#include <stdio.h>                       // for NULL definition
#include <string.h>
#include <ctype.h>

#include "cold_startup.h"
#include "identity.h"
#include "shield_discoveryf4_wiring.h"

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

#define P13_BOTTOM_TTY_0_EPOUT	0x1
#define P13_BOTTOM_TTY_0_EPIN  	0x82

#define P24_TOP_TTY_1_EPOUT	0x3
#define P24_TOP_TTY_1_EPIN  	0x84

#define SAMPLES_EPIN  	        0x85

static const struct usb_interface_descriptor p13_top_tty_0_iface;
static const struct usb_endpoint_descriptor p13_top_tty_0_endp[];
static const struct usb_interface_descriptor p13_top_features_0_iface;

static const struct usb_interface_descriptor p24_bottom_tty_1_iface;
static const struct usb_endpoint_descriptor p24_bottom_tty_1_endp[];
static const struct usb_interface_descriptor p24_bottom_features_1_iface;

static const struct usb_interface_descriptor samples_iface;
static const struct usb_endpoint_descriptor samples_endp;


static const struct usb_interface ifaces[];
static const char *usb_strings[];

char serialno_buffer[SERIALNO_MAX_SIZE]=SERIAL_NO_PREFIX;

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
    .iSerialNumber = 3,
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
    .altsetting = &p13_top_tty_0_iface,
},
{
    .num_altsetting = 1,
    .altsetting = &p13_top_features_0_iface,
},
{
    .num_altsetting = 1,
    .altsetting = &p24_bottom_tty_1_iface,
},
{
    .num_altsetting = 1,
    .altsetting = &p24_bottom_features_1_iface,
},
{
    .num_altsetting = 1,
    .altsetting = &samples_iface,
},
};

static const struct usb_interface_descriptor p13_top_tty_0_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 1,
    .bAlternateSetting = 1,
    .bNumEndpoints = 2,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 4,
    .endpoint = p13_top_tty_0_endp,
};

static const struct usb_endpoint_descriptor p13_top_tty_0_endp[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = P13_BOTTOM_TTY_0_EPOUT ,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 64,
     // 8ms polling rate : 8 Kbytes/s bandwith >  57600 baud rate
    .bInterval = 8,
}, {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = P13_BOTTOM_TTY_0_EPIN,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 64,
     // 16ms polling rate : 4 Kbytes/s bandwith < 57600 baud rate
    .bInterval = 16,
}
};

static const struct usb_interface_descriptor p13_top_features_0_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 2,
    .bAlternateSetting = 2,
    .bNumEndpoints = 0,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 5,
    .endpoint = NULL,
};

static const struct usb_interface_descriptor p24_bottom_tty_1_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 3,
    .bAlternateSetting = 3,
    .bNumEndpoints = 2,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 6,
    .endpoint = p24_bottom_tty_1_endp,
};

static const struct usb_endpoint_descriptor p24_bottom_tty_1_endp[] = {{
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = P24_TOP_TTY_1_EPOUT ,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 64,
     // 8ms polling rate : 8 Kbytes/s bandwith >  57600 baud rate
    .bInterval = 8,
}, {
    .bLength = USB_DT_ENDPOINT_SIZE,
    .bDescriptorType = USB_DT_ENDPOINT,
    .bEndpointAddress = P24_TOP_TTY_1_EPIN,
    .bmAttributes = USB_ENDPOINT_ATTR_INTERRUPT,
    .wMaxPacketSize = 64,
     // 16ms polling rate : 4 Kbytes/s bandwith < 57600 baud rate
    .bInterval = 16,
}
};

static const struct usb_interface_descriptor p24_bottom_features_1_iface = {
    .bLength = USB_DT_INTERFACE_SIZE,
    .bDescriptorType = USB_DT_INTERFACE,
    .bInterfaceNumber = 4,
    .bAlternateSetting = 4,
    .bNumEndpoints = 0,
    .bInterfaceClass = 0xFF,
    .bInterfaceSubClass = 0,
    .bInterfaceProtocol = 0,
    .iInterface = 7,
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
    .iInterface = 8,
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

#define USB_STRINGS_SIZE 8

static const char *usb_strings[] = {
    ID_VENDOR_STRING,            	// 1 
    ID_PRODUCT_STRING,           	// 2
    serialno_buffer,               	// 3
    "P13_BOTTOM_TTY_0",              	// 4
    "P13_BOTTOM_FEATURES_0",           	// 5
    "P24_TOP_TTY_1",             	// 6
    "P24_TOP_FEATURES_1",           	// 7
    "SAMPLES"                          	// 8
};

usbd_device *usbd_dev;

// Default pipe and features interface pipe control buffers
uint8_t default_control_buffer [DEFAULT_CONTROL_BUFFER_SIZE] __attribute__((aligned  (16)));
uint8_t features_control_buffer [FEATURES_CONTROL_BUFFER_SIZE] __attribute__((aligned  (16)));


char buffer[64];
int length;

void usb_reset_cb()
{
}


void tty0_out_callback (usbd_device *usbd_dev,uint8_t ep) {
  int i;

  length=usbd_ep_read_packet(usbd_dev,P13_BOTTOM_TTY_0_EPOUT,buffer,64);
//  plusplus();
  for(i=0;i<length;i++) {
      buffer[i] = toupper((int)buffer[i]);
    }
  length=usbd_ep_write_packet(usbd_dev,P13_BOTTOM_TTY_0_EPIN,buffer,length);
}


void intin_callback (usbd_device *usbd_dev,uint8_t ep) {
//   length=usbd_ep_write_packet(usbd_dev,P13_BOTTOM_TTY_0_EPOUT,"au revoir!",8);
}

int ctrlreq_callback (usbd_device *usbd_dev,
                      struct usb_setup_data *req, uint8_t **buf, uint16_t *len,
                      usbd_control_complete_callback *complete) {
   
    write_f4_leds (req->bRequest & 0x7); 
    // (*complete) (usbd_dev,req);
    return 1;  // ok

}

void usbdev_set_config(usbd_device *usbd_dev, uint16_t wValue)
{
    (void)wValue;

    usbd_register_control_callback(usbd_dev,USB_REQ_TYPE_VENDOR,USB_REQ_TYPE_TYPE,ctrlreq_callback);    
    usbd_ep_setup(usbd_dev, P13_BOTTOM_TTY_0_EPIN, USB_ENDPOINT_ATTR_INTERRUPT, 64,NULL);
    usbd_ep_setup(usbd_dev, P13_BOTTOM_TTY_0_EPOUT, USB_ENDPOINT_ATTR_INTERRUPT, 64,tty0_out_callback);

    usbd_ep_setup(usbd_dev, P24_TOP_TTY_1_EPIN, USB_ENDPOINT_ATTR_INTERRUPT, 64,NULL);

    usbd_ep_setup(usbd_dev, P24_TOP_TTY_1_EPOUT, USB_ENDPOINT_ATTR_INTERRUPT, 64,NULL);

    usbd_ep_setup(usbd_dev, SAMPLES_EPIN, USB_ENDPOINT_ATTR_BULK, 64, intin_callback);

    usbd_register_reset_callback(usbd_dev, usb_reset_cb);

}

#if 0
void usb_init() {
    uint16_t n;
    // Append board number n to serialno prefix
    n = input_id();    
    sprintf(serialno_buffer+strlen(serialno_buffer),"0x%02hx",n);

    usbd_dev = usbd_init(&otgfs_usb_driver, &dev, &config, 
                         usb_strings, USB_STRINGS_SIZE,
                         default_control_buffer, DEFAULT_CONTROL_BUFFER_SIZE);
    usbd_register_set_config_callback(usbd_dev, usbdev_set_config);

//   length=usbd_ep_write_packet(usbd_dev,SAMPLES_EPIN,"12345678",8);
//   length=usbd_ep_write_packet(usbd_dev,P13_BOTTOM_TTY_0_EPIN,"ABCDEFGH",8);
//   length=usbd_ep_write_packet(usbd_dev,P24_TOP_TTY_1_EPIN,"abcdefgh",8);
}
#endif
