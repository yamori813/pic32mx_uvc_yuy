/********************************************************************^M
 FileName:      usb_descriptors.c^M
 Dependencies:  See INCLUDES section^M
 Processor:     PIC18 or PIC24 USB Microcontrollers^M
 Hardware:      The code is natively intended to be used on the following^M
~
                PIC18F87J50 FS USB Plug-In Module, or^M
                Explorer 16 + PIC24 USB PIM.  The firmware may be^M
                modified for use on other USB platforms by editing the^M
                HardwareProfile.h file.^M
 Complier:      Pinguino gcc^M
 Company:       Microchip Technology, Inc.^M
^M
 Software License Agreement:^M
^M
 The software supplied herewith by Microchip Technology Incorporated^M
~
~
 exclusively on Microchip PIC Microcontroller products. The^M
 software is owned by the Company and/or its supplier, and is^M
 protected under applicable copyright laws. All rights are reserved.^M
 Any use in violation of the foregoing restrictions may subject the^M
 user to criminal sanctions under applicable laws, as well as to^M
 civil liability for the breach of the terms and conditions of this^M
 license.^M
^M
~
 WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED^M
 TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A^M
 PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,^M
 IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR^M

*********************************************************************^M
-usb_descriptors.c-^M
-------------------------------------------------------------------
Filling in the descriptor values in the usb_descriptors.c file:
-------------------------------------------------------------------

[Device Descriptors]
The device descriptor is defined as a USB_DEVICE_DESCRIPTOR type.
This type is defined in usb_ch9.h  Each entry into this structure
needs to be the correct length for the data type of the entry.

[Configuration Descriptors]
The configuration descriptor was changed in v2.x from a structure
to a BYTE array.  Given that the configuration is now a byte array
each byte of multi-byte fields must be listed individually.  This
means that for fields like the total size of the configuration where
the field is a 16-bit value "64,0," is the correct entry for a
configuration that is only 64 bytes long and not "64," which is one
too few bytes.

The configuration attribute must always have the _DEFAULT
definition at the minimum. Additional options can be ORed
to the _DEFAULT attribute. Available options are _SELF and _RWU.
These definitions are defined in the usb_device.h file. The
_SELF tells the USB host that this device is self-powered. The
_RWU tells the USB host that this device supports Remote Wakeup.

[Endpoint Descriptors]
Like the configuration descriptor, the endpoint descriptors were
changed in v2.x of the stack from a structure to a BYTE array.  As
endpoint descriptors also has a field that are multi-byte entities,
please be sure to specify both bytes of the field.  For example, for
the endpoint size an endpoint that is 64 bytes needs to have the size
defined as "64,0," instead of "64,"

Take the following example:
    // Endpoint Descriptor //
    0x07,                       //the size of this descriptor //
    USB_DESCRIPTOR_ENDPOINT,    //Endpoint Descriptor
    _EP02_IN,                   //EndpointAddress
    _INT,                       //Attributes
    0x08,0x00,                  //size (note: 2 bytes)
    0x02,                       //Interval

The first two parameters are self-explanatory. They specify the
length of this endpoint descriptor (7) and the descriptor type.
The next parameter identifies the endpoint, the definitions are
defined in usb_device.h and has the following naming
convention:
_EP<##>_<dir>
where ## is the endpoint number and dir is the direction of
transfer. The dir has the value of either 'OUT' or 'IN'.
The next parameter identifies the type of the endpoint. Available
options are _BULK, _INT, _ISO, and _CTRL. The _CTRL is not
typically used because the default control transfer endpoint is
not defined in the USB descriptors. When _ISO option is used,
addition options can be ORed to _ISO. Example:
_ISO|_AD|_FE
This describes the endpoint as an isochronous pipe with adaptive
and feedback attributes. See usb_device.h and the USB
specification for details. The next parameter defines the size of
the endpoint. The last parameter in the polling interval.

-------------------------------------------------------------------
Adding a USB String
-------------------------------------------------------------------
A string descriptor array should have the following format:

rom struct{byte bLength;byte bDscType;word string[size];}sdxxx={
sizeof(sdxxx),DSC_STR,<text>};

The above structure provides a means for the C compiler to
calculate the length of string descriptor sdxxx, where xxx is the
index number. The first two bytes of the descriptor are descriptor
length and type. The rest <text> are string texts which must be
in the unicode format. The unicode format is achieved by declaring
each character as a word type. The whole text string is declared
as a word array with the number of characters equals to <size>.
<size> has to be manually counted and entered into the array
declaration. Let's study this through an example:
if the string is "USB" , then the string descriptor should be:
(Using index 02)
rom struct{byte bLength;byte bDscType;word string[3];}sd002={
sizeof(sd002),DSC_STR,'U','S','B'};

A USB project may have multiple strings and the firmware supports
the management of multiple strings through a look-up table.
The look-up table is defined as:
rom const unsigned char *rom USB_SD_Ptr[]={&sd000,&sd001,&sd002};

The above declaration has 3 strings, sd000, sd001, and sd002.
Strings can be removed or added. sd000 is a specialized string
descriptor. It defines the language code, usually this is
US English (0x0409). The index of the string must match the index
position of the USB_SD_Ptr array, &sd000 must be in position
USB_SD_Ptr[0], &sd001 must be in position USB_SD_Ptr[1] and so on.
The look-up table USB_SD_Ptr is used by the get string handler
function.

-------------------------------------------------------------------

The look-up table scheme also applies to the configuration
descriptor. A USB device may have multiple configuration
descriptors, i.e. CFG01, CFG02, etc. To add a configuration
descriptor, user must implement a structure similar to CFG01.
The next step is to add the configuration descriptor name, i.e.
cfg01, cfg02,.., to the look-up table USB_CD_Ptr. USB_CD_Ptr[0]
is a dummy place holder since configuration 0 is the un-configured
state according to the definition in the USB specification.

********************************************************************/

