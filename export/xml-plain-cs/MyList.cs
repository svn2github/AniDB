using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Threading;
using System.Xml;
using System.Xml.Serialization;

namespace Kucing.AniDB.MyList
{

    [Serializable]
    public class MyList
    {
        public User User;
        [XmlElement("GenreN")]
        public List<GenreN> Categories;
        [XmlElement("Anime")]
        public List<Anime> Animes;

        public static string ToHuman(long x)
        {
            if (x < 1000L)
                return x + " B";
            else if (x < 10L * 1024)
                return Math.Round(((double)x) / 1024, 1) + " KB";
            else if (x < 1000L * 1024)
                return Math.Round(((double)x) / 1024, 0) + " KB";
            else if (x < 10L * 1024 * 1024)
                return Math.Round(((double)x) / 1024 / 1024, 1) + " MB";
            else if (x < 1000L * 1024 * 1024)
                return Math.Round(((double)x) / 1024 / 1024, 0) + " MB";
            else if (x < 10L * 1024 * 1024 * 1024)
                return "" + Math.Round(((double)x) / 1024 / 1024 / 1024, 1) + " GB";
            else if (x < 1000L * 1024 * 1024 * 1024)
                return "" + Math.Round(((double)x) / 1024 / 1024 / 1024, 0) + " GB";
            else if (x < 10L * 1024 * 1024 * 1024 * 1024)
                return "" + Math.Round(((double)x) / 1024 / 1024 / 1024 / 1024, 1) + " TB";
            else
                return "" + Math.Round(((double)x) / 1024 / 1024 / 1024 / 1024, 0) + " TB";
        }

        public static MyList Load(string fileName)
        {
            MyList result = null;
            var currentCulture = Thread.CurrentThread.CurrentCulture;
            try
            {
                Thread.CurrentThread.CurrentCulture = new CultureInfo("");
                var serializer = new XmlSerializer(typeof(MyList));
                using (var reader = new StreamReader(fileName)) result = (MyList)serializer.Deserialize(reader);
            }
            finally
            {
                Thread.CurrentThread.CurrentCulture = currentCulture;
            }
            return result;
        }

        public void Save(string fileName)
        {
            var currentCulture = Thread.CurrentThread.CurrentCulture;
            try
            {
                Thread.CurrentThread.CurrentCulture = new CultureInfo("");
                var serializer = new XmlSerializer(typeof(MyList));
                using (var writer = new StreamWriter(fileName)) serializer.Serialize(writer, this);
            }
            finally
            {
                Thread.CurrentThread.CurrentCulture = currentCulture;
            }
        }
    }

    [Serializable]
    public class User
    {
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public DateTime Date;
        [XmlAttribute]
        public int AnimeCount;
        [XmlAttribute]
        public int EpCount;
        [XmlAttribute]
        public int FileCount;
        [XmlAttribute]
        public long ByteCount;
        public string ByteCountH { get { return MyList.ToHuman(this.ByteCount); } }
        [XmlAttribute]
        public string AnimeDBUrl;
        [XmlAttribute]
        public int AnimeAdded;
        [XmlAttribute]
        public int EpsAdded;
        [XmlAttribute]
        public int FilesAdded;
        [XmlAttribute]
        public int GroupsAdded;
        [XmlAttribute]
        public int LameFiles;
        [XmlAttribute]
        public int LameFilesP;
        [XmlAttribute]
        public int IndependenceP;
        [XmlAttribute]
        public int LeechP;
        [XmlAttribute]
        public int Reviews;
        [XmlAttribute]
        public int Votes;
        [XmlAttribute]
        public int TotalViewedP;
        [XmlAttribute]
        public int TotalOwnedP;
        [XmlAttribute]
        public int OwnViewedP;
        [XmlAttribute]
        public int ViewedEpCnt;

        public override string ToString()
        {
            return /*this.UserId + "\t" +*/ this.Name;
        }
    }


    [Serializable]
    public class GenreN
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public int ParentId;
        [XmlAttribute]
        public bool IsHentai;

        public GenreN Parent;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class AnimeGenreN : GenreN
    {
        [XmlAttribute]
        public int Weight;
        [XmlAttribute]
        public string WeightName;
    }

    [Serializable]
    public class Anime
    {
        public int Id;
        public int Eps;
        public int EpsSpecial;
        public int EpsTotal { get { return this.Eps + this.EpsSpecial; } }
        public int YearStart, YearEnd;
        public string Year { get { return this.YearStart.ToString() + (this.YearStart == this.YearEnd ? "" : "-" + this.YearEnd); } }

        public string Url;
        public string Other;
        public DateTime Date;

