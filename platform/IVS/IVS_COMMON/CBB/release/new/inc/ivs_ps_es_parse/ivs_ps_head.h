#ifndef CBB_PS_HEAD
#define CBB_PS_HEAD

#pragma pack(push, 1)
typedef struct PES_SYSTEM_HEADER
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
} PES_SYSTEM_HEADER_S, *LPPS_SYSTEM_HEADER;

typedef struct PES_HEADER
{
    unsigned char start_code[3];                // 24bits, must be 00 00 01
    int           stream_id;                    // 8 bits
    int           pes_packet_length;            // 16 bits
    int           PES_scrambling_control;       // 2 bits
    int           PES_priority;                 // 2 bits
    int           data_alignment_indicator;     // 1 bit
    int           copyright;                    // 1 bit
    int           original_or_copy;             // 1 bit
    int           PTS_DTS_flags;                // 2 bits
    int           ESCR_flag;                    // 1 bit
    int           ES_rate_flag;                 // 1 bit
    int           DSM_trick_mode_flag;          // 1 bit
    int           additional_copy_info_flag;    // 1 bit
    int           PES_CRC_flag;                 // 1 bit
    int           PES_extension_flag;           // 1 bit
    int           PES_header_data_length;       // 8 bits
    unsigned long long PTS;                     // 33 bits
    unsigned long long DTS;                     // 33 bits
    unsigned long long ESCR_base;               // 33 bits
    unsigned int  ES_rate;                      // 22 bits
    int           trick_mode_control;           // 3 bits
    int           field_id;                     // 2 bits
    int           intra_slice_refresh;          // 1 bit
    int           frequency_truncation;         // 2 bits
    int           rep_cntrl;                    // 5 bits
    int           additional_copy_info;         // 7 bits
    int           previous_PES_packet_CRC;      // 16 bits
    int           PES_private_data_flag;        // 1 bits
    int           pack_header_field_flag;       // 1 bit
    int           program_packet_sequence_counter_flag; // 1 bit
    int           PES_STD_buffer_flag;          // 1 bit
    int           PES_extension_flag_2;         // 1 bit
    int           PES_private_data[16];         // 128 bits
    int           pack_field_length;            // 8 bits
    int           program_packet_sequence_counter; // 7 bits
    int           MPEG1_MPEG2_identifier;       // 1 bit
    int           original_stuff_length;        // 6 bits
    int           PES_STD_buffer_scale;         // 1 bit
    int           PES_STD_buffer_size;          // 13 bits
    int           PES_extension_field_length;   // 7 bits;
    int           PES_packet_data_byte;         // 8 bits;
    int           padding_byte;                 // 8 bits
} PES_HEADER_S, *LPPES_HEADER_S;

typedef struct PS_HEADER
{
    unsigned char           start_code[4];                      // 32 bits, must be 00 00 01 ba
    unsigned long long      system_clock_reference_base;        // 33 bits;
    int                     system_clock_reference_extension;   // 1bit
    unsigned int            program_mux_rate;                   // 22 bits;
    int                     pack_stuffing_length;               // 3 bits;
} PS_HEADER_S, *LPPS_HEADER_S;

typedef struct PS_HEADER_TOTALL
{
    PS_HEADER_S             ps_header;
    PES_SYSTEM_HEADER_S     system_header;
    PES_HEADER_S            pes_header;//一个ps包中有很多个pes包。里面的参数是最后一个pes包中的
}PS_HEADER_TOTALL_S, *LPPS_HEADER_TOTALL_S;

#pragma pack(pop)

#endif //CBB_PS_HEAD