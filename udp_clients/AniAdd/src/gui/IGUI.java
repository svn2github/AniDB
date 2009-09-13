package gui;

import aniAdd.Communication.ComEvent;
import aniAdd.Modules.IModule.eModState;
import gui.GUI.ITab;
import javax.swing.TransferHandler;

public interface IGUI{
    void ToMem(String key, Object value);
    Object FromMem(String key);
    Object FromMem(String key, Object defValue);

    void SetDragnDropHandler(TransferHandler th);

    eModState ModState();
    int AddTab(ITab tab);
    void SelectTab(int tabIndex);
    void RemoveTab(String tabName);

    void GUIEvent(ComEvent comEvent);

    void LogEvent(ComEvent.eType type, Object... params);
}
