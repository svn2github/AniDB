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
		fi.getActionsDone().add(FileInfo.Action.FileCmd);
		fi.getActionsDone().add(FileInfo.Action.MyListCmd);
		fi.getActionsDone().add(FileInfo.Action.Process);
		fi.getActionsError().add(FileInfo.Action.Rename);
		files.add(fi);

		fi = new FileInfo("Test2.mkv");
		fi.getActionsDone().add(FileInfo.Action.FileCmd);
		fi.getActionsTodo().add(FileInfo.Action.MyListCmd);
		fi.getActionsDone().add(FileInfo.Action.Process);
		fi.getActionsTodo().add(FileInfo.Action.Rename);
		files.add(fi);

		fi = new FileInfo("Test3.mkv");
		fi.getActionsError().add(FileInfo.Action.FileCmd);
		fi.getActionsError().add(FileInfo.Action.MyListCmd);
		fi.getActionsDone().add(FileInfo.Action.Process);
		fi.getActionsTodo().add(FileInfo.Action.Rename);
		files.add(fi);

		fi = new FileInfo("Test4.mkv");
		fi.getActionsTodo().add(FileInfo.Action.FileCmd);
		fi.getActionsTodo().add(FileInfo.Action.MyListCmd);
		fi.getActionsDone().add(FileInfo.Action.Process);
		fi.getActionsTodo().add(FileInfo.Action.Rename);
		files.add(fi);
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