/*********************************************************************
 * Descriptor specific type definitions are defined in:
 * usb_device.h
 *
 * Configuration options are defined in:
 * usb_config.h
 ********************************************************************/
#ifndef __USB_DESCRIPTORS_C
#define __USB_DESCRIPTORS_C

/** INCLUDES *******************************************************/

#include "./USB/usb.h"
#include "HardwareProfile.h"

/** CONSTANTS ******************************************************/
#if defined(__18CXX)
#pragma romdata
#endif

#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)
#define DBVAL(x) (x & 0xFF),((x >> 8) & 0xFF),((x >> 16) & 0xFF),((x >> 24) & 0xFF)


#define WIDTH                                         (unsigned int)160
#define HEIGHT                                        (unsigned int)120
#define CAM_FPS                                       5
//#define VIDEO_PACKET_SIZE                             (unsigned int)(768+2)//128+130
#define MIN_BIT_RATE                                  (unsigned long)(WIDTH*HEIGHT*16*CAM_FPS)//16 bit
#define MAX_BIT_RATE                                  (unsigned long)(WIDTH*HEIGHT*16*CAM_FPS)
#define MAX_FRAME_SIZE                                (unsigned long)(WIDTH*HEIGHT*2)//yuy2
//#define MAX_FRAME_SIZE                                (unsigned long)(WIDTH*HEIGHT*3/2)//nv12
#define INTERVAL                                      (unsigned long)(10000000/CAM_FPS)
//#define PACKETS_IN_FRAME                              (unsigned int)(MAX_FRAME_SIZE/(VIDEO_PACKET_SIZE-2))


/* Device Descriptor */
ROM USB_DEVICE_DESCRIPTOR device_dsc=
{
    0x12,                           // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,          // DEVICE descriptor type
    0x0200,                         // USB Spec Release Number in BCD format
    0xEF,                           // Class Code
    0x02,                           // Subclass code
    0x01,                           // Protocol code
    USB_EP0_BUFF_SIZE,              // Max packet size for EP0, see system_config.h
    0x04D8,                         // Vendor ID
    0x0054,                         // Product ID
    0x0100,                         // Device release number in BCD format
    0x01,                           // Manufacturer string index
    0x02,                           // Product string index
    0x00,                           // Device serial number string index
    0x01                            // Number of possible configurations
};

#ifdef SAMPLE24
typedef struct
{
    unsigned char left[3];
    unsigned char right[3];
}
AUDIO_PLAY_SAMPLE;
#else
typedef struct
{
    INT16 left;
    INT16 right;
}
AUDIO_PLAY_SAMPLE;
#endif
#define AUDIO_MAX_FREQ              48000
#define AUDIO_MAX_SAMPLES           48



