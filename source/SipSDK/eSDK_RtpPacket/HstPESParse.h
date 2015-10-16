/*****************************************************************************
  Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
  FileName: HstPESParse.h
  Author: yangshaobo 5592
  Version : v0.1
  Date:
  Description: Header File of HstPESParse.cpp
  Function List:
  History:
*****************************************************************************/

#ifndef _HSTPESPARSE_
#define _HSTPESPARSE_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef WIN32
#include <arpa/inet.h>
#endif
#include <assert.h>


#ifndef INT64_MIN
#define INT64_MIN  (-9223372036854775807LL - 1)
#endif

#ifndef INT64_C
    #ifdef WIN32
        typedef __int64 INT64_C; 
    #else
        #define INT64_C(n) n##LL
    #endif
#endif

typedef struct _HstPSSystemHeader
{
    unsigned char start_code[4];                // 32 bits, must be 00 00 01 bb
    int           header_length;                // 16 bits;
    unsigned int  rate_bound;                   // 22 bits;
    int           audio_bound;                  // 6 bits;
    int           fixed_flag;                   // 1 bit
    int           CSPS_flag;                    // 1 bit
    int           system_audio_lock_flag;       // 1 bit
    int           system_video_lock_flag;       // 1 bit
    int           video_bound;                  // 5 bits;
    int           packet_rate_restriction_flag; // 1 bits
    int           stream_id;                    // 8 bits;
    int           PS_STD_buffer_bound_scale;    // 1 bit
    int           PS_STD_buffer_size_bound;     // 13 bit
} HstPSSystemHeader;

#define DEMUX_NONE                          0
#define DEMUX_PROGRAM_STREAM                1
#define MAXIMUX_FRAME_LENGTH                (1024*1024)

#define HST_PS_HEADER_V1_LENGTH             6 
#define HST_PS_HEADER_V2_LENGTH             9 


#define VIDEO_BUFFER_LENGTH                 4000000
#define MEDIA_TYPE_MPEG                     0
#define MEDIA_TYPE_MJPEG                    1
#define MEDIA_TYPE_H263                     2

#define SYSTEM_CLOCK_FREQUENCY              27000000

#define PES_PS_HEADER                       0xba

// 1011 1100 1 program_stream_map
#define PES_PROGRAM_STREAM_MAP              0xbc

// 1011 1101 2 private_stream_1
#define PES_PRIVATE_STREAM_1                0xbd

// 1011 1110 padding_stream
#define PES_PADDING_STREAM                  0xbe

// 1011 1111 3 private_stream_2
#define PES_PRIVATE_STREAM_2                0xbf

// 110x xxxx ISO/IEC 13818-3 or ISO/IEC 11172-3 or ISO/IEC 13818-7 or ISO/IEC
//			 14496-3 audio stream number x xxxx
#define PES_ISO13818_FOOT                   0xc0  // 192
#define PES_ISO13818_TOP                    0xdf   // 223

// 1110 xxxx ITU-T Rec. H.262 | ISO/IEC 13818-2 or ISO/IEC 11172-2 or ISO/IEC
//			 14496-2 video stream number xxxx
#define PES_H262_FOOT                       0xe0
#define PES_H262_TOP                        0xef

// 1111 0000 3 ECM_stream
#define PES_ECM_STREAM                      0xf0

// 1111 0001 3 EMM_stream
#define PES_EMM_STREAM                      0xf1

// 1111 0010 5 ITU-T Rec. H.222.0 | ISO/IEC 13818-1 Annex A or ISO/IEC 13818-
//				6_DSMCC_stream
#define PES_ITUT_STREM                      0xf2

// 1111 0011 2 ISO/IEC_13522_stream
#define PES_ISO13522_STREAM                 0xf3

// 1111 0100 6 ITU-T Rec. H.222.1 type A
#define PES_H222A_STREAM                    0xf4

// 1111 0101 6 ITU-T Rec. H.222.1 type B
#define PES_H222B_STREAM                    0xf5

// 1111 0110 6 ITU-T Rec. H.222.1 type C
#define PES_H222C_STREAM                    0xf6

// 1111 0111 6 ITU-T Rec. H.222.1 type D
#define PES_H222D_STREAM                    0xf7

// 1111 1000 6 ITU-T Rec. H.222.1 type E
#define PES_H2221_STREAM                    0xf8

// 1111 1001 7 ancillary_stream
#define PES_ANCILLARY_STREAM                0xf9

// 1111 1010 ISO/IEC14496-1_SL-packetized_stream
#define PES_ISO14496SL_STREAM               0xfa

// 1111 1011 ISO/IEC14496-1_FlexMux_stream
#define PES_ISO14496FM_STREAM               0xfb

// 1111 1100 бн 1111 1110 reserved data stream

// 1111 1111 4 program_stream_directory
#define PES_PROGRAM_STREAM_DIRECTORY        0xff

const int STREAM_ID_NONE = 65535;

#define PES_FAST_FORWARD                    0
#define PES_SLOW_MOTION                     1
#define PES_FREEZE_FRAME                    2
#define PES_FAST_REVERSE                    3
#define PES_SLOW_REVERSE                    4

