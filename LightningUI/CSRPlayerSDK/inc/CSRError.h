/***************************************************************************
*                                                                         *
*                   SiRF Technology, Inc. Platform Software               *
*                                                                         *
*    Copyright (c) 2010 by SiRF Technology, Inc.  All rights reserved.    *
*                                                                         *
*    This Software is protected by United States copyright laws and       *
*    international treaties.  You may not reverse engineer, decompile     *
*    or disassemble this Software.                                        *
*                                                                         *
*    WARNING:                                                             *
*    This Software contains SiRF Technology, Inc.'s confidential and      *
*    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,    *
*    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED      *
*    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this      *
*    Software without SiRF Technology, Inc.'s  express written            *
*    permission.   Use of any portion of the contents of this Software    *
*    is subject to and restricted by your written agreement with          *
*    SiRF Technology, Inc.                                                *
*                                                                         *
***************************************************************************/

#ifndef __CSR_ERROR_H__
#define __CSR_ERROR_H__

/**
 * GstCoreError:
 * @CSR_CORE_ERROR_FAILED: a general error which doesn't fit in any other
 * category.  Make sure you add a custom message to the error call.
 * @CSR_CORE_ERROR_TOO_LAZY: do not use this except as a placeholder for
 * deciding where to go while developing code.
 * @CSR_CORE_ERROR_NOT_IMPLEMENTED: use this when you do not want to implement
 * this functionality yet.
 * @CSR_CORE_ERROR_STATE_CHANGE: used for state change errors.
 * @CSR_CORE_ERROR_PAD: used for pad-related errors.
 * @CSR_CORE_ERROR_THREAD: used for thread-related errors.
 * @CSR_CORE_ERROR_NEGOTIATION: used for negotiation-related errors.
 * @CSR_CORE_ERROR_EVENT: used for event-related errors.
 * @CSR_CORE_ERROR_SEEK: used for seek-related errors.
 * @CSR_CORE_ERROR_CAPS: used for caps-related errors.
 * @CSR_CORE_ERROR_TAG: used for negotiation-related errors.
 * @CSR_CORE_ERROR_MISSING_PLUGIN: used if a plugin is missing.
 * @CSR_CORE_ERROR_CLOCK: used for clock related errors.
 * @CSR_CORE_ERROR_DISABLED: used if functionality has been disabled at
 *                           compile time (Since: 0.10.13).
 * @CSR_CORE_ERROR_NUM_ERRORS: the number of core error types.
 *
 * Core errors are errors inside the core GStreamer library.
 */
/* FIXME: should we divide in numerical blocks so we can easily add
          for example PAD errors later ? */
typedef enum _CSR_CORE_ERROR
{
	CSR_CORE_ERROR_FAILED = 1,
	CSR_CORE_ERROR_TOO_LAZY,
	CSR_CORE_ERROR_NOT_IMPLEMENTED,
	CSR_CORE_ERROR_STATE_CHANGE,
	CSR_CORE_ERROR_PAD,
	CSR_CORE_ERROR_THREAD,
	CSR_CORE_ERROR_NEGOTIATION,
	CSR_CORE_ERROR_EVENT,
	CSR_CORE_ERROR_SEEK,
	CSR_CORE_ERROR_CAPS,
	CSR_CORE_ERROR_TAG,
	CSR_CORE_ERROR_MISSING_PLUGIN,
	CSR_CORE_ERROR_CLOCK,
	CSR_CORE_ERROR_DISABLED,
	CSR_CORE_ERROR_NUM_ERRORS
} CSR_CORE_ERROR;

/**
 * GstLibraryError:
 * @CSR_LIBRARY_ERROR_FAILED: a general error which doesn't fit in any other
 * category.  Make sure you add a custom message to the error call.
 * @CSR_LIBRARY_ERROR_TOO_LAZY: do not use this except as a placeholder for
 * deciding where to go while developing code.
 * @CSR_LIBRARY_ERROR_INIT: used when the library could not be opened.
 * @CSR_LIBRARY_ERROR_SHUTDOWN: used when the library could not be closed.
 * @CSR_LIBRARY_ERROR_SETTINGS: used when the library doesn't accept settings.
 * @CSR_LIBRARY_ERROR_ENCODE: used when the library generated an encoding error.
 * @CSR_LIBRARY_ERROR_NUM_ERRORS: the number of library error types.
 *
 * Library errors are for errors from the library being used by elements
 * (initializing, finalizing, settings, ...)
 */
typedef enum _CSR_LIBRARY_ERROR
{
	CSR_LIBRARY_ERROR_FAILED = 1,
	CSR_LIBRARY_ERROR_TOO_LAZY,
	CSR_LIBRARY_ERROR_INIT,
	CSR_LIBRARY_ERROR_SHUTDOWN,
	CSR_LIBRARY_ERROR_SETTINGS,
	CSR_LIBRARY_ERROR_ENCODE,
	CSR_LIBRARY_ERROR_NUM_ERRORS
} CSR_LIBRARY_ERROR;

