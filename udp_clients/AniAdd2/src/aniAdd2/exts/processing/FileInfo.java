/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.processing;

import java.io.File;
import java.util.EnumSet;

/**
 *
 * @author Arokh
 */
public class FileInfo {
    private String path;

    private EnumSet<Action> actionsTodo;
    private EnumSet<Action> actionsDone;
    private EnumSet<Action> actionsError;
    private boolean served;

	public EnumSet<Action> getActionsTodo() { return actionsTodo; }
    public EnumSet<Action> getActionsDone() { return actionsDone; }
    public EnumSet<Action> getActionsError() { return actionsError; }
    public String getPath() { return path; }
    public boolean isServed() { return served; }

	public FileInfo(String path) {
		this.path = path;

		actionsTodo = EnumSet.of(Action.Process);
		actionsTodo.remove(Action.Process);

		actionsDone = EnumSet.of(Action.Process);
		actionsDone.remove(Action.Process);

		actionsError = EnumSet.of(Action.Process);
		actionsError.remove(Action.Process);
	}




    public enum Action { Process, FileCmd, MyListCmd, VoteCmd, Rename, }
    public enum MLStorageState { Unknown, Internal, External, Deleted }
}
