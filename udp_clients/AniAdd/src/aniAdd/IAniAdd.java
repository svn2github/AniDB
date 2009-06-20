package aniAdd;

import java.util.Collection;

public interface IAniAdd extends Communication {
    Module GetModule(String modName);
    Collection<Module> GetModules();
}
