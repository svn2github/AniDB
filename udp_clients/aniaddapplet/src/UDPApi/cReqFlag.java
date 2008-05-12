/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package UDPApi;

import java.lang.reflect.Method;

public class cReqFlag {
    public String[] Tag;
    public int[] IDs;
    public Method Func;
    public Object MethodContainer;
    
    public cReqFlag(String[] NewTag, int[] NewID, Method NewFunc, Object NewMethodContainer) {
        Tag = NewTag;
        IDs = NewID;
        Func = NewFunc;
        MethodContainer = NewMethodContainer;
    }
    
    public static boolean Compare(cReqFlag x, cReqFlag y) {
        if (x.IDs != null && y.IDs != null) {
            for (int I : x.IDs) {
                for (int J : y.IDs) {
                    if (I == J) return true;
                }
            }
        }
        
        if (x.Tag != null && y.Tag != null) {
            for (String I : x.Tag) {
                for (String J : y.Tag) {
                    if (I.equals(J)) return true;
                }
            }            
        }
        
        return false;
    }
}
