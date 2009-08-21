/* src/config.h.in.  Generated from configure.ac by autoheader.  */

#define GENERIC
#define NOXFERMEM
#define ssize_t int
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#define snprintf _snprintf

/* Define if debugging is enabled. */
#undef DEBUG

/* Define if gapless is enabled. */
#undef GAPLESS

/* Define to 1 if you have the <asm/audioio.h> header file. */
#undef HAVE_ASM_AUDIOIO_H

/* Define to 1 if you have the <AudioToolbox/AudioToolbox.h> header file. */
#undef HAVE_AUDIOTOOLBOX_AUDIOTOOLBOX_H

/* Define to 1 if you have the <AudioUnit/AudioUnit.h> header file. */
#undef HAVE_AUDIOUNIT_AUDIOUNIT_H

/* Define to 1 if you have the <CoreServices/CoreServices.h> header file. */
#undef HAVE_CORESERVICES_CORESERVICES_H

/* Define to 1 if you have the `getpagesize' function. */
#undef HAVE_GETPAGESIZE

/* Define to 1 if you have the <inttypes.h> header file. */
#undef HAVE_INTTYPES_H

/* Define to 1 if you have the `m' library (-lm). */
#undef HAVE_LIBM

/* Define to 1 if you have the `mx' library (-lmx). */
#undef HAVE_LIBMX

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <linux/soundcard.h> header file. */
#undef HAVE_LINUX_SOUNDCARD_H

/* Define to 1 if you have the <machine/soundcard.h> header file. */
#undef HAVE_MACHINE_SOUNDCARD_H

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have a working `mmap' system call. */
#undef HAVE_MMAP

/* Define to 1 if you have the <sched.h> header file. */
#undef HAVE_SCHED_H

/* Define to 1 if you have the `sched_setscheduler' function. */
#undef HAVE_SCHED_SETSCHEDULER

/* Define to 1 if you have the `setpriority' function. */
#undef HAVE_SETPRIORITY

/* Define to 1 if you have the <stdint.h> header file. */
#undef HAVE_STDINT_H

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#undef HAVE_STRINGS_H

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sun/audioio.h> header file. */
#undef HAVE_SUN_AUDIOIO_H

/* Define to 1 if you have the <sys/audioio.h> header file. */
#undef HAVE_SYS_AUDIOIO_H

/* Define to 1 if you have the <sys/audio.h> header file. */
#undef HAVE_SYS_AUDIO_H

/* Define to 1 if you have the <sys/ioctl.h> header file. */
#undef HAVE_SYS_IOCTL_H

/* Define to 1 if you have the <sys/soundcard.h> header file. */
#undef HAVE_SYS_SOUNDCARD_H

/* Define to 1 if you have the <sys/stat.h> header file. */
#undef HAVE_SYS_STAT_H

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define this if you have the POSIX termios library */
#undef HAVE_TERMIOS

/* Define to 1 if you have the <unistd.h> header file. */
#undef HAVE_UNISTD_H

/* size of the frame index seek table */
#define INDEX_SIZE 1000

/* Name of package */
#define PACKAGE "whatever"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "whatever"

/* Define to the full name of this package. */
#define PACKAGE_NAME "whatever"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "whatever"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "whatever"

/* Define to the version of this package. */
#define PACKAGE_VERSION "whatever"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "whatever"

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#undef WORDS_BIGENDIAN

/* Define to empty if `const' does not conform to ANSI C. */
#undef const

/* Define to `__inline__' or `__inline' if that's what the C compiler
   calls it, or to nothing if 'inline' is not supported under any name.  */
#ifndef __cplusplus
#undef inline
#endif
