#ifndef __OGM_SPECS_H__
#define __OGM_SPECS_H__

#include <ogg/ogg.h>
#include "common_endian.h"

/* 
 * Taken from http://tobias.everwicked.com/packfmt.htm
 *

 First packet (header)
 ---------------------

 pos    | content                 | description
 -------+-------------------------+----------------------------------
 0x0000 | 0x01                    | indicates 'header packet'
 -------+-------------------------+----------------------------------
 0x0001 | stream_header           | the size is indicated in the
        |                         | size member


 Second packet (comment)
 -----------------------

 pos    | content                 | description
 -------+-------------------------+----------------------------------
 0x0000 | 0x03                    | indicates 'comment packet'
 -------+-------------------------+----------------------------------
 0x0001 | data                    | see vorbis doc on www.xiph.org

 Data packets
 ------------

 pos      | content                 | description
 ---------+-------------------------+----------------------------------
 0x0000   | Bit0  0                 | indicates data packet
          | Bit1  Bit 2 of lenbytes |
          | Bit2  unused            |
          | Bit3  keyframe          |
          | Bit4  unused            |
          | Bit5  unused            |
          | Bit6  Bit 0 of lenbytes |
          | Bit7  Bit 1 of lenbytes |
 ---------+-------------------------+----------------------------------
 0x0001   | LowByte                 | Length of this packet in samples
          | ...            lenbytes | (frames for video, samples for
          | HighByte                | audio, 1ms units for text)
 ---------+-------------------------+----------------------------------
 0x0001+  | data                    | packet contents
 lenbytes |                         |

 *
 *
 */

//// OggDS headers
// Header for the new header format
typedef struct {
	sint32	width;				/*ogg_int32_t*/
	sint32	height;				/*ogg_int32_t*/
} stream_header_video;

#define FIX_ENDIAN_OGM_STREAMHEADER_VIDEO(dst,src,aim)	\
	(dst)->width = INT32_##aim##_LE((src)->width);		\
	(dst)->height = INT32_##aim##_LE((src)->height);

#define FIX_ENDIAN_FROM_OGM_STREAMHEADER_VIDEO2(dst,src)	FIX_ENDIAN_OGM_STREAMHEADER_VIDEO(dst,src,FROM)
#define FIX_ENDIAN_FROM_OGM_STREAMHEADER_VIDEO(src)			FIX_ENDIAN_OGM_STREAMHEADER_VIDEO(src,src,FROM)
#define FIX_ENDIAN_TO_OGM_STREAMHEADER_VIDEO2(dst,src)		FIX_ENDIAN_OGM_STREAMHEADER_VIDEO(dst,src,TO)
#define FIX_ENDIAN_TO_OGM_STREAMHEADER_VIDEO(src)			FIX_ENDIAN_OGM_STREAMHEADER_VIDEO(src,src,TO)


typedef struct {
	sint16	channels;			/*ogg_int16_t*/
	sint16	blockalign;			/*ogg_int16_t*/
	sint32	avgbytespersec;		/*ogg_int32_t*/
} stream_header_audio;

#define FIX_ENDIAN_OGM_STREAMHEADER_AUDIO(dst,src,aim)					\
	(dst)->channels = INT16_##aim##_LE((src)->channels);				\
	(dst)->blockalign = INT16_##aim##_LE((src)->blockalign);			\
	(dst)->avgbytespersec = INT32_##aim##_LE((src)->avgbytespersec);

#define FIX_ENDIAN_FROM_OGM_STREAMHEADER_AUDIO2(dst,src)	FIX_ENDIAN_OGM_STREAMHEADER_AUDIO(dst,src,FROM)
#define FIX_ENDIAN_FROM_OGM_STREAMHEADER_AUDIO(src)			FIX_ENDIAN_OGM_STREAMHEADER_AUDIO(src,src,FROM)
#define FIX_ENDIAN_TO_OGM_STREAMHEADER_AUDIO2(dst,src)		FIX_ENDIAN_OGM_STREAMHEADER_AUDIO(dst,src,TO)
#define FIX_ENDIAN_TO_OGM_STREAMHEADER_AUDIO(src)			FIX_ENDIAN_OGM_STREAMHEADER_AUDIO(src,src,TO)


