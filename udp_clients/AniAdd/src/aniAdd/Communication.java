/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd;

import aniAdd.Modules.IModule;
import aniAdd.misc.Misc;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.EventListener;
import java.util.EventObject;

/**
 *
 * @author Arokh
 */
public interface Communication {
	//private ArrayList<ComListener> listeners;

	/*protected void ComFire(ComEvent comEvent){
	for (ComListener listener : listeners) {
	listener.EventHandler(comEvent);
	}
	}*/
	//public void AddComListener(ComListener comListener){ listeners.add(comListener); }
	//public void RemoveComListener(ComListener comListener){ listeners.remove(comListener); }
	void addComListener(ComListener comListener);

	void RemoveComListener(ComListener comListener);

	public static interface ComListener extends EventListener {
		void EventHandler(ComEvent comEvent);
	}

	public static class ComEvent extends EventObject {
		Date createdOn;
		eType type;
		ArrayList<Object> params;

		public ComEvent(Object source, eType type, ArrayList<Object> params) {
			this(source, type, params.toArray());
		}

		public ComEvent(Object source, eType type, Object... params) {
			this(source, type);

			this.params = new ArrayList<Object>();
			for(Object param : params) {
				this.params.add(param);
			}
		}

		public ComEvent(Object source, eType type) {
			this(source);
			this.type = type;
		}

		private ComEvent(Object source) {
			super(source);
			createdOn = new Date();
		}

		public eType Type() {
			return type;
		}

		public Object Params(int i) {
			return params.get(i);
		}

		public int ParamCount() {
			return params.size();
		}

		@Override
		public String toString() {
			String str;
			str = Misc.DateToString(createdOn, "HH:mm:ss") + " " + type + ": " + (getSource() instanceof IModule ? (((IModule)getSource()).ModuleName()) : "");
			for(int i = 0; i < ParamCount(); i++) str += " " + Params(i);

			return str;
		}

		public enum eType {
			Debug, Information, Manipulation, Warning, Error, Fatal
		}
	}
}
