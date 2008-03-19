
- Readme - MPEG Audio Info Tool V2.1 - 2005-11-17

Description:
This tool can display information about MPEG audio files. It supports
MPEG1, MPEG2, MPEG2.5 in all three layers. You can get all the fields
from the MPEG audio frame in each frame of the file. Additionally you
can check the whole file for inconsistencies.

This tool was written as an example on how to use the classes:
CMPAFile, CMPAFrame, CMPAHeader, CVBRHeader, CMPAStream, CTag and 
CMPAException.

The article MPEG Audio Frame Header on Sourceproject
[http://www.codeproject.com/audio/MPEGAudioInfo.asp] 
provides additional information about these classes and the MPEG audio
frame header in general.

This tool was written with MS Visual C++ 7.1. The MFC library is
statically linked.


Changes from version 2.0:

- cleaned up class architecture
- better navigation through frames
- improved checking of files
- fixed original bit checking
- fixed frame size calculation for all layers (correct truncation)
- fixed skipping of small frames in VBR files
- new cache management
- added CRC check
- added detection of ID3V1/V2, APE and Lyrics3 tags
