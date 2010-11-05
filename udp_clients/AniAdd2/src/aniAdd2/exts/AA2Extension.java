/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package aniAdd2.exts;

import aniAdd2.IAniAdd2;
import aniAdd2.communication.ComEvent;
import aniAdd2.communication.Communication;
import aniAdd2.communication.IComListener;

/**
 *
 * @author Arokh
 */
public abstract class AA2Extension extends Communication implements IAA2Extension {
	private State state;
	protected IAniAdd2 aniAdd2;

	public AA2Extension() {
		setState(State.New);
	}

	public void initialize(IAniAdd2 aniAdd2) {
		setState(State.Initializing);

		this.aniAdd2 = aniAdd2;
		aniAdd2.addComListener(new IComListener() {
			public void eventHandler(ComEvent comEvent) {
				parentEventHandler(comEvent);
			}
		});

		initializeInternal();
	}

	protected abstract void initializeInternal();



	public State getState() {
		return state;
	}

	protected final void setState(State state) {
		this.state = state;
		comFire(new ComEvent(this, ComEvent.ComType.Information, state));
	}
}