        protected long FUpdate;
        public long Update { get { return this.FUpdate; } set { this.FUpdate = value; if (value != 0) this.UpdateDate = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc).AddSeconds(value); } }
        public DateTime UpdateDate;
        public string UpdateShort;
        public DateTime StartDate;

        public DateTime? EndDate;

        public float Rating;
        public int Votes;
        public float TmpRating;
        public int TmpVotes;
        public float ReviewRating;
        public int ReviewVotes;
        public int TypeId;
        public string TypeName;

        public int MyEps;
        public int MyEpsSpecial;
        public int MyEpsTotal { get { return this.MyEps + this.MyEpsSpecial; } }
        public int MyWatchedEps;
        public int MyWatchedEpsSpecial;
        public int MyWatchedEpsTotal { get { return this.MyWatchedEps + this.MyWatchedEpsSpecial; } }
        public int MyUnwatchedEps;
        public int MyUnwatchedEpsSpecial;
        public int MyUnwatchedEpsTotal { get { return this.MyUnwatchedEps + this.MyUnwatchedEpsSpecial; } }
        public long MySize;
        public string MySizeH { get { return MyList.ToHuman(this.MySize); } }
        public bool IsComplete;
        public bool IsWatched;

        public string Name;
        public string NameLangName;
        public string NameLangSName;
        public string TitleJapKanji;
        public bool TitleHasJapKanji { get { return !string.IsNullOrEmpty(this.TitleJapKanji); } }
        public string TitleEng;
        public bool TitleHasEng { get { return !string.IsNullOrEmpty(this.TitleEng); } }
        public string TitleOther;
        public bool TitleHasOther { get { return !string.IsNullOrEmpty(this.TitleOther); } }

        public bool HasAwards;
        public string AwardIcons;

        public int MyState;
        public string MyStateString;
        public string MyStateIcon;
        public bool MyStateUndef;
        public bool MyStateUnknown;
        public bool MyStateOnHdd;
        public bool MyStateOnCD;
        public bool MyStateDeleted;

        public bool IsInWishList;
        public int? WishListType;
        public string WishListTypeName;
        public int? WishListPriority;
        public string WishListPriorityName;
        public string WishListComment;
        public bool MyIsVoted;
        public float? MyVote;
        public DateTime? MyVoteDate;

        public bool MyIsTmpVoted;
        public float? MyTmpVote;
        public DateTime? MyTmpVoteDate;

        [XmlElement("GenreN")]
        public List<AnimeGenreN> Categories;
        [XmlElement("Tag")]
        public List<Tag> Tags;
        [XmlElement("Company")]
        public List<Company> Companies;
        [XmlElement("Group")]
        public List<Group> Groups;
        [XmlElement("Ep")]
        public List<Ep> Episodes;


