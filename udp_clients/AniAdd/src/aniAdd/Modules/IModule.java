package aniAdd.Modules;

import aniAdd.*;


public interface IModule extends Communication {
    String ModuleName();
    eModState ModState();
    
    void Initialize(IAniAdd aniAdd);
    void Terminate();

    public enum eModState { New, Initializing, Initialized, Terminating, Terminated }
}
