/************************************************************************
  File Information:
    FileName:       usb_function_video.h

********************************************************************/

extern volatile CTRL_TRF_SETUP SetupPkt;
extern ROM BYTE configDescriptor1[];

// *****************************************************************************
/* Video Class Specific Request Codes.

  Summary:
    Identifies the Video Class Specific Request Codes.

  Description:
    This enumeration identifies the possible Video Class Specific Request codes.

  Remarks:
    The "CS" in the enumeration member names is an acronym for Class Specific.
*/
    
typedef enum
{
    USB_VIDEO_CS_REQUEST_CODE_UNDEFINED = 0x00,
    USB_VIDEO_CS_SET_CUR   = 0x01,
    USB_VIDEO_CS_GET_CUR   = 0x81,
    USB_VIDEO_CS_SET_MIN   = 0x02,
    USB_VIDEO_CS_GET_MIN   = 0x82,
    USB_VIDEO_CS_SET_MAX   = 0x03,
    USB_VIDEO_CS_GET_MAX   = 0x83,
    USB_VIDEO_CS_SET_RES   = 0x04,
    USB_VIDEO_CS_GET_RES   = 0x84,
    USB_VIDEO_CS_SET_MEM   = 0x05,
    USB_VIDEO_CS_GET_MEM   = 0x85,
    USB_VIDEO_CS_GET_INFO  = 0x86,
    USB_VIDEO_CS_GET_DEF   = 0x87,
    USB_VIDEO_CS_GET_STAT  = 0xFF

} USB_VIDEO_CS_REQUEST_CODE;

/* Values for bmHeaderInfo (Video and Still Image Payload Headers, 2.4.3.3) */
#define UVC_STREAM_EOH	(1 << 7)
#define UVC_STREAM_ERR	(1 << 6)
#define UVC_STREAM_STI	(1 << 5)
#define UVC_STREAM_RES	(1 << 4)
#define UVC_STREAM_SCR	(1 << 3)
#define UVC_STREAM_PTS	(1 << 2)
#define UVC_STREAM_EOF	(1 << 1)
#define UVC_STREAM_FID	(1 << 0)

