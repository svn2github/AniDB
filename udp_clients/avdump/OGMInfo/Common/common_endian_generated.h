/*
 * This file has been automatically generated.
 * You shouldn't modify manually this file,
 * because your modifications may be erased.
 */

#ifndef __GENERATED_FILE_ENDIANNESS__
#define __GENERATED_FILE_ENDIANNESS__

#ifndef BIG_ENDIAN
#	define LIL_ENDIAN 0x0102
#	define BIG_ENDIAN 0x0201
#endif

#undef BYTE_ORDER

/* This system appears to be Little Endian */
#define BYTE_ORDER	LIL_ENDIAN

/* Some information :
 * Size of char : 1 byte(s)
 * Size of unsigned long : 4 byte(s)
 * Written as unsigned long :
 *   0x01 0x23 0x45 0x67
 * Read from unsigned long (using bitshifting) :
 *   0x01 0x23 0x45 0x67
 * Read from memory :
 *   0x67 0x45 0x23 0x01
 */


#if (BYTE_ORDER != LIL_ENDIAN) && (BYTE_ORDER != BIG_ENDIAN)
#	error Please define the byte ordering of this system
#endif


#endif /* __GENERATED_FILE_ENDIANNESS__ */
