package avparsing;

/**
 * Subtitle stream data wrapper
 */
public class AVStreamDataSubtitle extends AVStreamData {
	public String language = "";
	public int subtitleType = 0;
	public boolean isForCommentary = false;
	public boolean isForForeignDub = false;
	public boolean isForHearingImpaired = false;
	public boolean isImageSubs = false;
	public boolean isStyledSubs = false;
	public boolean isUnstyledSubs = false;
	public AVStreamDataSubtitle() { super(); }
}
