/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui.exts.processing.icons;

import aniAdd2.exts.processing.FileInfo;
import aniAdd2.exts.processing.FileInfo.Action;
import java.awt.Component;
import java.awt.Graphics;
import java.awt.Image;
import java.io.IOException;
import java.net.URL;
import java.util.HashMap;
import java.util.TreeMap;
import javax.imageio.ImageIO;

/**
 *
 * @author Arokh
 */
public class IconsCell extends Component {
	private FileInfo fileInfo;
	private final static HashMap<Integer, Image> icons;
	private final static HashMap<Action, Integer> action2IconPos;

	public void setFile(FileInfo fileInfo) {
		this.fileInfo = fileInfo;
	}

	@Override
	public void paint(Graphics g) {
		Image[] imgList = getProgressIcons();
		for(int i = 0; i < imgList.length; i++) {
			if(imgList[i] != null) g.drawImage(imgList[i], i * 17 + 1, 1, null);
		}
	}

	private Image[] getProgressIcons() {
		Image[] iconGroup = new Image[5];
		iconGroup[0] = icons.get(IconType.P.getPos() | action2IconPos.get(fileInfo.getProcess()));
		iconGroup[1] = icons.get(IconType.F.getPos() | action2IconPos.get(fileInfo.getFileCmd()));
		iconGroup[2] = icons.get(IconType.M.getPos() | action2IconPos.get(fileInfo.getMlCmd()));
		iconGroup[3] = icons.get(IconType.V.getPos() | action2IconPos.get(fileInfo.getVoteCmd()));
		iconGroup[4] = icons.get(IconType.R.getPos() | action2IconPos.get(fileInfo.getMove()));
		
		return iconGroup;
	}

	static {
		icons = new HashMap<Integer, Image>();
		icons.put(IconType.F.getPos() | IconType.Gray.getPos(), createImageIcon("FGray.png"));
		icons.put(IconType.F.getPos() | IconType.Yellow.getPos(), createImageIcon("FYellow.png"));
		icons.put(IconType.F.getPos() | IconType.Red.getPos(), createImageIcon("FRed.png"));
		icons.put(IconType.F.getPos() | IconType.Green.getPos(), createImageIcon("FGreen.png"));
		icons.put(IconType.F.getPos() | IconType.Blue.getPos(), createImageIcon("FBlue.png"));

		icons.put(IconType.M.getPos() | IconType.Gray.getPos(), createImageIcon("MGray.png"));
		icons.put(IconType.M.getPos() | IconType.Yellow.getPos(), createImageIcon("MYellow.png"));
		icons.put(IconType.M.getPos() | IconType.Red.getPos(), createImageIcon("MRed.png"));
		icons.put(IconType.M.getPos() | IconType.Green.getPos(), createImageIcon("MGreen.png"));
		icons.put(IconType.M.getPos() | IconType.Blue.getPos(), createImageIcon("MBlue.png"));

		icons.put(IconType.P.getPos() | IconType.Gray.getPos(), createImageIcon("PGray.png"));
		icons.put(IconType.P.getPos() | IconType.Yellow.getPos(), createImageIcon("PYellow.png"));
		icons.put(IconType.P.getPos() | IconType.Red.getPos(), createImageIcon("PRed.png"));
		icons.put(IconType.P.getPos() | IconType.Green.getPos(), createImageIcon("PGreen.png"));
		icons.put(IconType.P.getPos() | IconType.Blue.getPos(), createImageIcon("PBlue.png"));

		icons.put(IconType.R.getPos() | IconType.Gray.getPos(), createImageIcon("RGray.png"));
		icons.put(IconType.R.getPos() | IconType.Yellow.getPos(), createImageIcon("RYellow.png"));
		icons.put(IconType.R.getPos() | IconType.Red.getPos(), createImageIcon("RRed.png"));
		icons.put(IconType.R.getPos() | IconType.Green.getPos(), createImageIcon("RGreen.png"));
		icons.put(IconType.R.getPos() | IconType.Blue.getPos(), createImageIcon("RBlue.png"));

		icons.put(IconType.V.getPos() | IconType.Gray.getPos(), createImageIcon("VGray.png"));
		icons.put(IconType.V.getPos() | IconType.Yellow.getPos(), createImageIcon("VYellow.png"));
		icons.put(IconType.V.getPos() | IconType.Red.getPos(), createImageIcon("VRed.png"));
		icons.put(IconType.V.getPos() | IconType.Green.getPos(), createImageIcon("VGreen.png"));

		action2IconPos = new HashMap<Action, Integer>();
		action2IconPos.put(Action.None, IconType.Gray.getPos());
		action2IconPos.put(Action.Todo, IconType.Yellow.getPos());
		action2IconPos.put(Action.Doing, IconType.Blue.getPos());
		action2IconPos.put(Action.Done, IconType.Green.getPos());
		action2IconPos.put(Action.Error, IconType.Red.getPos());
	}

	private static Image createImageIcon(String path) {
		URL imgURL = IconsCell.class.getResource(path);
		if(imgURL != null) {
			try {
				return ImageIO.read(imgURL);
			} catch(IOException ex) {
				System.err.println("Couldn't find file: " + path);
				return null;
			}
		} else {
			System.err.println("Couldn't find file: " + path);
			return null;
		}
	}

	public static Image getImage(int iconType){
		return icons.get(iconType);
	}

	public enum IconType {
		Gray(1),
		Yellow(1 << 1),
		Red(1 << 2),
		Green(1 << 3),
		Blue(1 << 4),
		F(1 << 23),
		M(1 << 24),
		P(1 << 26),
		R(1 << 27),
		V(1 << 31);
		private int bit;

		IconType(int bit) {
			this.bit = bit;
		}

		public int getPos() {
			return bit;
		}
	}
}
