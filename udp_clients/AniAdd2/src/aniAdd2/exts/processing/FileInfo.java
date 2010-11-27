/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.processing;

import java.util.EnumSet;

/**
 *
 * @author Arokh
 */
public class FileInfo {

    private String path;
	//private EnumSet<Action> actionsTodo;
	//private EnumSet<Action> actionsDone;
	//private EnumSet<Action> actionsError;
	private Action process;
	private Action fileCmd;
	private Action mlCmd;
	private Action voteCmd;
	private Action move;
	private MLOptions mlOptions;
	private int vote;
	private boolean locked;

	//public EnumSet<Action> getActionsTodo() { return actionsTodo; }
    //public EnumSet<Action> getActionsDone() { return actionsDone; }
    //public EnumSet<Action> getActionsError() { return actionsError; }
    public String getPath() { return path; }
    public boolean isLocked() { return locked; }

	public FileInfo(String path) {
		this.path = path;

		process = fileCmd = move = voteCmd = mlCmd = Action.None;

		mlOptions = new MLOptions();

		//actionsTodo = EnumSet.of(Action.Process);
		//actionsTodo.remove(Action.Process);

		//actionsDone = EnumSet.of(Action.Process);
		//actionsDone.remove(Action.Process);

		//actionsError = EnumSet.of(Action.Process);
		//actionsError.remove(Action.Process);
	}

	public Action getProcess() {
		return process;
	}

	public Action getFileCmd() {
		return fileCmd;
	}

	public Action getMlCmd() {
		return mlCmd;
	}

	public Action getVoteCmd() {
		return voteCmd;
	}

	public Action getMove() {
		return move;
	}

	public MLOptions getMlOptions() {
		return mlOptions;
	}

	public void setPath(String path) {
		this.path = path;
	}

	public void setProcess(Action process) {
		this.process = process;
	}

	public void setFileCmd(Action fileCmd) {
		this.fileCmd = fileCmd;
	}

	public void setMlCmd(Action mlCmd) {
		this.mlCmd = mlCmd;
	}

	public void setVoteCmd(Action voteCmd) {
		this.voteCmd = voteCmd;
	}

	public void setMove(Action move) {
		this.move = move;
	}

	public void setMlOptions(MLOptions mlOptions) {
		this.mlOptions = mlOptions;
	}

	public void setLocked(boolean locked) {
		this.locked = locked;
	}

	public int getVote() {
		return vote;
	}

	public void setVote(int vote) {
		this.vote = vote;
	}



	public enum Action { None, Todo, Doing, Done, Error }
	public static class MLOptions {
		private MLState state;
		private MLState watchedState;
		private MLStorageState storageState;
		private String other, source, storage;

		public MLOptions() {
			state = MLState.None;
			watchedState = MLState.None;
			storageState = MLStorageState.Internal;
			other = source = storage = "";
		}



		public MLState getState() {
			return state;
		}

		public void setState(MLState state) {
			this.state = state;
		}

		public MLState getWatchedState() {
			return watchedState;
		}

		public void setWatchedState(MLState watchedState) {
			this.watchedState = watchedState;
		}

		public MLStorageState getStorageState() {
			return storageState;
		}

		public void setStorageState(MLStorageState storageState) {
			this.storageState = storageState;
		}

		public String getOther() {
			return other;
		}

		public void setOther(String other) {
			this.other = other == null ? "" : other;
		}

		public String getSource() {
			return source;
		}

		public void setSource(String source) {
			this.source =  source == null ? "" : source;
		}

		public String getStorage() {
			return storage;
		}

		public void setStorage(String storage) {
			this.storage =  storage == null ? "" : storage;
		}

	}
	public enum MLState { None, Set, Unset }

    //public enum Action { Process, FileCmd, MyListCmd, Watched, VoteCmd, Rename, Move, Remove}
    public enum MLStorageState { Unknown, Internal, External, Deleted }
}
