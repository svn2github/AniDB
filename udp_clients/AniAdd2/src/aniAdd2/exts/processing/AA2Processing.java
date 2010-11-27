/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2.exts.processing;

import aniAdd2.communication.ComEvent;
import aniAdd2.exts.AA2Extension;
import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Arokh
 */
public class AA2Processing extends AA2Extension {
	List<FileInfo> files = new ArrayList<FileInfo>();

	public AA2Processing() {
		super();

		FileInfo fi = new FileInfo("Test1.mkv");
		fi.setProcess(FileInfo.Action.Done);
		fi.setFileCmd(FileInfo.Action.Error);
		fi.setMlCmd(FileInfo.Action.Error);
		fi.setMove(FileInfo.Action.None);
		fi.getMlOptions().setWatchedState(FileInfo.MLState.Unset);
		fi.getMlOptions().setState(FileInfo.MLState.Set);
		fi.setLocked(true);
		files.add(fi);
		
		fi = new FileInfo("Test2.mkv");
		fi.setProcess(FileInfo.Action.Done);
		fi.setFileCmd(FileInfo.Action.Done);
		fi.setMlCmd(FileInfo.Action.Done);
		fi.setMove(FileInfo.Action.Done);
		fi.getMlOptions().setWatchedState(FileInfo.MLState.Set);
		fi.getMlOptions().setState(FileInfo.MLState.Set);
		fi.setLocked(true);
		files.add(fi);

		fi = new FileInfo("Test3.mkv");
		fi.setProcess(FileInfo.Action.Done);
		fi.setFileCmd(FileInfo.Action.Done);
		fi.setMlCmd(FileInfo.Action.Done);
		fi.setMove(FileInfo.Action.Doing);
		fi.getMlOptions().setWatchedState(FileInfo.MLState.Set);
		fi.getMlOptions().setState(FileInfo.MLState.Set);
		fi.setLocked(true);
		files.add(fi);

		fi = new FileInfo("Test4.mkv");
		fi.setProcess(FileInfo.Action.Done);
		fi.setFileCmd(FileInfo.Action.Done);
		fi.setMlCmd(FileInfo.Action.Doing);
		fi.setMove(FileInfo.Action.Todo);
		fi.getMlOptions().setWatchedState(FileInfo.MLState.Set);
		fi.getMlOptions().setState(FileInfo.MLState.Set);
		fi.setLocked(true);
		files.add(fi);

		fi = new FileInfo("Test5.mkv");
		fi.setProcess(FileInfo.Action.Done);
		fi.setFileCmd(FileInfo.Action.Doing);
		fi.setMlCmd(FileInfo.Action.Todo);
		fi.setMove(FileInfo.Action.Todo);
		fi.getMlOptions().setWatchedState(FileInfo.MLState.Set);
		fi.getMlOptions().setState(FileInfo.MLState.Set);
		fi.setLocked(true);
		files.add(fi);

		fi = new FileInfo("Test6.mkv");
		fi.setProcess(FileInfo.Action.Doing);
		fi.setFileCmd(FileInfo.Action.Todo);
		fi.setMlCmd(FileInfo.Action.Todo);
		fi.setMove(FileInfo.Action.Todo);
		fi.getMlOptions().setWatchedState(FileInfo.MLState.Set);
		fi.getMlOptions().setState(FileInfo.MLState.Set);
		fi.setLocked(true);
		files.add(fi);

		fi = new FileInfo("Test7.mkv");
		fi.setProcess(FileInfo.Action.Todo);
		fi.setFileCmd(FileInfo.Action.Todo);
		fi.setMlCmd(FileInfo.Action.None);
		fi.setMove(FileInfo.Action.None);
		fi.getMlOptions().setStorageState(FileInfo.MLStorageState.External);
		files.add(fi);

		fi = new FileInfo("Test8.mkv");
		fi.setProcess(FileInfo.Action.Todo);
		fi.setFileCmd(FileInfo.Action.Todo);
		fi.setMlCmd(FileInfo.Action.Todo);
		fi.setMove(FileInfo.Action.None);
		fi.getMlOptions().setState(FileInfo.MLState.Unset);
		files.add(fi);

		fi = new FileInfo("Test9.mkv");
		fi.setProcess(FileInfo.Action.Todo);
		fi.setFileCmd(FileInfo.Action.Todo);
		fi.setMlCmd(FileInfo.Action.Todo);
		fi.setMove(FileInfo.Action.None);
		fi.getMlOptions().setState(FileInfo.MLState.Set);
		fi.getMlOptions().setWatchedState(FileInfo.MLState.None);
		fi.getMlOptions().setStorageState(FileInfo.MLStorageState.External);
		files.add(fi);

		fi = new FileInfo("Test10.mkv");
		fi.setProcess(FileInfo.Action.Todo);
		fi.setFileCmd(FileInfo.Action.Todo);
		fi.setMlCmd(FileInfo.Action.Todo);
		fi.setMove(FileInfo.Action.None);
		fi.getMlOptions().setState(FileInfo.MLState.Set);
		fi.getMlOptions().setWatchedState(FileInfo.MLState.Unset);
		files.add(fi);

		for(int i = 11; i < 40000; i++) {
			fi = new FileInfo("Test" + i + ".mkv");
			fi.setFileCmd(FileInfo.Action.Todo);
			fi.setMlCmd(FileInfo.Action.Todo);
			fi.setProcess(FileInfo.Action.Todo);
			fi.setMove(FileInfo.Action.Todo);
			fi.setVote((i*100) % 1100);
			fi.setVoteCmd(FileInfo.Action.Todo);
			fi.getMlOptions().setState(FileInfo.MLState.Set);
			fi.getMlOptions().setWatchedState(FileInfo.MLState.Set);
			files.add(fi);
		}
	}


	@Override
	protected void initializeInternal() {
		setState(State.Initialized);
	}

	@Override
	protected void parentEventHandler(ComEvent comEvent) {
	}

	public String getName() {
		return "MLAdd";
	}

	public int fileCount() {
		return files.size();
	}

	public FileInfo index2FileInfo(int rowIndex) {
		return files.get(rowIndex);
	}

}
