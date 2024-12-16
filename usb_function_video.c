/************************************************************************
  File Information:
    FileName:       usb_function_video.c

********************************************************************/

#include "HardwareProfile_MX220F032B.h"
#include "./USB/usb.h"
#include "usb_function_video.h"

#ifdef USB_USE_VIDEO_CLASS

extern BYTE CtrlTrfData[];

void USBCheckVideoRequest()
{
int i;
static char buf[26] = {0x00, 0x00, 0x01, 0x02, 0x80, 0x1a, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x09, 0x00, 0x00, 0x0c, 0x00, 0x00};

	if(SetupPkt.Recipient != USB_SETUP_RECIPIENT_INTERFACE_BITFIELD) return;

	if(SetupPkt.RequestType != USB_SETUP_TYPE_CLASS_BITFIELD) return;

	mLED_1_On();
	switch(SetupPkt.bRequest)
	{
		case USB_VIDEO_CS_SET_CUR:
			for(i = 0;i < SetupPkt.wLength - 8; ++i)
				buf[i] = CtrlTrfData[i];
			outPipes[0].wCount.Val = SetupPkt.wLength;
			outPipes[0].pDst.bRam = buf;
//			outPipes[0].pFunc = LINE_CODING_PFUNC;
			outPipes[0].info.bits.busy = 1;
			break;
		case USB_VIDEO_CS_GET_CUR:
		case USB_VIDEO_CS_GET_MIN:
		case USB_VIDEO_CS_GET_MAX:
			USBEP0SendRAMPtr( buf, 26, USB_EP0_INCLUDE_ZERO);
			break;
	}
}
#endif
