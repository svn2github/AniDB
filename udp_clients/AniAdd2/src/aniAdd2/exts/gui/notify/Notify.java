/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts.gui.notify;

import java.util.ArrayList;
import java.util.List;

/**
 *
 * @author Arokh
 */
public final class Notify {
	private List<NotifyEventListener> listeners;
	private final Object internalSyncRoot = new Object();
	private final Object eventSyncRoot = new Object();
	private final Object syncRoot = new Object();
	private boolean isClosed;
	private String message;
	private NotifyType type;
	private int id;

	public Notify(String message, NotifyType type, int id) {
		listeners = new ArrayList<NotifyEventListener>();
		this.message = message;
		this.type = type;
		this.id = id;
	}

	public boolean hasClosed() {
		return isClosed;
	}

	public int getId() {
		return id;
	}

	public String getMessage() {
		return message;
	}

	public void setMessage(String message) {
		synchronized(internalSyncRoot) {
			if(isClosed) return;

			this.message = message;

			fireEvent(new FireEvent(this) {
				@Override
				public void invoke(NotifyEventListener l) {
					l.messageChanged(new NotifyEvent(n));
				}
			});
		}
	}

	public NotifyType getType() {
		return type;
	}

	public void setType(NotifyType type) {
		synchronized(internalSyncRoot) {
			if(isClosed) return;

			this.type = type;

			fireEvent(new FireEvent(this) {
				@Override
				public void invoke(NotifyEventListener l) {
					l.typeChanged(new NotifyEvent(n));
				}
			});
		}
	}

	public void closeNotify() {
		synchronized(internalSyncRoot) {
			if(isClosed) return;
			isClosed = true;

			fireEvent(new FireEvent(this) {
				@Override
				public void invoke(NotifyEventListener l) {
					l.closed(new NotifyEvent(n));
				}
			});
		}
	}

	public void addNotifyEventListener(NotifyEventListener l) {
		synchronized(eventSyncRoot) {
			listeners.add(l);
		}
	}

	public void removeNotifyEventListener(NotifyEventListener l) {
		synchronized(eventSyncRoot) {
			listeners.remove(l);
		}
	}

	public Object getSyncRoot() {
		return syncRoot;
	}

	protected void fireEvent(FireEvent fireEvent) {
		synchronized(eventSyncRoot) {
			for(NotifyEventListener notifyEventListener : listeners) fireEvent.invoke(notifyEventListener);
		}
	}

	public static abstract class FireEvent {
		protected Notify n;

		public FireEvent(Notify n) {
			this.n = n;
		}

		public void invoke(NotifyEventListener l) {
		}
	}

	public static enum NotifyType {
		Information, Error, Warning, Success
	}
}
