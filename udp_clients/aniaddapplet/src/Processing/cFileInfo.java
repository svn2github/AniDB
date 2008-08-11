/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package Processing;

import java.io.File;

public class cFileInfo {
    public  cFileInfo(File NewFilePath){
        FilePath = NewFilePath;
    }
    
    public java.io.File FilePath;
    public eFileState FileState;
    public eMyListState MLState;
    
    public java.util.Map<String, Object> Data = new java.util.TreeMap<String, Object>();
    
    public enum eFileState{
        unprocessed,
        processing,
        processed,
        gotfileinfo,
        error
    }
    
    public enum eMyListState{
        queued,
        inmylist,
        added,
        unknown
    
    }
    
    public enum eMLFileState{
        unknown,
        onhdd,
        oncd,
        deleted,
        share,
        release
    }
}
