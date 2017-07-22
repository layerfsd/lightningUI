/*************************************************************

                 CSR plc 2011

                 All rights reserved

FILE:            a2dp_filter.h

DESCRIPTION:     A2DP demo definition for codec/file/endpoint filters.
                 A filter is anything that generates, consumes or
                 modifies a stream of audio (or video). This file
                 contain the interface definition for all filters
                 which is a structure with a number of function-pointers
                 and variables that must be initialised for each filter.

REVISION:        $Revision: #1 $

************************************************************/
#ifndef _A2DP_FILTER_H_
#define _A2DP_FILTER_H_

#include "bluetooth_a2dp.h"
#include "sbc_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Filter types */
#define FILTER_PRODUCER    0x01  /* Filter produces something */
#define FILTER_CONSUMER    0x02  /* Filter consumes something */
#define FILTER_ENCODER     0x04  /* Filter delivers encoded data */
#define FILTER_DECODER     0x08  /* Filter delivers raw (pcm) data */
#define FILTER_PASS        0x10  /* Data travels through filter */

/* Media/codec types for internal demo filters */
#define AV_NO_MEDIA        0xFF
#define AV_NO_CODEC        0xFF

/* Filter states */
#define FILTER_ST_NOINIT   0     /* Filter is un-initialised */
#define FILTER_ST_IDLE     1     /* Filter initialized */
#define FILTER_ST_OPEN     2     /* Filter has been opened */
#define FILTER_ST_START    3     /* Filter has been started */

/* Opcodes for simple filter runners */
#define FILTER_INIT        0     /* Run initialise */
#define FILTER_DEINIT      1     /* Run deinitialise */
#define FILTER_OPEN        2     /* Open or reconfig */
#define FILTER_CLOSE       3     /* Close */
#define FILTER_START       4     /* Start */
#define FILTER_STOP        5     /* Stop */
#define FILTER_MUTE		   6
#define FILTER_UNMUTE	   7

/* Definiton of the filter type */
struct a2dp_filter_t
{
    /* Static filter identification */
    LPTSTR f_name;                                      /* Static filter name */
    BYTE f_type;                                        /* Static filter type */
    a2dp_media_t f_media;                               /* A2DP media type */
    a2dp_codec_t f_codec;                               /* A2DP codec type */
    BYTE f_state;                                       /* Filter state */
    void *f_instance;                                   /* Filter instance data */

    /* Initialise/shutdown */
    BOOL (*f_init)(void **instance,                     /* Called when initialising demo */
                   struct a2dp_filter_t *filter);       /* filter points to the static filter definition */
    BOOL (*f_deinit)(void **instance);                  /* Called when demo is shutting down */

    /* Open/close media, start/stop stream */
    BOOL (*f_open)(void **instance);                    /* Open device or (re)prepare current config */
    BOOL (*f_close)(void **instance);                   /* Close device */
    BOOL (*f_start)(void **instance);                   /* Start streaming */
    BOOL (*f_stop)(void **instance);                    /* Stop streaming */

    /* Real-time processing */
    BOOL (*f_process)(BYTE index,                       /* Process/produce/consume data */
                      struct a2dp_filter_t **chain,     /* Use data/length if possible. Free 'data' */
                      BOOL freeData,                    /* if present and if 'freeData' is set. Call */
                      void *data,                       /* next filter in chain with index+1 like this */
                      DWORD length);                    /* chain[index+1]->f_process(index+1, ...)*/

    /* Stream configuration */
    BYTE *(*f_get_config)(void **instance,              /* Get configuration */
                          BYTE *conf_len);              /* Return pointer and store length in conf_len */

    BOOL (*f_set_config)(void **instance,               /* Set configuration */
                         BYTE *conf,                    /* New configuration is in conf/conf_len.*/
                         BYTE conf_len);                /* Return result code */

    BYTE *(*f_get_caps)(void **instance,                /* Get service capabilities of this filter*/
                        BYTE *cap_len);                 /* Return pointer and store length in cap_len */

    BYTE *(*f_remote_caps)(void **instance,             /* Remote capabilities received. */
                           BYTE *caps,                  /* Feed remote caps into function, and select */
                           BYTE caps_len,               /* the optimal configuration. Return "optimal config" */
                           BYTE *conf_len);             /* pointer and store length in *conf_len */

    void (*f_qos_update)(void **instance,               /* QoS buffer status level received. Called periodically */
                         WORD qosLevel);                /* with 'qosLevel' buffer status ranging from 0 to 10 */

	BOOL (*f_mute)(void **instance);					/* mute the a2dp stream*/
	BOOL (*f_unmute)(void **instance);
};

typedef struct a2dp_filter_t a2dp_filter_t;


#ifdef __cplusplus
}
#endif

#endif // _A2DP_FILTER_H_

