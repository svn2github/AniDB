package net.sf.ffmpeg_java.util;

import java.util.logging.Level;
import java.util.logging.Logger;

/**
 * Provides a global instance of ImageConverter.
 * Is able to provide the GPL-licensed net.sf.ffmpeg_java.util..SWScaleLibraryImageConverter if it is
 * linked, otherwise falls back to LGPL-licensed net.sf.ffmpeg_java.util.AVCodecLibraryImageConverter.
 * @author Ken Larson
 *
 */
public class ImageConverterSingleton
{
	private static final Logger logger = LoggerSingleton.logger;
	
	private static ImageConverter instance;
	
	public static ImageConverter instance()
	{
		if (instance != null)
			return instance;
		
		
		final String[] imageConverterClassNames = new String [] {
				"net.sf.ffmpeg_java.util.SWScaleLibraryImageConverter",	// GPL
				"net.sf.ffmpeg_java.util.AVCodecLibraryImageConverter"	// LGPL
				};
		
		for (String className : imageConverterClassNames)
		{
			try
			{
				final Class<?> imageConverterClass = Class.forName(className);
				instance = (ImageConverter) imageConverterClass.newInstance();
				return instance;
				
			
			}
			catch (Throwable t)
			{
				logger.log(Level.FINE, "Skipping ImageConverter: " + className + ": " + t);
			}
		}
		
		throw new RuntimeException("Unable to instantiate any ImageConverter");
		
	}
}