/* Configuration 1 Descriptor */
ROM BYTE configDescriptor1[] ={
     /* Configuration Descriptor */
        0x09,                           /* Descriptor Size */
        0x02,        			/* Configuration Descriptor Type */
        0xA7,0x00,                      /* Length of this descriptor and all sub descriptors */
        0x02,                           /* Number of interfaces */
        0x01,                           /* Configuration number */
        0x00,                           /* COnfiguration string index */
        0x80,                           /* Config characteristics - Bus powered */
        0xFA,                           /* Max power consumption of device (in 2mA unit) : 500mA */

        /* Interface Association Descriptor */
        0x08,                           /* Descriptor Size */
        11,                             /* Interface Association Descr Type: 11 */
        0x00,                           /* I/f number of first VideoControl i/f */
        0x02,                           /* Number of Video i/f */
        0x0E,                           /* CC_VIDEO : Video i/f class code */
        0x03,                           /* SC_VIDEO_INTERFACE_COLLECTION : Subclass code */
        0x00,                           /* Protocol : Not used */
        0x00,                           /* String desc index for interface */

        /* Standard Video Control Interface Descriptor */
        0x09,                           /* Descriptor size */
        0x04,        /* Interface Descriptor type */
        0x00,                           /* Interface number */
        0x00,                           /* Alternate setting number */
        0x00,                           /* Number of end points */
        0x0E,                           /* CC_VIDEO : Interface class */
        0x01,                           /* CC_VIDEOCONTROL : Interface sub class */
        0x00,                           /* Interface protocol code */
        0x00,                           /* Interface descriptor string index */

        /* Class specific VC Interface Header Descriptor */
        0x0D,                           /* Descriptor size */
        0x24,                           /* Class Specific I/f Header Descriptor type */
        0x01,                           /* Descriptor Sub type : VC_HEADER */
        0x00,0x01,                      /* Revision of class spec : 1.0 */
        0x27,0x00,                      /* Total Size of class specific descriptors (till Output terminal) */
        0x80,0x8D,0x5B,0x00,            /* Clock frequency : 48MHz(Deprecated) */
        0x01,                           /* Number of streaming interfaces */
        0x01,                           /* Video streaming I/f 1 belongs to VC i/f */

        /* Input (Camera) Terminal Descriptor */
        0x11,                           /* Descriptor size */
        0x24,                           /* Class specific interface desc type */
        0x02,                           /* Input Terminal Descriptor type */
        0x01,                           /* ID of this terminal */
        0x01,0x02,                      /* Camera terminal type */
        0x00,                           /* No association terminal */
        0x00,                           /* String desc index : Not used */
        0x00,0x00,                      /* No optical zoom supported */
        0x00,0x00,                      /* No optical zoom supported */
        0x00,0x00,                      /* No optical zoom supported */
        0x02,                           /* Size of controls field for this terminal : 2 bytes */
        0x00,0x00,                 /* No controls supported */
        
        /* Output Terminal Descriptor */
        0x09,                           /* Descriptor size */
        0x24,                           /* Class specific interface desc type */
        0x03,                           /* Output Terminal Descriptor type */
        0x02,                           /* ID of this terminal */
        0x01,0x01,                      /* USB Streaming terminal type */
        0x00,                           /* No association terminal */
        0x01,                           /* Source ID : 1 : Connected to Extn Unit */
        0x00,                           /* String desc index : Not used */
        
        /* Video Streaming (VS) Interface Descriptor */
        
        
        /* Standard VS Interface Descriptor  = interface 1 */
        // alternate setting 0 = Zero Bandwidth
        
        0x09,                           /* Descriptor size */
        0x04,        /* Interface Descriptor type */
        0x01,                           /* Interface number */
        0x00,                           /* Alternate setting number */
        0x00,                           /* Number of end points : Zero Bandwidth */
        0x0E,                           /* Interface class : CC_VIDEO */
        0x02,                           /* Interface sub class : CC_VIDEOSTREAMING */
        0x00,                           /* Interface protocol code : Undefined */
        0x00,                           /* Interface descriptor string index */  

       /* Class-specific Video Streaming Input Header Descriptor */
        0x0E,                           /* Descriptor size */
        0x24,                           /* Class-specific VS I/f Type */
        0x01,                           /* Descriptotor Subtype : Input Header */
        0x01,                           /* 1 format desciptor follows */
        0x4D,0x00,                      /* Total size of Class specific VS descr: 41 Bytes */
        0x81,             /* EP address for BULK video data */
        0x00,                           /* No dynamic format change supported */
        0x02,                           /* Output terminal ID : 4 */
        0x00,                           /* Still image capture method 1 supported */
        0x01,                           /* Hardware trigger NOT supported */
        0x00,                           /* Hardware to initiate still image capture NOT supported */
        0x01,                           /* Size of controls field : 1 byte */
        0x00,                           /* D2 : Compression quality supported */

   
       /* Class specific Uncompressed VS format descriptor */
        0x1B,                           /* Descriptor size */
        0x24,                           /* Class-specific VS I/f Type */
        0x04,                           /* Subtype : uncompressed format I/F */
        0x01,                           /* Format desciptor index (only one format is supported) */
        0x01,                           /* number of frame descriptor followed */
        0x59,0x55,0x59,0x32,            /* GUID, globally unique identifier used to identify streaming-encoding format: YUY2  */       
        0x00,0x00,0x10,0x00,
        0x80,0x00,0x00,0xAA,  
        0x00,0x38,0x9B,0x71,       
        0x10,                           /* Number of bits per pixel used to specify color in the decoded video frame. 0 if not applicable: 10 bit per pixel */
        0x01,                           /* Optimum Frame Index for this stream: 1 */
        0x00,                           /* X dimension of the picture aspect ratio: Non-interlaced in progressive scan */
        0x00,                           /* Y dimension of the pictuer aspect ratio: Non-interlaced in progressive scan*/
        0x00,                           /* Interlace Flags: Progressive scanning, no interlace */
        0x00,                           /* duplication of the video stream restriction: 0 - no restriction */
       


       /* Class specific Uncompressed VS Frame descriptor */
        0x1E,                           /* Descriptor size */
        0x24,                           /* Descriptor type*/
        0x05,                           /* Subtype: uncompressed frame I/F */
        0x01,                           /* Frame Descriptor Index */
        0x02,                           /* Still image capture method 1 supported, fixed frame rate */
        WBVAL(WIDTH),                      /* Width in pixel: 160-QVGA */
        WBVAL(HEIGHT),                      /* Height in pixel 120-QVGA */         
         DBVAL(MIN_BIT_RATE),            /* Min bit rate bits/s. Not specified, taken from MJPEG */
        DBVAL(MAX_BIT_RATE),            /* Max bit rate bits/s. Not specified, taken from MJPEG */        
        DBVAL(MAX_FRAME_SIZE),            /* Maximum video or still frame size in bytes(Deprecated) */        
        DBVAL(INTERVAL),            /* Default Frame Interval */        
        0x01,                           /* Frame interval(Frame Rate) types: Only one frame interval supported */
        DBVAL(INTERVAL),            /* Shortest Frame Interval */                
       
          /* Color Matching Descriptor */

        0x06,          /* bLength */
        0x24,                         /* bDescriptorType : CS_INTERFACE */
        0x0D,                                 /* bDescriptorSubType : VS_COLORFORMAT */
        0x01,                                 /* bColorPrimarie : 1: BT.709, sRGB (default) */
        0x01,                                 /* bTransferCharacteristics : 1: BT.709 (default) */
        0x04,                                 /* bMatrixCoefficients : 1: BT. 709. */
              
       /* Standard Video Streaming Interface Descriptor (Alternate Setting 1) */
        0x09,                           /* Descriptor size */
        0x04,        /* Interface Descriptor type */
        0x01,                           /* Interface number */
        0x01,                           /* Alternate setting number */
        0x01,                           /* Number of end points : Zero Bandwidth */
        0x0E,                           /* Interface class : CC_VIDEO */
        0x02,                           /* Interface sub class : CC_VIDEOSTREAMING */
        0x00,                           /* Interface protocol code : Undefined */
        0x00,                           /* Interface descriptor string index */  
        
        /* Endpoint Descriptor for BULK Streaming Video Data */
        0x07,                           /* Descriptor size */
        0x5,        /* Endpoint Descriptor Type */
        0x81,             /* Endpoint address and description */
        USB_TRANSFER_TYPE_ISOCHRONOUS ,                           /* BULK End point */
        0xFF,0x03,         /*  max packet size */       
        0x01                            /* Servicing interval for data transfers */    
};

//Language code string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[1];}sd000={
sizeof(sd000),USB_DESCRIPTOR_STRING,{0x0409}};

//Manufacturer string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[25];}sd001={
sizeof(sd001),USB_DESCRIPTOR_STRING,
{'M','i','c','r','o','c','h','i','p',' ','T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'}
};

//Product string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[21];}sd002={
sizeof(sd002),USB_DESCRIPTOR_STRING,
{'S','i','m','p','l','e',' ','W','i','n','U','S','B',' ','D','e','v','i','c','e',' ','D','e','m','o'}
};

//Array of configuration descriptors
ROM BYTE *ROM USB_CD_Ptr[]=
{
    (ROM BYTE *ROM)&configDescriptor1
};
//Array of string descriptors
ROM BYTE *ROM USB_SD_Ptr[]=
{
    (ROM BYTE *ROM)&sd000,
    (ROM BYTE *ROM)&sd001,
    (ROM BYTE *ROM)&sd002
};


#endif
/** EOF usb_descriptors.c ***************************************************/

