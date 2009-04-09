package utils;

import javax.swing.JProgressBar;

/**
 * Progress class, will hold the current progress of something
 * <p>Current available constructors:
 * <ul><li>Progress()</li>
 * <li>Progress(float <b>progress</b>)</li>
 * <li>Progress(float <b>progress</b>, String <b>action</b>)</li>
 * <li>Progress(JProgressBar <b>progressBar</b>)</li>
 * </ul></p>
 * @author fahrenheit
 */
public class Progress {
	/** Swing Progress Bar */
	protected JProgressBar progressBar;
	/** Current progress */
	protected float progress = 0;
	/** Current action */
	protected String action = "";
	/** Current state */
	protected String state = "Stopped";
	/** If enabled, current progress will be echoed to system.out */
	protected boolean showProgress = true;

	/**
	 * Default Progress object
	 */
	public Progress() {};
	/**
	 * Progress object constructor
	 * @param progress Current progress
	 */
	public Progress(float progress) { this.progress = progress; };
	/**
	 * Progress object constructor
	 * @param progress Current progress
	 * @param action Current action
	 */
	public Progress(float progress, String action) {
		this(progress); 
		this.action = action; 
	}
	/**
	 * Progress object constructor
	 * @param progressBar Swing Progress Bar
	 */
	public Progress(JProgressBar progressBar) { this.progressBar = progressBar; }

	/** @return the progressBar */
	public synchronized JProgressBar getProgressBar() { return progressBar; }
	/** @param progressBar the progressBar to set */
	public synchronized void setProgressBar(JProgressBar progressBar) { this.progressBar = progressBar; }
	/** @return the progress */
	public synchronized float getProgress() { return progress; }
	/** @param progress the progress to set */
	public synchronized void setProgress(float progress) { 
		this.progress = progress;
		if (progressBar != null) {
			progressBar.setValue((int)(progress*100));
			progressBar.setStringPainted(true);
		}
		if (showProgress)
			System.out.print((!this.action.equals("") ? this.action+": " : "")+((int)(progress*100))+"%\r");
	}
	/** @return the action */
	public synchronized String getAction() { return action; }
	/** @param action the action to set */
	public synchronized void setAction(String action) { this.action = action; }
	/** @return the state */
	public synchronized String getState() { return state; }
	/** @param state the state to set */
	public synchronized void setState(String state) { this.state = state; }
	/** @return the showProgress */
	public synchronized boolean isShowProgress() { return showProgress; }
	/** @param showProgress the showProgress to set */
	public synchronized void setShowProgress(boolean showProgress) { this.showProgress = showProgress; }
}
