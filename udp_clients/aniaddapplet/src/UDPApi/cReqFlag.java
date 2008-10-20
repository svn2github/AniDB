/**
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package UDPApi;

import java.lang.reflect.Method;

public class cReqFlag {
    public String Tag;
    public Method Func;
    public Object MethodContainer;
    
    public cReqFlag(String NewTag, Method NewFunc, Object NewMethodContainer) {
        Tag = NewTag;
        Func = NewFunc;
        MethodContainer = NewMethodContainer;
    }
}
