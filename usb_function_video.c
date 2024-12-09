/************************************************************************
  File Information:
    FileName:       usb_function_video.c

********************************************************************/

#include "HardwareProfile_MX220F032B.h"
#include "./USB/usb.h"
#include "usb_function_video.h"

#ifdef USB_USE_VIDEO_CLASS

void USBCheckVideoRequest()
{
char buf[26];

	if(SetupPkt.Recipient != USB_SETUP_RECIPIENT_INTERFACE_BITFIELD) return;

	if(SetupPkt.RequestType != USB_SETUP_TYPE_CLASS_BITFIELD) return;

	mLED_1_On();
	switch(SetupPkt.bRequest)
	{
		case USB_VIDEO_CS_SET_CUR:
		case USB_VIDEO_CS_GET_MIN:
            outPipes[0].wCount.Val = SetupPkt.wLength;
            outPipes[0].pDst.bRam = buf;
//            outPipes[0].pFunc = LINE_CODING_PFUNC;
            outPipes[0].info.bits.busy = 1;
		break;
	}
}
#endif
