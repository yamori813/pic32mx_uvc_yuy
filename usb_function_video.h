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