        public override string ToString()
        {
            return /*this.Id + "\t" +*/ this.Name;
        }
    }
    /*[XmlElement("AnimeID")]
    public int ID;
    public string Name;
    public string NameKanji;
    public string NameEnglish;
    [XmlElement("Year")]
    /*[JsonExIgnore]* /
    public Xml_CDataSection _Year;
    public int YearStart { get { return int.Parse(_Year.Value.Substring(0, 4)); ; } }
    public int YearEnd
    {
        get
        {
            if (_Year.Value.Length == 4)
                return int.Parse(_Year.Value.Substring(0, 4));
            else if (_Year.Value.Length == 9)
                return int.Parse(_Year.Value.Substring(5, 4));
            else
                throw new Exception("Anime.Year = " + _Year.Value);
        }
    }
    [XmlElement("URL")]
    public string Url;
    [XmlElement("AnimeDescription")]
    public string Description;
    [XmlElement("Date")]
    /*[JsonExIgnore]* /
    public Xml_CDataSection _Date;
    public DateTime Date { get { return MyAnimeList.ParseDate(_Date); } }
    public string Update; // TODO Update
    [XmlElement("StartDate")]
    /*[JsonExIgnore]* /
    public Xml_CDataSection _StartDate;
    public DateTime StartDate { get { return MyAnimeList.ParseDate(_StartDate); } }
    [XmlElement("EndDate")]
    /*[JsonExIgnore]* /
    public Xml_CDataSection _EndDate;
    public DateTime EndDate { get { return MyAnimeList.ParseDate(_EndDate); } }
    [XmlElement("Rating")]
    /*[JsonExIgnore]* /
    public Xml_CDataSection _Rating;
    public double Rating { get { return MyAnimeList.ParseNaN(_Rating); } }
    public int Votes;
    [XmlElement("TempRating")]
    /*[JsonExIgnore]* /
    public Xml_CDataSection _TempRating;
    public double TempRating { get { return MyAnimeList.ParseNaN(_TempRating); } }
    public int TempVotes;
    [XmlElement("ReviewRating")]
    /*[JsonExIgnore]* /
    public Xml_CDataSection _ReviewRating;
    public double ReviewRating { get { return MyAnimeList.ParseNaN(_ReviewRating); } }
    public int Reviews;
    public Xml_CDataSection MyVote; // TODO MyVote
    public Xml_CDataSection MyVoteDate; // TODO MyVoteDate
    public Xml_CDataSection MyTempVote; // TODO MyTempVote
    public Xml_CDataSection MyTempVoteDate; // TODO MyTempVoteDate
    public int TypeID;
    public string TypeName;
    public int NFOID;
    public string NFOID2;
    public string NFOURL;
    public string NFOURLName;
    public int ANNID;
    public string ANNURL;
    public int AllCinemaID;
    public string AllCinemaURL;
    public Xml_CDataSection AnimePlanetURL; // TODO AnimePlanetURL
    public int Eps;
    public int EpsSpecial;
    public int EpsTotal;
    public int MyEps;
    public int MyEpsSpecial;
    public int MyEpsTotal;
    public int WatchedEps;
    public int WatchedSpecial;
    public int WatchedTotal;
    public int UnWatchedEps;
    public int UnWatchedSpecial;
    public int UnWatchedTotal;
    [XmlElement("ByteCount")]
    /*[JsonExIgnore]* /
    public Xml_CDataSection _ByteCount;
    public long ByteCount { get { return MyAnimeList.ParseLong(_ByteCount); } }
    public Xml_CDataSection MyWishlistType; // TODO MyWishlist*
    public Xml_CDataSection MyWishlistTypeName; // TODO MyWishlistTypeName
    public Xml_CDataSection MyWishlistPriority; // TODO MyWishlistPriority
    public Xml_CDataSection MyWishlistPriorityName; // TODO MyWishlistPriorityName
    public Xml_CDataSection MyWishlistComment; // TODO MyWishlistComment
    public bool Hentai;

    public List<Episode> Episodes;

    public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class Episode // TODO Episode
    {
    public int AnimeID;
    [XmlElement("EpID")]
    public int ID;
    public string EpNo;
    public string EpName;
    public string EpNameRomaji;
    public string EpNameKanji;
    public int EpLength;
    //public string EpAired><![CDATA[25.11.2004 00:00]]></EpAired>
    //public string EpDate><![CDATA[12.09.2004 18:53]]></EpDate>
    //public string EpUpdate><![CDATA[09.06.2005 13:25]]></EpUpdate>
    //public string EpOther><![CDATA[]]></EpOther>
    //public string EpState><![CDATA[0]]></EpState>
    public bool EpStateSpecial;
    public bool EpStateRecap;
    public bool EpStateOp;
    public bool EpStateEnd;
    //public string EpRating><![CDATA[5.25]]></EpRating>
    public int EpVotes;
    //public string EpMyVote><![CDATA[-]]></EpMyVote>
    //public string EpMyVoteDate><![CDATA[-]]></EpMyVoteDate>
    //public string MyEpWatched><![CDATA[]]></MyEpWatched>
    //public string MyEpState><![CDATA[2]]></MyEpState>
    public string MyEpStateString;
    public string MyEpStateIcon;

    public Anime Anime;
    public List<File> Files;

    public override string ToString() { return this.EpName; }
    }

    [Serializable]
    public class File
    {
    public int AnimeID;
    public int EpID;
    [XmlElement("FID")]
    public int ID;
    public long Size;
    public int Length;
    public bool Generic;
    public string FileType;
    public string CRC;
    public string MD5;
    public string SHA1;
    public string TTH;
    public string ed2kName;
    public string ed2kHash;
    public string ed2kLink;
    public int GID;
    public string GName;
    public string GShortName;
    //public string ReleaseDate><![CDATA[-]]></ReleaseDate>
    //public string QualityID><![CDATA[5]]></QualityID>
    //public string QualityName><![CDATA[low]]></QualityName>
    //public string ResName><![CDATA[352x240]]></ResName>
    //public string FileType><![CDATA[10]]></FileType>
    //public string VCount><![CDATA[1]]></VCount>
    //public string VAspectRatio><![CDATA[10]]></VAspectRatio>
    //public string VAspectRatioName><![CDATA[4:3]]></VAspectRatioName>
    //public string VFPS><![CDATA[29970]]></VFPS>
    //public string VFlags><![CDATA[0]]></VFlags>
    //public string VBitRate><![CDATA[582]]></VBitRate>
    //public string VCodecID><![CDATA[3]]></VCodecID>
    //public string VCodecName><![CDATA[DivX3]]></VCodecName>
    //public string ACount><![CDATA[1]]></ACount>
    //public string ABitRate><![CDATA[96]]></ABitRate>
    //public string ACodecID><![CDATA[5]]></ACodecID>
    //public string ACodecName><![CDATA[MP3 CBR]]></ACodecName>
    //public string AChanType><![CDATA[20]]></AChanType>
    //public string AChanTypeName><![CDATA[2.0]]></AChanTypeName>
    //public string ALangID><![CDATA[2]]></ALangID>
    //public string ALangName><![CDATA[japanese]]></ALangName>
    //public string AType><![CDATA[10]]></AType>
    //public string ATypeName><![CDATA[normal]]></ATypeName>
    //public string ABitRate2><![CDATA[0]]></ABitRate2>
    //public string ACodecID2><![CDATA[0]]></ACodecID2>
    //public string ACodecName2><![CDATA[]]></ACodecName2>
    //public string AChanType2><![CDATA[0]]></AChanType2>
    //public string AChanTypeName2><![CDATA[]]></AChanTypeName2>
    //public string ALangID2><![CDATA[0]]></ALangID2>
    //public string ALangName2><![CDATA[]]></ALangName2>
    //public string AType2><![CDATA[0]]></AType2>
    //public string ATypeName2><![CDATA[]]></ATypeName2>
    //public string SubCount><![CDATA[1]]></SubCount>
    //public string SubID><![CDATA[4]]></SubID>
    //public string SubName><![CDATA[english]]></SubName>
    //public string SubFlags><![CDATA[0]]></SubFlags>
    //public string SubType><![CDATA[10]]></SubType>
    //public string SubTypeName><![CDATA[hard]]></SubTypeName>
    //public string SubID2><![CDATA[0]]></SubID2>
    //public string SubName2><![CDATA[]]></SubName2>
    //public string SubFlags2><![CDATA[0]]></SubFlags2>
    //public string SubType2><![CDATA[0]]></SubType2>
    //public string SubTypeName2><![CDATA[]]></SubTypeName2>
    //public string TypeID><![CDATA[4]]></TypeID>
    //public string TypeName><![CDATA[VHS]]></TypeName>
    //public string Other><![CDATA[]]></Other>
    //public string HasComment><![CDATA[0]]></HasComment>
    //public string Date><![CDATA[09.09.2004 11:15]]></Date>
    //public string Update><![CDATA[26.12.2007 19:28]]></Update>
    //public string Storage><![CDATA[]]></Storage>
    //public string Source><![CDATA[]]></Source>
    //public string ListDate><![CDATA[12.04.2008 06:05]]></ListDate>
    //public string ViewDate><![CDATA[12.04.2008 06:05]]></ViewDate>
    //public string State><![CDATA[0]]></State>
    //public string StateCRCOK><![CDATA[0]]></StateCRCOK>
    //public string StateCRCFailed><![CDATA[0]]></StateCRCFailed>
    //public string StateCRCUnverified><![CDATA[1]]></StateCRCUnverified>
    //public string VersionName><![CDATA[v1]]></VersionName>
    //public string MyWatched><![CDATA[1]]></MyWatched>
    //public string MyState><![CDATA[2]]></MyState>
    //public string MyStateString><![CDATA[external storage (cd/dvd/...)]]></MyStateString>
    //public string MyStateIcon><![CDATA[<a class="i_icon i_state_oncd" title="File with status: external storage (cd/dvd/...)" href="http://wiki.anidb.net/w/Filestates" rel="anidb::wiki"><span>SE</span></a>]]></MyStateIcon>
    //public string MyFileState><![CDATA[0]]></MyFileState>
    //public string MyFileStateString><![CDATA[]]></MyFileStateString>
    //public string MyFileStateIcon><![CDATA[]]></MyFileStateIcon>

    public Anime Anime;
    public Episode Episode;

    public override string ToString() { return this.ed2kName; }
    }*.
    }*/

    [Serializable]
    public class Tag
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public string Name;
        private long FDateUnix;
        [XmlAttribute("Date")]
        public long DateUnix { get { return this.FDateUnix; } set { this.FDateUnix = value; if (value != 0) this.Date = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc).AddSeconds(value); } }
        [XmlIgnore]
        public DateTime Date;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class Company
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public string ShortName;
        [XmlAttribute]
        public string OtherName;
        [XmlAttribute]
        public int Type;
        [XmlAttribute]
        public string TypeName;
        [XmlAttribute]
        public string Url;
        [XmlAttribute]
        public string PicUrl;
        [XmlAttribute]
        public int APType;
        [XmlAttribute]
        public string APTypeName;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class Group
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public int AGId;
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public string SName;
        [XmlAttribute]
        public string State;
        [XmlAttribute]
        public string EpRange;
        [XmlAttribute]
        public int LastEp;
        [XmlAttribute]
        public DateTime LastUp;
        [XmlAttribute]
        public int Votes;
        [XmlAttribute]
        public float Rating;
        [XmlAttribute]
        public int Cmts;
        [XmlAttribute]
        public int EpCnt;
        [XmlAttribute]
        public int SEpCnt;
        [XmlAttribute]
        public float MyVote;
        [XmlAttribute]
        public DateTime MyVoteDate;

        [XmlElement("ALang")]
        public List<XLang> AudioLanguages;
        [XmlElement("SLang")]
        public List<XLang> SubLanguages;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class XLang
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public string SName;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class Ep
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public string EpNo;
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public bool HasNameEnglish { get { return !string.IsNullOrEmpty(this.Name); } }
        [XmlAttribute]
        public string NameRomaji;
        [XmlAttribute]
        public bool HasNameRomaji { get { return !string.IsNullOrEmpty(this.NameRomaji); } }
        [XmlAttribute]
        public string NameKanji;
        [XmlAttribute]
        public bool HasNameKanji { get { return !string.IsNullOrEmpty(this.NameKanji); } }
        [XmlAttribute]
        public int Length;
        [XmlAttribute]
        public DateTime Aired;
        
        public string Other;
        [XmlAttribute]
        public float Rating;
        [XmlAttribute]
        public int Votes;
        [XmlAttribute]
        public float MyVote;
        [XmlAttribute]
        public DateTime MyVoteDate;
        [XmlAttribute]
        public DateTime Date;
        [XmlAttribute]
        public DateTime Update;
        [XmlAttribute]
        public int State;
        [XmlAttribute]
        public bool StateSpecial;
        [XmlAttribute]
        public bool StateRecap;
        [XmlAttribute]
        public bool StateOp;
        [XmlAttribute]
        public bool StateEnd;
        [XmlAttribute]
        public bool StateIsWatched;
        [XmlAttribute]
        public bool StateHasFile;
        [XmlAttribute]
        public int MyState;
        [XmlAttribute]
        public string MyStateString;
        /*[XmlAttribute] public string MyStateIcon;*/
        [XmlAttribute]
        public bool MyStateUndef;
        [XmlAttribute]
        public bool MyStateUnknown;
        [XmlAttribute]
        public bool MyStateOnHdd;
        [XmlAttribute]
        public bool MyStateOnCD;
        [XmlAttribute]
        public bool MyStateDeleted;
        [XmlAttribute]
        public bool MyStateRelease;
        [XmlAttribute]
        public bool MyStateShared;
        [XmlAttribute]
        public bool MyStateHanger;

        [XmlElement("File")]
        public List<File> Files;
        [XmlElement("FileEpRel")]
        public List<FileEpRel> FileEpisodeRelations;

        public override string ToString() { return this.EpNo + " - " + this.Name; }
    }

    [Serializable]
    public class File
    {
        public int Id;
        public long Size;
        public string SizeH { get { return MyList.ToHuman(this.Size); } }
        public bool IsGeneric;
        public string FileType;
        public string Crc;
        public string Md5;
        public string Sha1;
        public string Tth;
        public string Ed2kLink;
        public string Ed2kHash;
        public string Ed2kName;
        public int GroupId;
        public string GroupName;
        public string GroupShortName;
        public DateTime ReleaseD;
        public int QualId;
        public string QualName;
        public string ResName;
        public int VidCnt;
        public int AR;
        public string ARName;
        public int Fps;
        public int VBitRate;
        public int VCodecId;
        public string VCodecName;
        public int VFlags;

        [XmlElement("FileRel")]
        public List<FileRel> FileFileRelations;

        public override string ToString() { return this.Ed2kName; }
    }

    [Serializable]
    public class FileRel
    {
        [XmlAttribute]
        public int FId;
        [XmlAttribute]
        public int OtherFId;
        [XmlAttribute]
        public string Type;

        public override string ToString() { return this.FId + " " + this.Type + " " + this.OtherFId; }
    }

    [Serializable]
    public class FileEpRel
    {
        [XmlAttribute]
        public int FId;
        [XmlAttribute]
        public int EId;
        [XmlAttribute]
        public int StartP;
        [XmlAttribute]
        public int EndP;
    }
}