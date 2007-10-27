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

/*
 * Created on 24.10.05
 *
 * @version 	01 (1.14)
 * @author 		epoximator
 */

package epox.util;

/**
 * Combined linked list and hash map. Fast and ordered.
 * @author JV
 * @version 1
 */
public class LinkedHash extends java.util.HashMap/*!<Object,Object>*/{
	private Node head = null, tail = null;
	public LinkedHash(){
		super();
		head = tail = new Node(null, null);
	}
	public boolean addLast(Object o){
		Node n = new Node(o, tail);
		if(super.put(o, n)==null){
			tail = tail.next = n;
			return true;
		}
		return false;
	}
	public Object remove(Object o){
		Node n = (Node) super.remove(o);
		if(n==null)
			System.out.println("! LinkedHash: Tried to remove non existing entry: "+o);
		n.prev.next = n.next;
		if(n.next!=null)
			n.next.prev = n.prev;
		else tail = n.prev;
		return null;
	}
	public void clear(){
		super.clear();
		head.next = head.prev = null;
		tail = head;
	}
	public Object getFirst(){
		if(head.next==null)
			return null;
		return head.next.o;
	}
	private class Node{
		Node prev, next;
		Object o;
		public Node(Object o, Node prev){
			this.o = o;
			this.prev = prev;
		}
	}
}