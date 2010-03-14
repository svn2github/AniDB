package processing;

import java.io.File;
import java.util.EnumSet;
import java.util.TreeMap;

public class FileInfo {

    public FileInfo(File fileObj, int id) {
        this.fileObj = fileObj;
        this.id = id;
        actionsTodo = EnumSet.of(eAction.Process);
        actionsDone = EnumSet.of(eAction.Process);
        actionsDone.remove(eAction.Process);
        actionsError = EnumSet.of(eAction.Process);
        actionsError.remove(eAction.Process);
        data = new TreeMap<String, String>();
    }

    private File fileObj;
    private int id;
    private EnumSet<eAction> actionsTodo;
    private EnumSet<eAction> actionsDone;
    private EnumSet<eAction> actionsError;
    private eMLStorageState mlStorage;
    private TreeMap<String, String> data;
    private Boolean watched;
    private boolean served;
    private boolean isFinal;

    public enum eAction { Process, FileCmd, MyListCmd, VoteCmd, Rename, }
    public enum eMLStorageState { Unknown, Internal, External, Deleted }

    public Integer Id() { return id; }
    public File FileObj() { return fileObj; }
    public EnumSet<eAction> ActionsTodo() { return actionsTodo; }
    public EnumSet<eAction> ActionsDone() { return actionsDone; }
    public EnumSet<eAction> ActionsError() { return actionsError; }
    public eMLStorageState MLStorage() { return mlStorage; }
    public TreeMap<String, String> Data() { return data; }
    public boolean Served() { return served; }
    public boolean IsFinal() { return isFinal; }
    public Boolean Watched(){ return watched; }

    public void FileObj(File fileObj) { this.fileObj = fileObj; }
    public void ActionsTodo(EnumSet<eAction> actionsTodo) { this.actionsTodo = actionsTodo; }
    public void ActionsDone(EnumSet<eAction> actionsDone) { this.actionsDone = actionsDone; }
    public void ActionsError(EnumSet<eAction> actionsError) { this.actionsError = actionsError; }
    public void MLStorage(eMLStorageState mlStorage) { this.mlStorage = mlStorage; }
    public void Data(TreeMap<String, String> data) { this.data = data; }
    public void Served(boolean served) { this.served = served; }
    public void IsFinal(boolean isFinal) { this.isFinal = isFinal; }
    public void Watched(Boolean watched){ this.watched = watched; }
}
