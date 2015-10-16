/*****************************************************************************
  Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
  FileName: HstPESParse.cpp
  Author: yangshaobo 5592
  Version : v0.1
  Date:
  Description: Parse PS file , get raw video frame
  Function List:
  History:
*****************************************************************************/
#include "stdafx.h"
#ifdef WIN32 
#include <Winsock2.h>
#pragma warning( disable : 4127 )
#endif

// ´íÎóÂëÍ·ÎÄ¼þ
#include "ivs_error.h"
#include "HstPESParse.h"
#include "eSDKTool.h"
#include "eSDKMem.h"

using namespace std;

CHstPESParse::CHstPESParse(void)
{
    mRawFrameBuffer.nMaxLength = 0;
}

CHstPESParse::~CHstPESParse(void)
{
	try
	{
		(void)Delete();
	}
	catch(...)
	{
	}
}

int CHstPESParse::Create()
{
    // Alloc buffers
    if (mRawFrameBuffer.nMaxLength <= 0)
    {
        HW_NEW_A(mRawFrameBuffer.pPayLoad, unsigned char, MAXIMUX_FRAME_LENGTH);
        HW_NEW_A(mRawFrameBuffer.pCurPSPacketPayLoad, unsigned char, MAXIMUX_FRAME_LENGTH);

        if((NULL == mRawFrameBuffer.pPayLoad) || 
           (NULL == mRawFrameBuffer.pCurPSPacketPayLoad))
        {
            (void)Delete();
            //BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Alloc mem for HstPESParse fail", "");
            return IVS_FAIL;
        }

        mRawFrameBuffer.nMaxPayload = MAXIMUX_FRAME_LENGTH;
        mRawFrameBuffer.nMaxLength = MAXIMUX_FRAME_LENGTH;
        mRawFrameBuffer.nCurPSPacketMaxPayload = MAXIMUX_FRAME_LENGTH;

        mRawFrameBuffer.nOffset = 0;
        mRawFrameBuffer.nLength = 0;
        mRawFrameBuffer.nPayloadLength = 0;
        mRawFrameBuffer.nFrameLength = 0;
        mRawFrameBuffer.nCurPSPacketPayloadLength = 0;
    }

    mRawFrameBuffer.nPayloadLength = 0;
    mRawFrameBuffer.nFrameLength = 0;
    mRawFrameBuffer.nCurPSPacketPayloadLength = 0;

    return IVS_SUCCEED;
}

int CHstPESParse::Delete()
{
    if (mRawFrameBuffer.pCurPSPacketPayLoad)
    {
        HW_DELETE_A(mRawFrameBuffer.pCurPSPacketPayLoad);
        mRawFrameBuffer.nCurPSPacketMaxPayload = 0;
        mRawFrameBuffer.nCurPSPacketPayloadLength = 0;
    }

    if (mRawFrameBuffer.pPSFrame)
    {
        HW_DELETE_A(mRawFrameBuffer.pPSFrame);
        mRawFrameBuffer.nFrameLength = 0;
    }

    if (mRawFrameBuffer.pPayLoad)
    {
        HW_DELETE_A(mRawFrameBuffer.pPayLoad);
        mRawFrameBuffer.nPayloadLength = 0;
    }

    if (mRawFrameBuffer.pPSBuffer)
    {
        HW_DELETE_A(mRawFrameBuffer.pPSBuffer);
        mRawFrameBuffer.nLength = 0;
        mRawFrameBuffer.nMaxLength =0;
        mRawFrameBuffer.nOffset = 0;
        mRawFrameBuffer.nPosition = 0;
    }

    return IVS_SUCCEED;
}

void CHstPESParse::InitGetBits(HstGetBitContext& mGetBitCtx, unsigned char *buf, int nBufLen)const
{
    mGetBitCtx.max   = nBufLen - 1;
    mGetBitCtx.buf   = buf;
    mGetBitCtx.index = mGetBitCtx.bits = 0;
}
//
///*
//   description: get nBits bits from the buffer and move forward the buffer
//   input: HstGetBitContext: the get bit context to be initilized
//          nBits: how many bits you want, should be no more than 24
//   output: none
//   refs: none
//   refed by: none
//   return: on success return the integer value of the desired bits, else -1
// */
int CHstPESParse::xGetBits(HstGetBitContext& mGetBitCtx, int nBits)const
{
    if (nBits > 24)
    {
        return -1;
    }

    if ((mGetBitCtx.index > mGetBitCtx.max) || (mGetBitCtx.index + nBits / 8 + 1 > mGetBitCtx.max))
    {
        return -1;
    }

    unsigned int p = htonl(((unsigned int *) (mGetBitCtx.buf + mGetBitCtx.index))[0]); //lint !e826
    unsigned int k = p >> ((32 - mGetBitCtx.bits) - nBits);
    unsigned int m = 0xffffffff >> (32 - nBits);
    unsigned int result = k & m;
    mGetBitCtx.index += (nBits / 8);
    mGetBitCtx.bits += (nBits % 8);

    if (mGetBitCtx.bits >= 8)
    {
        mGetBitCtx.index += 1;
        mGetBitCtx.bits -= 8;
    }

    return (int)result;
}