/* ensure proper alignment for non-MS compilers */
typedef struct
#if defined(__GNUC__)
__attribute__((__aligned__(8)))
#endif
{
	char		streamtype[8];		/*char*/
	char		subtype[4];			/*char*/

	sint32	size;				/*ogg_int32_t*/	// size of the structure

	sint64	time_unit;			/*ogg_int64_t*/	// in reference time
	sint64	samples_per_unit;	/*ogg_int64_t*/
	sint32	default_len;		/*ogg_int32_t*/	// in media time

	sint32	buffersize;			/*ogg_int32_t*/
	sint16	bits_per_sample;	/*ogg_int16_t*/

	union
#if defined(__GNUC__)
	__attribute__((__aligned__(4)))
#endif
	{
		// Video specific
		stream_header_video	video;
		// Audio specific
		stream_header_audio	audio;
	} sh;
} stream_header;

#define FIX_ENDIAN_OGM_STREAMHEADER(dst,src,aim)							\
	if((dst) != (src)) {													\
		*((uint64 *)(dst)->streamtype) = *((uint64 *)(src)->streamtype);	\
		*((uint32 *)(dst)->subtype) = *((uint32 *)(src)->subtype);			\
		(dst)->sh = (src)->sh;												\
	}																		\
	(dst)->size = INT32_##aim##_LE((src)->size);							\
	(dst)->time_unit = INT64_##aim##_LE((src)->time_unit);					\
	(dst)->samples_per_unit = INT64_##aim##_LE((src)->samples_per_unit);	\
	(dst)->default_len = INT32_##aim##_LE((src)->default_len);				\
	(dst)->buffersize = INT32_##aim##_LE((src)->buffersize);				\
	(dst)->bits_per_sample = INT16_##aim##_LE((src)->bits_per_sample);

#define FIX_ENDIAN_FROM_OGM_STREAMHEADER2(dst,src)	FIX_ENDIAN_OGM_STREAMHEADER(dst,src,FROM)
#define FIX_ENDIAN_FROM_OGM_STREAMHEADER(src)		FIX_ENDIAN_OGM_STREAMHEADER(src,src,FROM)
#define FIX_ENDIAN_TO_OGM_STREAMHEADER2(dst,src)	FIX_ENDIAN_OGM_STREAMHEADER(dst,src,TO)
#define FIX_ENDIAN_TO_OGM_STREAMHEADER(src)			FIX_ENDIAN_OGM_STREAMHEADER(src,src,TO)


/*
 * Some infos :
 *
 * - size : size of the header. Generally the header is only composed of the
 *     stream_header, i.e. size = sizeof(stream_header). When dealing with
 *     WAVEFORMATEX (audio) headers, the extra data (appended after the generic
 *     structure) are the same, i.e.
 *     stream_header.size = sizeof(stream_header) + WAVEFORMATEX.cbSize
 *     and the corresponding number of extra bytes (cbSize) appended after
 *     the stream_header and WAVEFORMATEX are the same.
 *          
 * - blockalign : number of bytes for 1 sample including all channels
 *     (i.e. same than WAVEFORMATEX.nBlockAlign)
 *     This mean we should have (for PCM streams) :
 *       nblockalign = bits_per_sample * channels / 8;
 *       bits_per_sample = avgbytespersec * 8 / sample_rate;
 *
 * - time_unit : unit of time expressed in a 100ns time reference
 *     (100ns is the reference time for Windows' DirectShow)
 *     So the real time unit is time_unit / 10000000
 *
 * - samples_per_unit : is the number of samples per unit of time
 *     So the number of samples per second is :
 *       samples_per_second = samples_per_unit / (time_unit / 10000000)
 *         samples_per_second      10000000
 *   Or : -------------------- = -----------
 *          samples_per_unit      time_unit
 *
 * - default_len : default number of samples in a Packet when its length
 *     isn't written (lenbytes == 0)
 *
 * Application :
 * - for a video stream there is only 1 sample per unit (a packet represents
 *  a unit here), so samples_per_second = fps = 10000000 / time_unit
 * - for an audio stream the unit is 1s, so time_unit = 10000000 and
 *  samples_per_unit = samples_per_second
 * - for a text stream, the unit is 1ms, so time_unit = 10000. As one sample
 *  lasts 1ms, samples_per_second = 1000 => samples_per_unit = 1
 */


// Some defines
#define PACKET_TYPE_HEADER       0x01
#define PACKET_TYPE_COMMENT      0x03
#define PACKET_TYPE_DATA         0x00
#define PACKET_TYPE_BITS         0x07
#define PACKET_LEN_BITS01        0xc0
#define PACKET_LEN_BITS2         0x02
#define PACKET_IS_SYNCPOINT      0x08

#endif // __OGM_SPECS_H__