typedef struct _HstPSVideoFrameBuffer
{
    int            nMaxLength;                      // the maximum length of the buffer
    int            nOffset;                         // the offset to read the buffer
    unsigned char *pPSBuffer;                       // the program video buffer
    int            nLength;                         // the length of the data
    long long      nPosition;                       // position in the original file

    unsigned char *pPayLoad;                        // ps payload data
    int            nPayloadLength;                  // the length of payload length
    int            nMaxPayload;                     // the maximum length of the payload buffer

    unsigned char *pPSFrame;                        // a single video frame
    int            nFrameLength;

    unsigned char *pCurPSPacketPayLoad;             // ps payload data
    int            nCurPSPacketPayloadLength;       // the length of payload length
    int            nCurPSPacketMaxPayload;          // the maximum length of the payload buffer

    int             nTmpLength;

    int             bEOF;

    // init the vars
    _HstPSVideoFrameBuffer()
    {
        nMaxLength = 0;
        nOffset = 0;
        pPSBuffer = NULL;
        nLength = 0;
        nPosition = 0;

        pPayLoad = NULL;
        nPayloadLength = 0;
        nMaxPayload = 0;

        pPSFrame = NULL;
        nFrameLength = 0;

        pCurPSPacketPayLoad = NULL;
        nCurPSPacketPayloadLength = 0;
        nCurPSPacketMaxPayload = 0;

        nTmpLength = 0;

        bEOF = 0;
    }
} HstPSVideoFrameBuffer;

typedef struct _HstPESHeader
{
    unsigned char start_code[3];                        // 24bits, must be 00 00 01
    int           stream_id;                            // 8 bits
    int           pes_packet_length;                    // 16 bits
    int           PES_scrambling_control;               // 2 bits
    int           PES_priority;                         // 2 bits
    int           data_alignment_indicator;             // 1 bit
    int           copyright;                            // 1 bit
    int           original_or_copy;                     // 1 bit
    int           PTS_DTS_flags;                        // 2 bits
    int           ESCR_flag;                            // 1 bit
    int           ES_rate_flag;                         // 1 bit
    int           DSM_trick_mode_flag;                  // 1 bit
    int           additional_copy_info_flag;            // 1 bit
    int           PES_CRC_flag;                         // 1 bit
    int           PES_extension_flag;                   // 1 bit
    int           PES_header_data_length;               // 8 bits
    unsigned int  PTS;                                  // 32 bits
    unsigned int  DTS;                                  // 32 bits
    unsigned int  ESCR_base;                            // 32 bits
    unsigned int  ES_rate;                              // 22 bits
    int           trick_mode_control;                   // 3 bits
    int           field_id;                             // 2 bits
    int           intra_slice_refresh;                  // 1 bit
    int           frequency_truncation;                 // 2 bits
    int           rep_cntrl;                            // 5 bits
    int           additional_copy_info;                 // 7 bits
    int           previous_PES_packet_CRC;              // 16 bits
    int           PES_private_data_flag;                // 1 bits
    int           pack_header_field_flag;               // 1 bit
    int           program_packet_sequence_counter_flag; // 1 bit
    int           PES_STD_buffer_flag;                  // 1 bit
    int           PES_extension_flag_2;                 // 1 bit
    int           PES_private_data[16];                 // 128 bits
    int           pack_field_length;                    // 8 bits
    int           program_packet_sequence_counter;      // 7 bits
    int           MPEG1_MPEG2_identifier;               // 1 bit
    int           original_stuff_length;                // 6 bits
    int           PES_STD_buffer_scale;                 // 1 bit
    int           PES_STD_buffer_size;                  // 13 bits
    int           PES_extension_field_length;           // 7 bits;
    int           PES_packet_data_byte;                 // 8 bits;
    int           padding_byte;                         // 8 bits
} HstPESHeader;

typedef struct _HstPSHeader
{
    unsigned char          start_code[4];                       // 32 bits, must be 00 00 01 ba
    unsigned long long  system_clock_reference_base;            // 33 bits;
    int                    system_clock_reference_extension;    // 1bit
    unsigned int           program_mux_rate;                    // 22 bits;
    int                    pack_stuffing_length;                // 3 bits;
    HstPSSystemHeader      system_header;
} HstPSHeader;

typedef struct _HstGetBitContext
{
    unsigned char *buf;             // buffer for bit analysising
    int            index;           // the byte index
    int            bits;            // the bit index
    int            max;             // max byte in buf
} HstGetBitContext;

typedef struct _HstPSFrame
{
    unsigned char   *buf;               // buffer for frame data byte
    int             length;             // the length of buf has been used
    int             audio_length;
    int             max;                // the max length of buf
    int             first_packet;       // this is the first pes packet of one frame
                                        // the whole frame or not
    int             pts;                // present time of this frame
    int             dts;                // decode time of this frame
    int             type;               // frame type 1:I 0:p/b

    // init the vars
    _HstPSFrame()
    {
        buf = NULL;
        length = 0;
		audio_length = 0;
        max = 0;
        first_packet = 0;

        pts = 0;
		dts = 0;
        type = -1;
    }
    
} HstPSFrame;

class CHstPESParse
{
public:

    CHstPESParse(void);
    virtual ~CHstPESParse(void);

    HstPSVideoFrameBuffer mRawFrameBuffer;

    int  Create();
    int  Delete();

    // some functions for bit stream reading ...
    // initilize the get bit context, put the buffer and size into the context
    void InitGetBits(HstGetBitContext& gb, unsigned char *buf, int nBufLen)const;

    // get nBits from the context
    int  xGetBits(HstGetBitContext& gb, int nBits)const;

    // just show the next nBits from the context
    int  ShowBits(const HstGetBitContext& gb, int nBits)const;

    // parse the program stream packet (MPEG2 PS) and save the payload into
    // frame, be careful, you have to allocate the memory of frame, frame->buf
    // before you call this function
    int  ParsePSPacket(HstPSHeader *header, unsigned char *buf,
                       int bufLength, HstPSFrame *frame,
                       int *stream_id = NULL)const;

    int  ParsePESHeader(HstPESHeader *header, unsigned char *buf, int bufLength, HstPSFrame *frame)const;

    int CheckPSHeader(const unsigned char *buf, int bufLength, unsigned char &ucType)const;
};

#endif  //_HSTPESPARSE_

