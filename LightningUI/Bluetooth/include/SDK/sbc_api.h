/*************************************************************

                        ?Cambridge Silicon Radio Ltd, 2014

                        All rights reserved

FILE:                   sbc_api.h

DESCRIPTION:            Sub-Band Codec (decoder and encoder) interface
                        definitions.

REVISION:               #1 $am02 $Created
                        #2 $sv01 $Added 2 frame size defines and modified
                                  all function prototypes
                        #3 $sms  $Changed enum values, added SBC_Init_Decoder.

************************************************************/

#ifndef SBC_API_H__
#define SBC_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SBC_MAX_BLOCKS                  16
#define SBC_MAX_CHANNELS                2
#define SBC_MAX_SUBBANDS                8

#define MAX_SBC_AUDIO_FRAME_SIZE_BYTES  512
#define MAX_SBC_FRAME_SIZE_BYTES        512

/* Unsigned fixed width types */
typedef unsigned char CsrUint8;

typedef CsrUint8 CsrSbcChannelMode;
#define CSR_SBC_MONO            ((CsrSbcChannelMode) 0)
#define CSR_SBC_DUAL            ((CsrSbcChannelMode) 1)
#define CSR_SBC_STEREO          ((CsrSbcChannelMode) 2)
#define CSR_SBC_JOINT_STEREO    ((CsrSbcChannelMode) 3)

typedef CsrUint8 CsrSbcAllocMethod;
#define CSR_SBC_METHOD_LOUDNESS ((CsrSbcAllocMethod) 0)
#define CSR_SBC_METHOD_SNR      ((CsrSbcAllocMethod) 1)

typedef struct
{
    CsrSbcChannelMode           channel_mode;
    CsrSbcAllocMethod           alloc_method;
    unsigned short int          sample_freq;
    unsigned char               blocks;
    unsigned char               subbands;
    unsigned char               minBitpool;
    unsigned char               maxBitpool;
    unsigned char               bitPool;
    unsigned short int          framesize;
    unsigned char               channels;
    unsigned short int          frames_per_payload;
    unsigned short int          bytes_per_encoding;
} SbcConfig_t;


/****************************************************************************
  SBC_CalcBitPool - calculate the bitpool value

  Returns:
    * the main bitpool
    * the alternative bitpool (via the first argument)
    * the toggle period (via the second argument)

  Notes:
    If the bitpool needed to give a more accurate bit rate
    (i.e. closer to that required by the user) is not too close
    to a whole number [1],  then the toggle period is set to
    greater than zero and an alternate bitpool is specified.
    On the application level the main bitpool can then be made to
    switch to the alternative bitpool after every
    'toggle period'/2  frames.  The application may alternatively
    choose not to use a variable bitpool.

    [1] In this version of the codec the algoritm used in working
    out whether the bitpool falls 'close to a whole number' is
    very simple. When the bitpool is calculated, if it falls in
    the middle half     between two whole numbers (i.e.>1/4 & <3/4),
    the toggle period is set to two and the alternative bitpool
    is 'the main bitpool'+1.
****************************************************************************/
unsigned char SBC_CalcBitPool(unsigned char *bitPoolAlt, unsigned char *togglePeriod,
                              CsrSbcChannelMode channel_mode,
                              unsigned short int sample_freq,
                              unsigned char nrof_blocks,
                              unsigned char nrof_subbands,
                              unsigned short int bitrate);


/****************************************************************************
  SBC_Config - configure the settings required before encoding

  Returns:
    frame size in bytes if inputs are valid else returns zero.

  Notes:
    * sample_freq must be either 16000, 32000, 44100 or 48000.
    * nrof_subbands must either 4 or 8.
    * nrof_blocks must be either 4, 8, 12 or 16.
    * bitpool must be greater than 2 and no greater than
       16 * nrof_subbands for MONO and DUAL_CHANNEL channel modes
    or 32 * nrof_subbands for STEREO and JOINT_STEREO channel modes

    If all inputs are correct, all the appropriate settings will be set.
    This function does not need to be called before decoding.
****************************************************************************/
unsigned short int SBC_Config(CsrSbcChannelMode channel_mode,
                              CsrSbcAllocMethod alloc_method,
                              unsigned short int sample_freq,
                              unsigned char nrof_blocks,
                              unsigned char nrof_subbands,
                              unsigned char bitpool);


/****************************************************************************
  SBC_Encode - encodes a single sbc frame

  Inputs:
    * pointer to the input wav samples casted to a (signed short int (*)[2])
    * char* to the frame where sbc frame will be written
    * bitpool to be used for encoding the frame

  Returns:
    frame size in bytes of the sbc frame just encoded
****************************************************************************/
unsigned short int SBC_Encode(const signed short int audio_samples[][2],
                              unsigned char *frame,
                              unsigned char bit_pool);


/****************************************************************************
  SBC_Init_Decoder - initialise the decoder ready to accept another file.

  Input:
    none

  Returns:
    nothing
****************************************************************************/
void SBC_Init_Decoder(void);


/****************************************************************************
  SBC_Read_Header - verify the sbc frame header before attempting to decode
                        the frame

  Input:
    * char* to the frame to be read

  Returns:
    * frame size if frame is valid else returns zero.
****************************************************************************/
unsigned short int SBC_Read_Header(unsigned char *frame);


/****************************************************************************
  SBC_Decode - decodes a single sbc frame

  Inputs:
    * char* to the frame where sbc frame to be decoded begins
    * pointer to the buffer storing decoded wav samples
****************************************************************************/
void SBC_Decode(
    unsigned char *frame,
    signed short int audio_samples[SBC_MAX_BLOCKS][SBC_MAX_CHANNELS][SBC_MAX_SUBBANDS]);

#ifdef __cplusplus
}
#endif

#endif /* SBC_API_H__ */
