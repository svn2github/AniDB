package processing;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import aniAdd.misc.ICallBack;
import ed2kHasher.Edonkey;
import java.io.BufferedInputStream;
import java.io.FileInputStream;
import java.util.logging.Level;
import java.util.logging.Logger;

public class FileParser {

    private boolean paused;
    private File file;
    private Object tag;
    private ICallBack<FileParser> callBack;
    private Thread thread;
    private Parser parser;
    private String hash;
    private long parseStartOn;
    private long parseEndOn;
    private long pauseStartOn;
    private long pauseDuration;

    public FileParser(File file, ICallBack<FileParser> callBack, Object tag) {
        this.file = file;
        this.callBack = callBack;
        this.tag = tag;
        paused = false;

        parser = new Parser(this);
        thread = new Thread(parser);
    }

    public long getBytesRead() {
        return parser.getBytesRead();
    }

    public long getByteCount() {
        return file.length();
    }

    public void pause() {
        pauseStartOn = System.currentTimeMillis();
        paused = true;
    }

    public void resume() {
        pauseDuration = System.currentTimeMillis() - pauseStartOn;
        pauseStartOn = 0;
        paused = false;
    }

    public void start() {
        pauseDuration = 0;
        parseStartOn = 0;
        parseStartOn = System.currentTimeMillis();
        thread.start();
    }

    public void terminate() {
        parser.terminate();
    }

    public long ParseDuration() {
        return parseEndOn - parseStartOn - pauseDuration;
    }

    public int MBPerSecond() {
        return (int) ((file.length() / ParseDuration()) * 1000 / 1024 / 1024);
    }

    public Object Tag() {
        return tag;
    }

    public String Hash() {
        return hash;
    }

    private class Parser implements Runnable {

        private FileParser fileParser;
        private long bytesRead = 0;
        private boolean terminate = false;

        public void terminate() {
            terminate = true;
        }

        public Parser(FileParser fileParser) {
            this.fileParser = fileParser;
        }

        public long getBytesRead() {
            return bytesRead;
        }

        public void run() {
            ArrayList<Object> obj = new ArrayList<Object>();
            obj.add(tag);
            bytesRead = 0;

            try {
                Edonkey ed2k = new Edonkey();
                byte[] b = new byte[1024 * 1024 * 4];
                BufferedInputStream bis = new BufferedInputStream(new FileInputStream(file));

                try {
                    int numRead;
                    while ((numRead = bis.read(b)) != -1 && !terminate) {
                        while (paused) Thread.sleep(100);
                        
                        ed2k.update(b, 0, numRead);
                        bytesRead += numRead;
                    }
                    hash = ed2k.getHexValue();
                } finally { bis.close(); }

            } catch (Exception e) { e.printStackTrace(); }
   

            parseEndOn = System.currentTimeMillis();
            if (!terminate) {
                callBack.invoke(fileParser);
            }
        }
    }
}
