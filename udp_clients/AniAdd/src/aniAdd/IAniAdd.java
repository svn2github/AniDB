package aniAdd;

import aniAdd.Modules.IModule;
import java.util.Collection;

public interface IAniAdd extends Communication {
    IModule GetModule(String modName);
    Collection<IModule> GetModules();
}
