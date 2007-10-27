// Copyright (C) 2005-2006 epoximator
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

package epox.swing;

public class TableSorter{
	TableModelSortable m_mod;
	private int m_last_col, m_last_n;
	private int m_cmp0, m_cmp1;
	private boolean m_desc, m_swit;

	public TableSorter(TableModelSortable model){
		m_mod = model;
		reset();
	}
	public void reset(){
		m_last_col = m_cmp0 = m_cmp1 = 0;
		m_last_n = -1;
		m_desc = m_swit=false;
	}
	public void sort(int[] array, int col, boolean refresh){
		if(array.length<1) return;
		int num = m_mod.getRowCount();
		if(refresh){
			sort0(array, 0, num-1);
			m_desc = m_swit;
			if(m_desc)
				invert(array);
		}else{
			if(m_cmp0!=col)
				m_cmp1 = m_cmp0;
			m_cmp0 = col;

			if(col==m_last_col||m_last_n<0)
				m_swit = !m_swit;

			if(col!=m_last_col || num!=m_last_n){
				sort0(array, 0, num-1);
				m_desc = false;
			}
			if(m_swit || m_desc){
				invert(array);
				m_desc = true;
			}

			m_last_col = col;
			m_last_n = num;
		}
		//System.err.println("1len="+array.length+",col="+col+",lastcol="+m_last_col+",last_n="+m_last_n+",cmp0="+m_cmp0+",cmp1="+m_cmp1+",rf="+refresh+",inv="+m_desc);
	}
	private void invert(int[] a){
		int x = m_mod.getRowCount()-1;
		for (int i=0; i<=x/2; i++)
			swap(a,i,x-i);
	}
	public void sort0(int a[], int l, int h){
		quickSort(a, l, h);
		insertionSort(a, l, h);
	}
	private void quickSort(int a[], int l, int r){
		int M = 4;
		int i;
		int j;
		int v;

		if ((r-l)>M){
			i = (r+l)/2;
			if (compare(l, i)>0) swap(a,l,i);
			if (compare(l, r)>0) swap(a,l,r);
			if (compare(i, r)>0) swap(a,i,r);

			j = r-1;
			swap(a,i,j);
			i = l;
			v = j;
			for(;;){
				do i++;
				while(compare(i, v)<0);
				do j--;
				while(j>0&&compare(v, j)<0);
				if (j<i) break;
				swap (a,i,j);
			}
			swap(a,i,r-1);
			quickSort(a,l,j);
			quickSort(a,i+1,r);
		}
	}
	private void swap(int a[], int i, int j){
		int t;
		t = a[i];
		a[i] = a[j];
		a[j] = t;
	}
	private void insertionSort(int a[], int l, int h){
		int i,j;
		for (i=l; i<=h; i++)
			for (j=i; j>l && compare(j-1,j)>0; j--)
			    swap(a, j, j-1);
	}
	public int compare(int r, int s){
		int x = compare0(m_cmp0, r, s);
		if(x==0) return compare0(m_cmp1, r, s);
		return x;
	}
	public int compare0(int c, int row1, int row2){
		Object o1 = m_mod.getValueAt(row1, c);
		Object o2 = m_mod.getValueAt(row2, c);
		//if (o1 == null && o2 == null) return  0;
		//else if (o1 == null) return -1;
		//else if (o2 == null) return  1;
		//else{
			Class type = m_mod.getColumnClass(c);
			if (type.getSuperclass() == Number.class)
				return (int)(((Number)o1).floatValue()-((Number)o2).floatValue());//compare((Number)o1, (Number)o2);
			else if (type == String.class)
				return ((String)o1).compareToIgnoreCase((String)o2);
				//return ((String)o1).toLowerCase().compareTo(((String)o2).toLowerCase());
			else
				return (o1.toString()).compareToIgnoreCase(o2.toString());
		//}
	}
	public int compare(Number o1, Number o2){
		double n1 = o1.doubleValue();
		double n2 = o2.doubleValue();
		if (n1 < n2)
			return -1;
		else if (n1 > n2)
			return 1;
		else
			return 0;
	}
}