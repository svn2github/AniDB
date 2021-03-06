/** test
 * @author Arokh
 * @email DvdKhl@googlemail.com
 */
package UI;

import java.awt.Color;
import java.awt.Component;
import java.util.ArrayList;
import java.util.ArrayList;
import javax.swing.JTable;
import javax.swing.table.TableCellRenderer;

public class ListView extends JTable{
    cListViewItems Items = new cListViewItems();
    javax.swing.table.DefaultTableModel TableModel;
    
    public ListView() {
        super();
        TableModel = new javax.swing.table.DefaultTableModel();         
        setModel(TableModel);
    }
    
    @Override
    public Component prepareRenderer(TableCellRenderer renderer, int row, int column) {
        Component component = super.prepareRenderer(renderer, row, column);

        if(Items.Item(row).Columns.get(column).BGColor != null){
            component.setBackground(Items.Item(row).Columns.get(column).BGColor);
        } else {
            component.setBackground(Color.white);
        }
        
        for(int Index : this.getSelectedRows()) if(Index==row) {
            component.setBackground(MixColor(Items.Item(row).Columns.get(column).BGColor, Color.lightGray));            
        }
        
        return component;
    }
    public boolean isCellEditable(int row, int column) {
        return Items.Rows.get(row).Columns.get(column).IsEditable;
    }
    
    public Color MixColor(Color C1, Color C2) {
        if(C1 == null && C2 == null) return Color.white;
        if(C1 == null) return C2;
        if(C2 == null) return C1;
        
        int R, B, G;
        R = (C1.getRed() + C2.getRed()) / 2;
        G = (C1.getGreen() + C2.getGreen()) / 2;
        B = (C1.getBlue() + C2.getBlue()) / 2;
        Color MColor = new Color(R,G,B);
        return MColor;
    }
    
    public void SetColumns(String[] Columns) {
        for (String Column : Columns) TableModel.addColumn(Column);
        TableModel.fireTableStructureChanged();
    }
    
    public class cListViewItems {
        ArrayList<cListViewItemRow> Rows = new ArrayList<cListViewItemRow>();
        
        public void Add(cListViewItemRow Item) {
            Rows.add(Item);
            TableModel.addRow(GetRowValues(Item));
            TableModel.fireTableRowsInserted(Rows.size()-1, Rows.size()-1);
        }
        public void Insert(cListViewItemRow Item, int Index){
            Rows.add(Index, Item);
            TableModel.insertRow(Index, GetRowValues(Item));
            TableModel.fireTableRowsInserted(Index, Index);
        }
        public void Edit(cListViewItemRow Item, int Index) {
            Rows.set(Index, Item);
            for (int I=0; I<Item.Columns.size(); I++ ) TableModel.setValueAt(Item.Columns.get(I), Index, I);
            TableModel.fireTableRowsUpdated(Index, Index);
        }
        public cListViewItemRow Item(int Index){
            return Rows.get(Index); //Todo: create new Item to avoid inconsistensies
        }
        public cListViewItemRow Remove(int Index){
            TableModel.removeRow(Index);
            TableModel.fireTableRowsDeleted(Index, Index);
            return Rows.remove(Index);
        }
    }
    
    static Object[] GetRowValues(cListViewItemRow Item) {
        ArrayList<Object> Values = new ArrayList<Object>();
        for (int I=0; I<Item.Columns.size(); I++) Values.add(Item.Columns.get(I).Value);
        return Values.toArray();
    }
    
}
