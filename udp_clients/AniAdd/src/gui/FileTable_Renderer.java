/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package gui;

import java.awt.Component;
import java.awt.Graphics;
import java.awt.Image;
import java.io.IOException;
import java.util.TreeMap;
import javax.imageio.ImageIO;
import javax.swing.JTable;
import javax.swing.table.DefaultTableCellRenderer;
import processing.FileInfo;
import processing.FileInfo.eAction;

/**
 *
 * @author Arokh
 */
public class FileTable_Renderer extends DefaultTableCellRenderer{
    ImgCell cellComponent;

    public FileTable_Renderer() {cellComponent = new ImgCell();}

	@Override
    public Component getTableCellRendererComponent(JTable table, Object value, boolean isSelected, boolean hasFocus, int row, int column) {
        //renderer = new ImgCell();
        FileInfo fileInfo = (FileInfo)value;

        if(column==0){
            return super.getTableCellRendererComponent(table, fileInfo.FileObj().getName(), isSelected, hasFocus, row, column);
        } else {
            cellComponent.setFile(fileInfo);
            return cellComponent;
        }
    }


    class ImgCell extends Component{
        FileInfo fileInfo;
        TreeMap<Integer, Image> icons;
        
        public ImgCell() {
            icons = new TreeMap<Integer, Image>();
            icons.put(eIconType.F.getPos() | eIconType.Gray.getPos(), createImageIcon("icons/FGray.png"));
            icons.put(eIconType.F.getPos() | eIconType.Yellow.getPos(), createImageIcon("icons/FYellow.png"));
            icons.put(eIconType.F.getPos() | eIconType.Red.getPos(), createImageIcon("icons/FRed.png"));
            icons.put(eIconType.F.getPos() | eIconType.Green.getPos(), createImageIcon("icons/FGreen.png"));
            icons.put(eIconType.F.getPos() | eIconType.Blue.getPos(), createImageIcon("icons/FBlue.png"));

            icons.put(eIconType.M.getPos() | eIconType.Gray.getPos(), createImageIcon("icons/MGray.png"));
            icons.put(eIconType.M.getPos() | eIconType.Yellow.getPos(), createImageIcon("icons/MYellow.png"));
            icons.put(eIconType.M.getPos() | eIconType.Red.getPos(), createImageIcon("icons/MRed.png"));
            icons.put(eIconType.M.getPos() | eIconType.Green.getPos(), createImageIcon("icons/MGreen.png"));
            icons.put(eIconType.M.getPos() | eIconType.Blue.getPos(), createImageIcon("icons/MBlue.png"));

            icons.put(eIconType.P.getPos() | eIconType.Gray.getPos(), createImageIcon("icons/PGray.png"));
            icons.put(eIconType.P.getPos() | eIconType.Yellow.getPos(), createImageIcon("icons/PYellow.png"));
            icons.put(eIconType.P.getPos() | eIconType.Red.getPos(), createImageIcon("icons/PRed.png"));
            icons.put(eIconType.P.getPos() | eIconType.Green.getPos(), createImageIcon("icons/PGreen.png"));
            icons.put(eIconType.P.getPos() | eIconType.Blue.getPos(), createImageIcon("icons/PBlue.png"));

            icons.put(eIconType.R.getPos() | eIconType.Gray.getPos(), createImageIcon("icons/RGray.png"));
            icons.put(eIconType.R.getPos() | eIconType.Yellow.getPos(), createImageIcon("icons/RYellow.png"));
            icons.put(eIconType.R.getPos() | eIconType.Red.getPos(), createImageIcon("icons/RRed.png"));
            icons.put(eIconType.R.getPos() | eIconType.Green.getPos(), createImageIcon("icons/RGreen.png"));
            icons.put(eIconType.R.getPos() | eIconType.Blue.getPos(), createImageIcon("icons/RBlue.png"));

            icons.put(eIconType.V.getPos() | eIconType.Gray.getPos(), createImageIcon("icons/VGray.png"));
            icons.put(eIconType.V.getPos() | eIconType.Yellow.getPos(), createImageIcon("icons/VYellow.png"));
            icons.put(eIconType.V.getPos() | eIconType.Red.getPos(), createImageIcon("icons/VRed.png"));
            icons.put(eIconType.V.getPos() | eIconType.Green.getPos(), createImageIcon("icons/VGreen.png"));
        }

        public void setFile(FileInfo fileInfo){this.fileInfo = fileInfo;}

        public void paint(Graphics g) {
            Image[] imgList = getProgressIcons();
            for (int i = 0; i < imgList.length; i++) {
                if(imgList[i]!=null) g.drawImage(imgList[i], i*17+1, 1, null);
            }
        }

        private Image[] getProgressIcons() {
            Image[] iconGroup = new Image[4];
            iconGroup[0] = ImgSelector(eIconType.P, eAction.Process, fileInfo.Served());
            iconGroup[1] = ImgSelector(eIconType.F, eAction.FileCmd, fileInfo.ActionsDone().contains(eAction.Process));
            iconGroup[2] = ImgSelector(eIconType.M, eAction.MyListCmd, fileInfo.ActionsDone().contains(eAction.Process));
            iconGroup[3] = ImgSelector(eIconType.R, eAction.Rename, fileInfo.ActionsDone().contains(eAction.Process));

            return iconGroup;
        }

        private Image ImgSelector(eIconType iconType, eAction action, boolean doingAction) {
            if((fileInfo.ActionsTodo().contains(action))) {
                return icons.get(iconType.getPos() | (doingAction?eIconType.Blue.getPos():eIconType.Yellow.getPos()));
            } else if((fileInfo.ActionsDone().contains(action))) {
                return icons.get(iconType.getPos() | eIconType.Green.getPos());
            } else if((fileInfo.ActionsError().contains(action))) {
                return icons.get(iconType.getPos() | eIconType.Red.getPos());
            } else {
                return icons.get(iconType.getPos() | eIconType.Gray.getPos());
            }
        }

        private Image createImageIcon(String path) {
            java.net.URL imgURL = getClass().getResource(path);
            if (imgURL != null) {
                try {
                    return ImageIO.read(imgURL);
                } catch (IOException ex) {
                    System.err.println("Couldn't find file: " + path);
                    return null;
                }
            } else {
                System.err.println("Couldn't find file: " + path);
                return null;
            }
        }
    }

    private enum eIconType {
        Gray(1<<0),
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
        eIconType(int bit){
            this.bit = bit;
        }
        public int getPos(){
            return bit;
        }
    }
}