/**
 * GstResourceError:
 * @CSR_RESOURCE_ERROR_FAILED: a general error which doesn't fit in any other
 * category.  Make sure you add a custom message to the error call.
 * @CSR_RESOURCE_ERROR_TOO_LAZY: do not use this except as a placeholder for
 * deciding where to go while developing code.
 * @CSR_RESOURCE_ERROR_NOT_FOUND: used when the resource could not be found.
 * @CSR_RESOURCE_ERROR_BUSY: used when resource is busy.
 * @CSR_RESOURCE_ERROR_OPEN_READ: used when resource fails to open for reading.
 * @CSR_RESOURCE_ERROR_OPEN_WRITE: used when resource fails to open for writing.
 * @CSR_RESOURCE_ERROR_OPEN_READ_WRITE: used when resource cannot be opened for
 * both reading and writing, or either (but unspecified which).
 * @CSR_RESOURCE_ERROR_CLOSE: used when the resource can't be closed.
 * @CSR_RESOURCE_ERROR_READ: used when the resource can't be read from.
 * @CSR_RESOURCE_ERROR_WRITE: used when the resource can't be written to.
 * @CSR_RESOURCE_ERROR_SEEK: used when a seek on the resource fails.
 * @CSR_RESOURCE_ERROR_SYNC: used when a synchronize on the resource fails.
 * @CSR_RESOURCE_ERROR_SETTINGS: used when settings can't be manipulated on.
 * @CSR_RESOURCE_ERROR_NO_SPACE_LEFT: used when the resource has no space left.
 * @CSR_RESOURCE_ERROR_NUM_ERRORS: the number of resource error types.
 *
 * Resource errors are for any resource used by an element:
 * memory, files, network connections, process space, ...
 * They're typically used by source and sink elements.
 */
typedef enum _CSR_RESOURCE_ERROR
{
	CSR_RESOURCE_ERROR_FAILED = 1,
	CSR_RESOURCE_ERROR_TOO_LAZY,
	CSR_RESOURCE_ERROR_NOT_FOUND,
	CSR_RESOURCE_ERROR_BUSY,
	CSR_RESOURCE_ERROR_OPEN_READ,
	CSR_RESOURCE_ERROR_OPEN_WRITE,
	CSR_RESOURCE_ERROR_OPEN_READ_WRITE,
	CSR_RESOURCE_ERROR_CLOSE,
	CSR_RESOURCE_ERROR_READ,
	CSR_RESOURCE_ERROR_WRITE,
	CSR_RESOURCE_ERROR_SEEK,
	CSR_RESOURCE_ERROR_SYNC,
	CSR_RESOURCE_ERROR_SETTINGS,
	CSR_RESOURCE_ERROR_NO_SPACE_LEFT,
	CSR_RESOURCE_ERROR_NUM_ERRORS
} CSR_RESOURCE_ERROR;

/**
 * GstStreamError:
 * @CSR_STREAM_ERROR_FAILED: a general error which doesn't fit in any other
 * category.  Make sure you add a custom message to the error call.
 * @CSR_STREAM_ERROR_TOO_LAZY: do not use this except as a placeholder for
 * deciding where to go while developing code.
 * @CSR_STREAM_ERROR_NOT_IMPLEMENTED: use this when you do not want to implement
 * this functionality yet.
 * @CSR_STREAM_ERROR_TYPE_NOT_FOUND: used when the element doesn't know the
 * stream's type.
 * @CSR_STREAM_ERROR_WRONG_TYPE: used when the element doesn't handle this type
 * of stream.
 * @CSR_STREAM_ERROR_CODEC_NOT_FOUND: used when there's no codec to handle the
 * stream's type.
 * @CSR_STREAM_ERROR_DECODE: used when decoding fails.
 * @CSR_STREAM_ERROR_ENCODE: used when encoding fails.
 * @CSR_STREAM_ERROR_DEMUX: used when demuxing fails.
 * @CSR_STREAM_ERROR_MUX: used when muxing fails.
 * @CSR_STREAM_ERROR_FORMAT: used when the stream is of the wrong format
 * (for example, wrong caps).
 * @CSR_STREAM_ERROR_DECRYPT: used when the stream is encrypted and can't be
 * decrypted because this is not supported by the element. (Since: 0.10.20)
 * @CSR_STREAM_ERROR_DECRYPT_NOKEY: used when the stream is encrypted and
 * can't be decrypted because no suitable key is available. (Since: 0.10.20)
 * @CSR_STREAM_ERROR_NUM_ERRORS: the number of stream error types.
 *
 * Stream errors are for anything related to the stream being processed:
 * format errors, media type errors, ...
 * They're typically used by decoders, demuxers, converters, ...
 */
typedef enum _CSR_STREAM_ERROR
{
	CSR_STREAM_ERROR_FAILED = 1,
	CSR_STREAM_ERROR_TOO_LAZY,
	CSR_STREAM_ERROR_NOT_IMPLEMENTED,
	CSR_STREAM_ERROR_TYPE_NOT_FOUND,
	CSR_STREAM_ERROR_WRONG_TYPE,
	CSR_STREAM_ERROR_CODEC_NOT_FOUND,
	CSR_STREAM_ERROR_DECODE,
	CSR_STREAM_ERROR_ENCODE,
	CSR_STREAM_ERROR_DEMUX,
	CSR_STREAM_ERROR_MUX,
	CSR_STREAM_ERROR_FORMAT,
	CSR_STREAM_ERROR_DECRYPT,
	CSR_STREAM_ERROR_DECRYPT_NOKEY,
	CSR_STREAM_ERROR_NUM_ERRORS
} CSR_STREAM_ERROR;


typedef enum _CM_ERROR_DOMAIN
{
	CM_ERROR_UNKNOWN,		//	Uknown error domain
	CM_ERROR_CORE,			//	CSR_CORE_ERROR_*
	CM_ERROR_LIBRARY,		//	CSR_LIBRARY_ERROR_*
	CM_ERROR_RESOURCE,		//	CSR_RESOURCE_ERROR_*
	CM_ERROR_STREAM			//	CSR_STREAM_ERROR_*
} CM_ERROR_DOMAIN;

#endif /* __CSR_ERROR_H__ */
