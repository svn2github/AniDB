package Processing;

/**
 * @author Arokh
 */
public class cUser {
    public EpProcessing.eRenameStyle Type = EpProcessing.eRenameStyle.dbstandard;
    public eLanguage[] SeriesLang = new eLanguage[] {cUser.eLanguage.Romaji, cUser.eLanguage.English, cUser.eLanguage.Kanji};
    public eLanguage[] EpisodeLang = new eLanguage[] {cUser.eLanguage.English, cUser.eLanguage.Romaji, cUser.eLanguage.Kanji};
    
    public static enum eLanguage{English, Romaji, Kanji}
}