//
///*
//   description: show nBits bits from the buffer
//   input: HstGetBitContext: the get bit context to be initilized
//          nBits: how many bits you want, should be no more than 24
//   output: none
//   refs: none
//   refed by: none
//   return: on success return the integer value of the desired bits, else -1
// */
int CHstPESParse::ShowBits(const HstGetBitContext& mGetBitCtx, int nBits)const
{
    if (nBits > 24)
    {
        return -1;
    }

    if ((mGetBitCtx.index > mGetBitCtx.max) || (mGetBitCtx.index + nBits / 8 + 1 > mGetBitCtx.max))
    {
        return -1;
    }

    unsigned int p = htonl(((unsigned int *) (mGetBitCtx.buf + mGetBitCtx.index))[0]); //lint !e826
    unsigned int k = p >> ((32 - mGetBitCtx.bits) - nBits);
    unsigned int m = 0xffffffff >> (32 - nBits);
    unsigned int result = k & m;

    return (int)result;
}

//*
//   description: parse a program element stream packet, and save the PES
//                head information into header and get out the payload media
//                bytes into frame.
//   input: buf: the buffer to be analysised
//          bufLength: the length of buf
//   output: header: the buffer for PES header
//           frame: the buffer for media bytes, be careful, you have to allocate
//                  frame->buf before you call this function
//   refs: none
//   refed by: none
//   return: on success return how many bytes been analyzed, else -1
// */
int CHstPESParse::ParsePESHeader(HstPESHeader *header, unsigned char *buf,
                                int bufLength, HstPSFrame *frame) const
{
    //CHECK_POINTER(header, -1);
    //CHECK_POINTER(buf, -1);
    //CHECK_POINTER(frame, -1);
    //CHECK_POINTER(frame->buf, -1);

    if (bufLength < 3)
    {
        //BP_RUN_LOG_INF("Invalid input , please check it", "leng=%d", bufLength);
        return -1;
    }
    
    frame->length = 0;
    frame->first_packet = 0;

    memset(header, 0, sizeof(HstPESHeader));
    HstGetBitContext mGetBitCtx;
    InitGetBits(mGetBitCtx, buf, bufLength);

    while (1) //lint !e716
    {
        char prefix[3] = {0, 0, 1};         // set prefix to PES start code
        if (memcmp(mGetBitCtx.buf + mGetBitCtx.index, prefix, 3) || (mGetBitCtx.max - mGetBitCtx.index < 3))
        {
            break;
        }
        
        if(mGetBitCtx.max <= 0)
        {
            break;
        }
        
        if (mGetBitCtx.buf[3 + mGetBitCtx.index] == 0xba)
        {
            break;
        }

        if (xGetBits(mGetBitCtx, 24) < 0)
        {
            //BP_RUN_LOG_INF("Get bits fail", "");
            return -1;
        }

        if ((header->stream_id = xGetBits(mGetBitCtx, 8)) < 0)
        {
            //BP_RUN_LOG_INF("Get stream id fail", "stream id=%d", header->stream_id);
            return -1;
        }

        // important!! for calculate the actual media byte length
        if ((header->pes_packet_length = xGetBits(mGetBitCtx, 16)) < 0)
        {
            //BP_RUN_LOG_INF("Get ps packet len fail", "ps packet len=%d", header->pes_packet_length);
            return -1;
        }

        if ((header->stream_id != PES_PROGRAM_STREAM_MAP)
            && (header->stream_id != PES_PADDING_STREAM)
            && (header->stream_id != PES_PRIVATE_STREAM_1)
            && (header->stream_id != PES_PRIVATE_STREAM_2)
            && (header->stream_id != PES_ECM_STREAM)
            && (header->stream_id != PES_EMM_STREAM)
            && (header->stream_id != PES_PROGRAM_STREAM_DIRECTORY)
            && (header->stream_id != PES_ITUT_STREM)
            && (header->stream_id != PES_H2221_STREAM))
            //&&(header->stream_id != PES_ISO13818_FOOT))
        {
            if (xGetBits(mGetBitCtx, 2) != 2)
            {
                //BP_RUN_LOG_INF("Get ps packet bit ctc fail", "");
                return -1;
            }

            if ((header->PES_scrambling_control = xGetBits(mGetBitCtx, 2)) < 0)
            {
                //BP_RUN_LOG_INF("Get ps scrambling control fail", "ps scrambling control=%d", header->PES_scrambling_control);
                return -1;
            }

            if ((header->PES_priority = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get ps priority fail", "ps priority=%d", header->PES_priority);
                return -1;
            }

            if ((header->data_alignment_indicator = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get ps alignment indicator fail", "alignment indicator=%d", header->data_alignment_indicator);
                return -1;
            }

            if ((header->copyright = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get ps copyright fail", "copyright=%d", header->copyright);            
                return -1;
            }

            if ((header->original_or_copy = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get original or copy fail", "original or copy=%d", header->original_or_copy);            
                return -1;
            }

            if ((header->PTS_DTS_flags = xGetBits(mGetBitCtx, 2)) < 0)
            {
                //BP_RUN_LOG_INF("Get dts flag fail", "dts flag=%d", header->PTS_DTS_flags);            
                return -1;
            }

            if ((header->ESCR_flag = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get escr flag fail", "escr flag=%d", header->ESCR_flag);            
                return -1;
            }

            if ((header->ES_rate_flag = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get es rate flag fail", "es rate flag=%d", header->ES_rate_flag);            
                return -1;
            }

            if ((header->DSM_trick_mode_flag = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get tick mode flag fail", "tick mode flag=%d", header->DSM_trick_mode_flag);            
                return -1;
            }

            if ((header->additional_copy_info_flag = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get copy info fail", "copy info=%d", header->additional_copy_info_flag);            
                return -1;
            }

            if ((header->PES_CRC_flag = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get pes crc flag fail", "pes crc flag=%d", header->PES_CRC_flag);            
                return -1;
            }

            if ((header->PES_extension_flag = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get pes extension flag fail", "pes extension flag=%d", header->PES_extension_flag);            
                return -1;
            }

            if ((header->PES_header_data_length = xGetBits(mGetBitCtx, 8)) < 0)
            {
                //BP_RUN_LOG_INF("Get pes header data length fail", "pes header data length=%d", header->PES_header_data_length);            
                return -1;
            }

            int nOptionalFieldsBitLength = 0;

            if (header->PTS_DTS_flags == 2)
            {
                nOptionalFieldsBitLength += 40;
                if (xGetBits(mGetBitCtx, 4) != 2)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", ""); 
                    return -1;
                }

                // god save me, we don't need the pts, so just drop it, total 36 bits

                /*
                if(xGetBits(mGetBitCtx, 16) < 0)
                    return -1;
                if(xGetBits(mGetBitCtx, 20) < 0)
                    return -1;
                 */
                int pts, kk;
                if ((pts = xGetBits(mGetBitCtx, 3)) < 0)
                {
                    //BP_RUN_LOG_INF("Get pts fail", "pts=%d", pts);                
                    return -1;
                }

                pts <<= 30; //lint !e701

                if (xGetBits(mGetBitCtx, 1) != 1)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                 
                    return -1;
                }

                if ((kk = xGetBits(mGetBitCtx, 15)) < 0)
                {
                    //BP_RUN_LOG_INF("Get kk fail", "kk=%d", kk);                 
                    return -1;
                }

                if (xGetBits(mGetBitCtx, 1) != 1)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                 
                    return -1;
                }

                kk <<= 15;  //lint !e701
                pts = pts | kk;

                if ((kk = xGetBits(mGetBitCtx, 15)) < 0)
                {
                    //BP_RUN_LOG_INF("Get kk fail", "kk=%d", kk);                
                    return -1;
                }

                pts |= kk;

                if (xGetBits(mGetBitCtx, 1) != 1)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                    return -1;
                }

                if (header->stream_id == 0xe0)
                {
                    frame->first_packet = 1;
                    frame->pts = pts;
                }
            }
            else if (header->PTS_DTS_flags == 3)
            {
                nOptionalFieldsBitLength += 80;

                if (xGetBits(mGetBitCtx, 4) != 3) // 0011
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }
                
                // Get PTS 
                int pts, kk;
                if ((pts = xGetBits(mGetBitCtx, 3)) < 0)
                {
                    //BP_RUN_LOG_INF("Get pts fail", "pts=%d", pts);               
                    return -1;
                }

                pts <<= 30;  //lint !e701

                if (xGetBits(mGetBitCtx, 1) != 1)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }

                if ((kk = xGetBits(mGetBitCtx, 15)) < 0)
                {
                    //BP_RUN_LOG_INF("Get kk fail", "kk=%d", kk);                 
                    return -1;
                }

                if (xGetBits(mGetBitCtx, 1) != 1)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }

                kk <<= 15;  //lint !e701
                pts = pts | kk;

                if ((kk = xGetBits(mGetBitCtx, 15)) < 0)
                {
                    //BP_RUN_LOG_INF("Get kk fail", "kk=%d", kk);                
                    return -1;
                }

                pts |= kk;

                if (xGetBits(mGetBitCtx, 1) != 1) // maker bit 
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }

                if (xGetBits(mGetBitCtx, 4) != 1) // 0001
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }
                
                // Get DTS 
                int dts, jj;
                if ((dts = xGetBits(mGetBitCtx, 3)) < 0)
                {
                    //BP_RUN_LOG_INF("Get dts fail", "dts=%d", dts);               
                    return -1;
                }

                dts <<= 30;  //lint !e701

                if (xGetBits(mGetBitCtx, 1) != 1)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }

                if ((jj = xGetBits(mGetBitCtx, 15)) < 0)
                {
                    //BP_RUN_LOG_INF("Get jj fail", "jj=%d", jj);                
                    return -1;
                }

                if (xGetBits(mGetBitCtx, 1) != 1)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                    return -1;
                }

                jj <<= 15;  //lint !e701
                dts = dts | jj;

                if ((jj = xGetBits(mGetBitCtx, 15)) < 0)
                {
                    //BP_RUN_LOG_INF("Get jj fail", "jj=%d", jj);                 
                    return -1;
                }

                dts |= jj;

                if (xGetBits(mGetBitCtx, 1) != 1) // maker bit 
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }

                if (header->stream_id == 0xe0)
                {
                    frame->first_packet = 1;
                    frame->pts = pts;
                    frame->dts = dts;
                }
            }
            else
            {
            }

            if (header->ESCR_flag == 1)
            {
                nOptionalFieldsBitLength += 48;

                if (xGetBits(mGetBitCtx, 2) < 0)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }

                // we don't need the ESCR base, drop it, total 46 bits
                if (xGetBits(mGetBitCtx, 24) < 0)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }

                if (xGetBits(mGetBitCtx, 12) < 0)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }
            }

            if (header->ES_rate_flag == 1)
            {
                nOptionalFieldsBitLength += 24;

                if (xGetBits(mGetBitCtx, 1) < 0)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");              
                    return -1;
                }

                int iTmpRet = xGetBits(mGetBitCtx, 22);
                if (iTmpRet < 0) 
                {
                    //BP_RUN_LOG_INF("Get es rate fail", "es rate=%d", header->ES_rate);                     
                    return -1;
                }
                header->ES_rate = (unsigned int)iTmpRet;  //lint !e685 !e568

                if (xGetBits(mGetBitCtx, 1) < 0)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                    return -1;
                }
            }

            if (header->DSM_trick_mode_flag == 1)
            {
                nOptionalFieldsBitLength += 3;

                if ((header->trick_mode_control = xGetBits(mGetBitCtx, 3)) < 0)
                {
                    //BP_RUN_LOG_INF("Get trick mode control fail", "trick mode control=%d", header->trick_mode_control);                
                    return -1;
                }

                if ((header->trick_mode_control == PES_FAST_FORWARD)
                    || (header->trick_mode_control == PES_FAST_REVERSE))
                {
                    nOptionalFieldsBitLength += 5;

                    if ((header->field_id = xGetBits(mGetBitCtx, 2)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get field id fail", "field id=%d", header->field_id);                
                        return -1;
                    }

                    if ((header->intra_slice_refresh = xGetBits(mGetBitCtx, 1)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get intra slice refresh fail", "intra slice refresh=%d", header->intra_slice_refresh);                
                        return -1;
                    }

                    if ((header->frequency_truncation = xGetBits(mGetBitCtx, 2)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get frequency truncation fail", "frequency truncation=%d", header->frequency_truncation);            
                        return -1;
                    }
                }
                else if ((header->trick_mode_control == PES_SLOW_MOTION)
                         || (header->trick_mode_control == PES_SLOW_REVERSE))
                {
                    nOptionalFieldsBitLength += 5;
                    if ((header->rep_cntrl = xGetBits(mGetBitCtx, 5)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get rep cntrl fail", "rep cntrl truncation=%d", header->rep_cntrl);              
                        return -1;
                    }
                }
                else if (header->trick_mode_control == PES_FREEZE_FRAME)
                {
                    nOptionalFieldsBitLength += 5;

                    if ((header->field_id = xGetBits(mGetBitCtx, 2)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get field id fail", "field id=%d", header->field_id);                
                        return -1;
                    }

                    if (xGetBits(mGetBitCtx, 3) < 0)
                    {
                        //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                        return -1;
                    }
                }
                else
                {
                    nOptionalFieldsBitLength += 5;

                    if (xGetBits(mGetBitCtx, 5) < 0)
                    {
                        //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                        return -1;
                    }
                }
            }

            if (header->additional_copy_info_flag == 1)
            {
                nOptionalFieldsBitLength += 8;
                char c;
                if ((c = (char)xGetBits(mGetBitCtx, 8)) < 0) //lint !e734
                {
                    //BP_RUN_LOG_INF("Get c fail", "c=%d", c);              
                    return -1;
                }

                header->additional_copy_info = c & 0x7f;
            }

            if (header->PES_CRC_flag == 1)
            {
                nOptionalFieldsBitLength += 16;
                if ((header->previous_PES_packet_CRC = xGetBits(mGetBitCtx, 16)) < 0)
                {
                    //BP_RUN_LOG_INF("Get es src fail", "es src=%d", header->previous_PES_packet_CRC);               
                    return -1;
                }
            }

            if (header->PES_extension_flag == 1)
            {
                nOptionalFieldsBitLength += 8;

                if ((header->PES_private_data_flag = xGetBits(mGetBitCtx, 1)) < 0)
                {
                    //BP_RUN_LOG_INF("Get data flag fail", "data flag=%d", header->PES_private_data_flag);                
                    return -1;
                }

                if ((header->pack_header_field_flag = xGetBits(mGetBitCtx, 1)) < 0)
                {
                    //BP_RUN_LOG_INF("Get field flag fail", "field flag=%d", header->pack_header_field_flag);                 
                    return -1;
                }

                if ((header->program_packet_sequence_counter_flag = xGetBits(mGetBitCtx, 1)) < 0)
                {
                    //BP_RUN_LOG_INF("Get packet sequence counter fail", "packet sequence counter=%d", header->program_packet_sequence_counter_flag);                
                    return -1;
                }

                if ((header->PES_STD_buffer_flag = xGetBits(mGetBitCtx, 1)) < 0)
                {
                    //BP_RUN_LOG_INF("Get std buffer flag fail", "std buffer flag=%d", header->PES_STD_buffer_flag);                
                    return -1;
                }

                if (xGetBits(mGetBitCtx, 3) < 0)
                {
                    //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                    return -1;
                }

                if ((header->PES_extension_flag_2 = xGetBits(mGetBitCtx, 1)) < 0)
                {
                    //BP_RUN_LOG_INF("Get extention flag fail", "extention flag=%d", header->PES_extension_flag_2);                
                    return -1;
                }

                if (header->PES_private_data_flag == 1)
                {
                    nOptionalFieldsBitLength += 128;

                    for (int i = 0; i < 16; i++)
                    {
                        if ((header->PES_private_data[i] = xGetBits(mGetBitCtx, 8)) < 0)
                        {
                            //BP_RUN_LOG_INF("Get private data fail", "private data=%d", header->PES_private_data[i]);                
                            return -1;
                        }
                    }
                }

                if (header->pack_header_field_flag == 1)
                {
                    nOptionalFieldsBitLength += 8;

                    if ((header->pack_field_length = xGetBits(mGetBitCtx, 8)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get field len fail", "field len=%d", header->pack_field_length);                
                        return -1;
                    }
                }

                if (header->program_packet_sequence_counter_flag == 1)
                {
                    nOptionalFieldsBitLength += 16;

                    if (xGetBits(mGetBitCtx, 1) != 1)
                    {
                        //BP_RUN_LOG_INF("Get bit ctx fail", "");              
                        return -1;
                    }

                    if ((header->program_packet_sequence_counter = xGetBits(mGetBitCtx, 7)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get seq counter fail", "seq counter=%d", header->program_packet_sequence_counter);                
                        return -1;
                    }

                    if (xGetBits(mGetBitCtx, 1) != 1)
                    {
                        //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                        return -1;
                    }

                    if ((header->MPEG1_MPEG2_identifier = xGetBits(mGetBitCtx, 1)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get identifier fail", "identifier=%d", header->MPEG1_MPEG2_identifier);                
                        return -1;
                    }

                    if ((header->original_stuff_length = xGetBits(mGetBitCtx, 6)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get buff len fail", "buff len=%d", header->original_stuff_length);                
                        return -1;
                    }
                }

                if (header->PES_STD_buffer_flag == 1)
                {
                    nOptionalFieldsBitLength += 16;

                    if (xGetBits(mGetBitCtx, 2) != 1)
                    {
                        //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                        return -1;
                    }

                    if ((header->PES_STD_buffer_scale = xGetBits(mGetBitCtx, 1)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get buff scale fail", "buff scale=%d", header->PES_STD_buffer_scale);                
                        return -1;
                    }

                    if ((header->PES_STD_buffer_size = xGetBits(mGetBitCtx, 13)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get buff size fail", "buff szie=%d", header->PES_STD_buffer_size);                
                        return -1;
                    }
                }

                if (header->PES_extension_flag_2 == 1)
                {
                    nOptionalFieldsBitLength += 8;

                    if (xGetBits(mGetBitCtx, 1) < 0)
                    {
                        //BP_RUN_LOG_INF("Get bit ctx fail", "");                
                        return -1;
                    }

                    if ((header->PES_extension_field_length = xGetBits(mGetBitCtx, 7)) < 0)
                    {
                        //BP_RUN_LOG_INF("Get field len fail", "field len=%d", header->PES_extension_field_length);                
                        return -1;
                    }

                    for (int i = 0; i < header->PES_extension_field_length; i++)
                    {
                        nOptionalFieldsBitLength += 8;
                        if (xGetBits(mGetBitCtx, 8) < 0)
                        {
                            //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                            return -1;
                        }
                    }
                }
            }

            // ATTENTION: this part has something different from ISO13818
            // on the paper of ISO: the stuffing byte will be exactly equal to 0xff and
            // no less than 32 bits, while here, we always drop 24 stuffing
            // bits, so IF ANYTHING WRONG WITH THE PICTURE, PLEASE LOOK AT
            // THIS FIRSTLY!!!!
            //int cc;

            //for(int i = 0; i < 4 ; i++) {
            //	cc = ShowBits(mGetBitCtx, 8);
            //	if(cc == 0 || cc < 0xb0) //{
            //		break;

            int nStuffingBytesLength =
                header->PES_header_data_length - nOptionalFieldsBitLength / 8;
            if (header->PES_header_data_length > 0)
            {
                for (int i = 0; i < nStuffingBytesLength; i++)
                {
                    if( (ShowBits(mGetBitCtx, 8) == 0xff) ||
                        (ShowBits(mGetBitCtx, 8) == 0xfe) ||
                        (ShowBits(mGetBitCtx, 8) == 0xfc) ||
                        (ShowBits(mGetBitCtx, 8) == 0xfd) ||
                        (ShowBits(mGetBitCtx, 8) == 0xfa) ||
                        (ShowBits(mGetBitCtx, 8) == 0xf8))
                    {
                        if (xGetBits(mGetBitCtx, 8) < 0)
                        {
                            //BP_RUN_LOG_INF("Get bit ctx fail", "");               
                            return -1;
                        }
                    }
                    else
                    {
                        break;
                    }
                }


            }
            
            // here coming the real data
            int copyLength = header->pes_packet_length - (mGetBitCtx.index - 6);
            copyLength = (frame->max - frame->length > copyLength) ? copyLength :
                         (frame->max - frame->length);
            copyLength = (((mGetBitCtx.max - mGetBitCtx.index) + 1) >= copyLength) ? copyLength :
                         ((mGetBitCtx.max - mGetBitCtx.index) + 1);

            if (header->stream_id == PES_ISO13818_FOOT)
            {
                memcpy(frame->buf + frame->length, mGetBitCtx.buf + mGetBitCtx.index, (unsigned int)copyLength);
                //frame->length += copyLength;
                frame->audio_length += copyLength;
                mGetBitCtx.index += header->pes_packet_length - (mGetBitCtx.index - 6);
            }
            else
            {
                if ((copyLength <= 0) || (header->pes_packet_length < 0))
                {
                    //BP_RUN_LOG_INF("Length is invalid", "");
                    return -1;
                }

                memcpy(frame->buf + frame->length, mGetBitCtx.buf + mGetBitCtx.index, (unsigned int)copyLength);
                frame->length    += copyLength;
                mGetBitCtx.index += header->pes_packet_length - (mGetBitCtx.index - 6);
            }

        }
        else if ((header->stream_id == PES_PROGRAM_STREAM_MAP)
                 || (header->stream_id == PES_PRIVATE_STREAM_1)
                 || (header->stream_id == PES_PRIVATE_STREAM_2)
                 || (header->stream_id == PES_ECM_STREAM)
                 || (header->stream_id == PES_EMM_STREAM)
                 || (header->stream_id == PES_PROGRAM_STREAM_DIRECTORY)
                 || (header->stream_id == PES_ITUT_STREM)
                 || (header->stream_id == PES_H2221_STREAM))
                 //|| (header->stream_id == PES_ISO13818_FOOT))
        {

            //if (header->stream_id == PES_ISO13818_FOOT)
            //{
            //    int copyLength = (frame->max - frame->length > header->pes_packet_length) ?
            //        header->pes_packet_length : (frame->max - frame->length);
            //    memcpy(frame->buf + frame->length, mGetBitCtx.buf + mGetBitCtx.index, (unsigned int)copyLength);
            //    //frame->length += copyLength;
            //    frame->audio_length += copyLength;

            //}
            

            if (header->pes_packet_length < 0)
            {
                //BP_RUN_LOG_INF("Length is invalid", "");                
                return -1;
            }

            mGetBitCtx.index += header->pes_packet_length;
        }
        else if (header->stream_id == PES_PADDING_STREAM)
        {
            if (header->pes_packet_length < 0)
            {
                //BP_RUN_LOG_INF("Length is invalid", "");                
                return -1;
            }

            mGetBitCtx.index += header->pes_packet_length;      // drop the padding bytes
        }

        // reset the mGetBitCtx context
        InitGetBits(mGetBitCtx, mGetBitCtx.buf + mGetBitCtx.index, (mGetBitCtx.max - mGetBitCtx.index) + 1);
    }

    return mGetBitCtx.buf + mGetBitCtx.index - buf;
}


int CHstPESParse::CheckPSHeader(const unsigned char *buf, int bufLength, unsigned char &ucType)const
{
    //CHECK_POINTER(buf, IVS_FAIL);

    if (bufLength < 4)
    {
        //BP_RUN_LOG_INF("Length is invalid", "len=%d", bufLength);
        return IVS_FAIL;
    }

    ucType = 0;
    unsigned char nPSHeaderPrefix[4] = {0, 0, 1, 0xba};      // ps header

    if (0 == memcmp(buf, nPSHeaderPrefix, 4))
    {
        ucType = PES_PS_HEADER;
        return IVS_SUCCEED;
    }

    unsigned char nPESHeaderVideo[4] = {0, 0, 1, 0xe0};      // ps header

    if (0 == memcmp(buf, nPESHeaderVideo, 4))
    {
        ucType = PES_H262_FOOT;
        return IVS_SUCCEED;
    }

    unsigned char nPSHeaderAudio[4] = {0, 0, 1, 0xc0};      // ps header

    if (0 == memcmp(buf, nPSHeaderAudio, 4))
    {
        ucType = PES_ISO13818_FOOT;
        return IVS_SUCCEED;
    }

    return IVS_FAIL;
}

/*
   description: parse a program stream packet, and save the PS
                head information into header and get out the payload media
                bytes into frame.
   input: buf: the buffer to be analysised
          bufLength: the length of buf
   output: header: the buffer for PS header
           frame: the buffer for media bytes, be careful, you have to allocate
                  frame->buf before you call this function
   refs: none
   refed by: none
   return: on success return howmany bytes been analyzed, else -1
 */
int CHstPESParse::ParsePSPacket(HstPSHeader *header,
                               unsigned char *buf, int bufLength, HstPSFrame *frame,
                               int *stream_id)const
{
    //CHECK_POINTER(header, -1);
    //CHECK_POINTER(buf, -1);
    //CHECK_POINTER(frame, -1);
    //CHECK_POINTER(frame->buf, -1);

    if (bufLength < 4)
    {
        //BP_RUN_LOG_INF("Length is invalid", "len=%d", bufLength);
        return -1;
    }
    
    frame->length = 0;

    unsigned char ucType = 0;
    int iRet = CheckPSHeader(buf, bufLength, ucType);
    if (IVS_SUCCEED != iRet)
    {
        //BP_RUN_LOG_INF("Check ps header fail", "");
        return -1;
    }

    unsigned char nPSHeaderPrefix[4] = {0, 0, 1, 0xba};      // ps header
    if ((PES_H262_FOOT == ucType)
        ||(PES_ISO13818_FOOT == ucType))
    {
        HstPESHeader pesHeader;
		int parsed_length = ParsePESHeader(&pesHeader, buf,	bufLength, frame);
        if (parsed_length  > 0)
        {
            *stream_id = pesHeader.stream_id; //lint !e644
            return parsed_length;
        }
    }
    
    HstGetBitContext mGetBitCtx;

    InitGetBits(mGetBitCtx, buf + 4, bufLength - 4);

    if (xGetBits(mGetBitCtx, 2) != 1)
    {
        //BP_RUN_LOG_INF("Get bit ctx fail", "");
        return -1;
    }

    // get clock base
    int c;
    header->system_clock_reference_base = 0;
    if ((c = xGetBits(mGetBitCtx, 3)) < 0)
    {
        //BP_RUN_LOG_INF("Get c fail", "c=%d", c);
        return -1;
    }

    int *p = (int *) (&header->system_clock_reference_base);
    *p = (*p) | c;

    if (xGetBits(mGetBitCtx, 1) != 1)
    {
        //BP_RUN_LOG_INF("Get bit ctx fail", "");
        return -1;
    }

    if ((c = xGetBits(mGetBitCtx, 15)) < 0)
    {
        // the mid 15 bits, 15~29
        //BP_RUN_LOG_INF("Get c fail", "c=%d", c);
        return -1;
    }

    header->system_clock_reference_base <<= 15;
    header->system_clock_reference_base &= 0x38000;
    *p = (*p) | c;

    if (xGetBits(mGetBitCtx, 1) != 1)
    {
        // the lower 15 bits, 0~14
		//BP_RUN_LOG_INF("Get bit ctx fail", "");
        return -1;
    }

    if ((c = xGetBits(mGetBitCtx, 15)) < 0)
    {
        //BP_RUN_LOG_INF("Get c fail", "c=%d", c);
        return -1;
    }

    header->system_clock_reference_base <<= 15;
    header->system_clock_reference_base &= 0x38000;
    *p = (*p) | c;

    if (xGetBits(mGetBitCtx, 1) != 1)
    {
        //BP_RUN_LOG_INF("Get bit ctx fail", "");
        return -1;
    }

    if ((header->system_clock_reference_extension = xGetBits(mGetBitCtx, 9)) < 0)
    {
        //BP_RUN_LOG_INF("Get extension fail", "extension=%d", header->system_clock_reference_extension);
        return -1;
    }

    if (xGetBits(mGetBitCtx, 1) != 1)
    {
		//BP_RUN_LOG_INF("Get bit ctx fail", "");
        return -1;
    }

    int uiTmpRet = xGetBits(mGetBitCtx, 22);
    if (uiTmpRet < 0) 
    {
        //BP_RUN_LOG_INF("Get mux rate fail", "mux rate=%d", header->program_mux_rate);
        return -1;
    }
    header->program_mux_rate = (unsigned int)uiTmpRet;  //lint !e732 !e685 !e568

    if (xGetBits(mGetBitCtx, 7) < 0)
    {
        //BP_RUN_LOG_INF("Get bit ctx fail", "");
        return -1;
    }

    if ((header->pack_stuffing_length = xGetBits(mGetBitCtx, 3)) < 0)
    {
        //BP_RUN_LOG_INF("Get stuff length fail", "stuff length=%d", header->pack_stuffing_length);
        return -1;
    }

    for (int i = 0; i < header->pack_stuffing_length; i++)
    {
        if (xGetBits(mGetBitCtx, 8) < 0)
        {
            //BP_RUN_LOG_INF("Get bit ctx fail", "");
            return -1;
        }
    }

    // check if we have system header or not
    int offset = 14 + header->pack_stuffing_length;
    nPSHeaderPrefix[3] = 0xbb;           // set prefix to system header start code
    HstPSSystemHeader *sheader = &(header->system_header);
    if (memcmp(buf + offset, nPSHeaderPrefix, 4) == 0)
    {
        // system header
        if (xGetBits(mGetBitCtx, 16) < 0)
        {
            //BP_RUN_LOG_INF("Get bit ctx fail", "");
            return -1;
        }

        if (xGetBits(mGetBitCtx, 16) < 0)
        {
            //BP_RUN_LOG_INF("Get bit ctx fail", "");
            return -1;
        }

        if ((sheader->header_length = xGetBits(mGetBitCtx, 16)) < 0)
        {
            //BP_RUN_LOG_INF("Get header length fail", "header length=%d", sheader->header_length);
            return -1;
        }

        if (xGetBits(mGetBitCtx, 1) < 0)
        {
            //BP_RUN_LOG_INF("Get bit ctx fail", "");
            return -1;
        }
		int rate_tmp = xGetBits(mGetBitCtx, 22);
        //if ( rate_tmp < 0)  //lint !e732 !e685 !e568
        //{
        //    //BP_RUN_LOG_INF("Get rate bound fail", "rate bound=%d", sheader->rate_bound);
        //    return -1;
        //}
		sheader->rate_bound = (unsigned int)rate_tmp;
        if (xGetBits(mGetBitCtx, 1) < 0)
        {
            //BP_RUN_LOG_INF("Get bit ctx fail", "");
            return -1;
        }

        if ((sheader->audio_bound = xGetBits(mGetBitCtx, 6)) < 0)
        {
            //BP_RUN_LOG_INF("Get audio bound fail", "audio bound=%d", sheader->audio_bound);
            return -1;
        }

        if ((sheader->fixed_flag = xGetBits(mGetBitCtx, 1)) < 0)
        {
            //BP_RUN_LOG_INF("Get fixed flag fail", "fixed flag=%d", sheader->fixed_flag);
            return -1;
        }

        if ((sheader->CSPS_flag = xGetBits(mGetBitCtx, 1)) < 0)
        {
            //BP_RUN_LOG_INF("Get CSPS flag fail", "CSPS flag=%d", sheader->CSPS_flag);
            return -1;
        }

        if ((sheader->system_audio_lock_flag = xGetBits(mGetBitCtx, 1)) < 0)
        {
            //BP_RUN_LOG_INF("Get audio lock flag fail", "audio lock flag=%d", sheader->system_audio_lock_flag);
            return -1;
        }

        if ((sheader->system_video_lock_flag = xGetBits(mGetBitCtx, 1)) < 0)
        {
            //BP_RUN_LOG_INF("Get video lock flag fail", "video lock flag=%d", sheader->system_video_lock_flag);
            return -1;
        }

        if (xGetBits(mGetBitCtx, 1) < 0)
        {
            //BP_RUN_LOG_INF("Get bit ctx fail", "");
            return -1;
        }

        if ((sheader->video_bound = xGetBits(mGetBitCtx, 5)) < 0)
        {
            //BP_RUN_LOG_INF("Get video bound fail", "video bound=%d", sheader->video_bound);
            return -1;
        }

        if ((sheader->packet_rate_restriction_flag = xGetBits(mGetBitCtx, 1)) < 0)
        {
            //BP_RUN_LOG_INF("Get rate flag fail", "rate flag=%d", sheader->packet_rate_restriction_flag);
            return -1;
        }

        if (xGetBits(mGetBitCtx, 7) < 0)
        {
            //BP_RUN_LOG_INF("Get bit ctx fail", "");
            return -1;
        }

        if (ShowBits(mGetBitCtx, 1) == 1)
        {
            if ((sheader->stream_id = xGetBits(mGetBitCtx, 8)) < 0)
            {
                //BP_RUN_LOG_INF("Get stream id fail", "stream id=%d", sheader->stream_id);
                return -1;
            }

            if (xGetBits(mGetBitCtx, 2) != 3)
            {
                //BP_RUN_LOG_INF("Get bit ctx fail", "");
                return -1;
            }

            if ((sheader->PS_STD_buffer_bound_scale = xGetBits(mGetBitCtx, 1)) < 0)
            {
                //BP_RUN_LOG_INF("Get std buff scale fail", "std buff scale=%d", sheader->PS_STD_buffer_bound_scale);
                return -1;
            }

            if ((sheader->PS_STD_buffer_size_bound = xGetBits(mGetBitCtx, 13)) < 0)
            {
                //BP_RUN_LOG_INF("Get std buff size fail", "std buff size=%d", sheader->PS_STD_buffer_size_bound);
                return -1;
            }

            // offset += 15;		// modified by jerrifer 2011.04.22
            offset += 6 + sheader->header_length;
        }
        else
        {
            // offset += 12;
            offset += 6 + sheader->header_length;
        }                                         // modified by jerrifer 2011.04.22
    }

    nPSHeaderPrefix[2] = 1;          // set prefix to 00 00 01 for pes start code
    int parsed_length = 0;
    // pes packet
    HstPESHeader pesHeader;
    (void)::memset(&pesHeader, 0, sizeof(pesHeader));
    if (memcmp(buf + offset, nPSHeaderPrefix, 3) == 0)
    {
        // analysis all PES packets in this PS packet
        if ((parsed_length = ParsePESHeader(&pesHeader, buf + offset,
                                            bufLength - offset, frame)) > 0)
        {
            offset += parsed_length;
        }
    }

    *stream_id = pesHeader.stream_id; //lint !e644

    return offset;
}

#ifdef WIN32 
#pragma warning( default : 4127 )
#endif

