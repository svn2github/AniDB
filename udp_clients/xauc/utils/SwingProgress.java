package utils;

import javax.swing.JProgressBar;

/**
 * Swing Progress Bar handler
 */
public class SwingProgress extends Progress {
	protected JProgressBar progressBar;

	public SwingProgress() { super(); this.showProgress = false; }
	public SwingProgress(float progress) { super(progress); };
	public SwingProgress(float progress, String action) { super(progress,action); }
	public SwingProgress(JProgressBar progressBar) { this.progressBar = progressBar; }
	
	/** @return the progressBar */
	public synchronized JProgressBar getProgressBar() { return progressBar; }
	/** @param progressBar the progressBar to set */
	public synchronized void setProgressBar(JProgressBar progressBar) { this.progressBar = progressBar; }

	/** @param progress the progress to set */
	public synchronized void setProgress(float progress) { 
		this.progress = progress;
		progressBar.setValue((int)(progress*100));
		progressBar.setStringPainted(true);
		if (showProgress)
			System.out.print(this.action+": "+((int)(progress*100))+"%\r");
	}
}
