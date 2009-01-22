!!README!!

To enable avparsing several libs need to be installed in your library path (c:\windows\system32 in windows).
If you don't wish to use Xuggle (JNI based) for your project and still have avparsing, you can use ffmpeg-java (JNA based).

Currently the current avparser implementations are:
	AVParserFFmpegJava: JNA based ffmpeg wrapper (unstable, specially while using full parsing)
	AVParserXuggle: JNI based ffmpeg wrapper (very stable)
	AVParserNone: A null parser (doesn't do anything)
	
Use the parser implementation you want, these are the jar files needed to run each:
	AVParserFFmpegJava: jna.jar (777KB)
	AVParserXuggle: commons-cli.jar, jetm.jar, junit.jar, logback-classic.jar, logback-core.jar, slf4j-api.jar, xuggle-utils.jar, xuggle-xuggler.jar (932KB)
	AVParserNone: none
	
These are the shared library files you needed to run each:
	AVParserFFmpegJava: avcodec-52, avformat-52
	AVParserXuggle: all the libraries listed above (except, maybe, the optional ones)
	AVParserNone: none

A complete list of required libs is:

[FFmpeg]
	avcodec-52
	avformat-52
	avutil-49 (optional)
	avdevice-52 (optional)
	swscale-0 (optional)
[Xuggle]
	libxuggle-ferry-1
	libxuggle-xuggler-1
	libxuggle-xuggler-io-1
[Extra]
	libmp3lame-0 (needed by Xuggle)
	libspeex-1 (needed by Xuggle)
	libspeexdsp-1 (needed by Xuggle)
	
I'll provide windows bundles of these libs at:
	http://static.anidb.net/client/xauc/required_windows_libs.7z
	
Note, these libs were compiled against: 
	Xuggle: 1.17 RC3
	ffmpeg: svn://svn.mplayerhq.hu/ffmpeg/trunk : 16686
	libswscale: svn://svn.ffmpeg.org/mplayer/trunk/libswscale : 28322
	libmp3lame: http://downloads.sourceforge.net/lame/lame-398-2.tar.gz: 3.98.2
	libspeex: http://downloads.xiph.org/releases/speex/speex-1.2rc1.tar.gz: 1.2 RC1
	
You can get shared libraries for your platform at 
	http://www.xuggle.com/xuggler/downloads/
	
	Or build from source xuggler (which also builds ffmpeg, liblame, libspeex and libswscale) and get all the required libs.
