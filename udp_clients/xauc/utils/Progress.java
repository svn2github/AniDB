package utils;

/**
 * Progress class, will hold the current progress of something
 * @author fahrenheit
 */
public class Progress {
	
	protected float progress = 0;
	protected String action = "";
	protected String state = "Stopped";
	protected boolean showProgress = true;

	public Progress() {};
	public Progress(float progress) { this.progress = progress; };
	public Progress(float progress, String action) {
		this(progress); 
		this.action = action; 
	}

	/** @return the progress */
	public synchronized float getProgress() { return progress; }
	/** @param progress the progress to set */
	public synchronized void setProgress(float progress) { 
		this.progress = progress;
		if (showProgress)
			System.out.print(this.action+": "+((int)(progress*100))+"%\r");
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
