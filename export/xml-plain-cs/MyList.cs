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
            return this.Name;
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
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public int Eps;
        [XmlAttribute]
        public int EpsSpecial;
        [XmlAttribute]
        public int EpsTotal;
        [XmlAttribute]
        public int YearStart, YearEnd;
        public string Year { get { return this.YearStart.ToString() + (this.YearStart == this.YearEnd ? "" : "-" + this.YearEnd); } }

        [XmlAttribute]
        public string Url;
        [XmlAttribute]
        public string Other;
        [XmlAttribute]
        public DateTime Date;
        protected int FUpdate;
        [XmlAttribute]
        public int Update { get { return this.FUpdate; } set { this.FUpdate = value; if (value != 0) this.UpdateDate = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc).AddSeconds(value); } }
        [XmlAttribute]
        public DateTime UpdateDate;
        [XmlAttribute]
        public DateTime StartDate;
        [XmlAttribute]
        public DateTime EndDate;
        [XmlAttribute]
        public float Rating;
        [XmlAttribute]
        public int Votes;
        [XmlAttribute]
        public float TmpRating;
        [XmlAttribute]
        public int TmpVotes;
        [XmlAttribute]
        public float ReviewRating;
        [XmlAttribute]
        public int ReviewVotes;
        [XmlAttribute]
        public int TypeId;
        [XmlAttribute]
        public string TypeName;
        [XmlAttribute]
        public int AnimeNfoId;
        [XmlAttribute]
        public string AnimeNfoId2;
        [XmlAttribute]
        public string AnimeNfoUrl;
        [XmlAttribute]
        public string AnimeNfoUrlName;
        [XmlAttribute]
        public int AnnId;
        [XmlAttribute]
        public bool AnnListed;
        [XmlAttribute]
        public string AnnUrl;
        [XmlAttribute]
        public int AllCinemaId;
        [XmlAttribute]
        public bool AllCinemaListed;
        [XmlAttribute]
        public string AllCinemaUrl;
        [XmlAttribute]
        public int AnimePlanetId;
        [XmlAttribute]
        public bool AnimePlanetListed;
        [XmlAttribute]
        public string AnimePlanetUrl;
        [XmlAttribute]
        public bool IsRestricted;
        [XmlAttribute]
        public int MyEps;
        [XmlAttribute]
        public int MyEpsSpecial;
        [XmlAttribute]
        public int MyEpsTotal;
        [XmlAttribute]
        public int MyWatchedEps;
        [XmlAttribute]
        public int MyWatchedEpsSpecial;
        [XmlAttribute]
        public int MyWatchedEpsTotal;
        [XmlAttribute]
        public int MyUnwatchedEps;
        [XmlAttribute]
        public int MyUnwatchedEpsSpecial;
        [XmlAttribute]
        public int MyUnwatchedEpsTotal;
        [XmlAttribute]
        public long MySize;
        public string MySizeH { get { return MyList.ToHuman(this.MySize); } }
        [XmlAttribute]
        public bool IsComplete;
        [XmlAttribute]
        public bool IsWatched;
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public int NameLangId;
        [XmlAttribute]
        public string NameLangName;
        [XmlAttribute]
        public string NameLangSName;
        [XmlAttribute]
        public string TitleJapKanji;
        [XmlAttribute]
        public bool TitleHasJapKanji;
        [XmlAttribute]
        public string TitleEng;
        [XmlAttribute]
        public bool TitleHasEng;
        [XmlAttribute]
        public string TitleOther;
        [XmlAttribute]
        public bool TitleHasOther;
        [XmlAttribute]
        public bool HasAwards;
        [XmlAttribute]
        public string AwardIcons;
        [XmlAttribute]
        public int MyState;
        [XmlAttribute]
        public string MyStateString;
        [XmlAttribute]
        public string MyStateIcon;
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
        public bool IsInWishList;
        [XmlAttribute]
        public int WishListType;
        [XmlAttribute]
        public string WishListTypeName;
        [XmlAttribute]
        public int WishListPriority;
        [XmlAttribute]
        public string WishListPriorityName;
        [XmlAttribute]
        public string WishListComment;
        [XmlAttribute]
        public bool MyIsVoted;
        [XmlAttribute]
        public float MyVote;
        [XmlAttribute]
        public DateTime MyVoteDate;
        [XmlAttribute]
        public bool MyIsTmpVoted;
        [XmlAttribute]
        public float MyTmpVote;
        [XmlAttribute]
        public DateTime MyTmpVoteDate;

        [XmlElement("GenreN")]
        public List<AnimeGenreN> Categories;
        [XmlElement("Tag")]
        public List<Tag> Tags;
        [XmlElement("Company")]
        public List<Company> Companies;
        [XmlElement("Group")]
        public List<Group> Groups;
        [XmlElement("Title")]
        public List<AnimeTitle> Titles;
        [XmlElement("AwardType")]
        public List<AwardType> AwardTypes;
        [XmlElement("Award")]
        public List<Award> Awards;
        [XmlElement("Ep")]
        public List<Ep> Episodes;

        public override string ToString()
        {
            return this.Name;
        }
    }

    [Serializable]
    public class Tag
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public string Name;
        private int FDateUnix;
        [XmlAttribute("Date")]
        public int DateUnix { get { return this.FDateUnix; } set { this.FDateUnix = value; if (value != 0) this.Date = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc).AddSeconds(value); } }
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
    public class EpTitle
    {
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public int LangId;
        [XmlAttribute]
        public string LangName;
        [XmlAttribute]
        public string LangSName;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class AnimeTitle : EpTitle
    {
        [XmlAttribute]
        public int TypeId;
        [XmlAttribute]
        public string TypeString;
    }

    [Serializable]
    public class AwardType
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public string Img;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class Award
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public string Name;
        [XmlAttribute]
        public string Type;
        [XmlAttribute]
        public string Url;
        [XmlAttribute]
        public string PicUrl;

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
        [XmlAttribute]
        public string MyStateIcon;
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

        [XmlElement("Title")]
        public List<EpTitle> Titles;
        [XmlElement("File")]
        public List<File> Files;
        [XmlElement("FileEpRel")]
        public List<FileEpRel> FileEpisodeRelations;

        public override string ToString() { return this.EpNo + " - " + this.Name; }
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

    [Serializable]
    public class File
    {
        [XmlAttribute]
        public int Id;
        [XmlAttribute]
        public int LId;
        [XmlAttribute]
        public long SizePlain;
        public string SizeH { get { return MyList.ToHuman(this.SizePlain); } }
        [XmlAttribute]
        public bool IsGeneric;
        [XmlAttribute]
        public string FileType;
        [XmlAttribute]
        public string Crc;
        [XmlAttribute]
        public string Md5;
        [XmlAttribute]
        public string Sha1;
        [XmlAttribute]
        public string Tth;
        [XmlAttribute]
        public string Ed2kLink;
        [XmlAttribute]
        public string Ed2kHash;
        [XmlAttribute]
        public string Ed2kName;
        [XmlAttribute]
        public int GroupId;
        [XmlAttribute]
        public string GroupName;
        [XmlAttribute]
        public string GroupShortName;
        [XmlAttribute]
        public DateTime ReleaseD;
        [XmlAttribute]
        public int QualId;
        [XmlAttribute]
        public string QualName;
        [XmlAttribute]
        public string ResName;
        [XmlAttribute]
        public int VidCnt;
        [XmlElement("Video")]
        public List<Video> Videos;
        [XmlAttribute]
        public int AudCnt;
        [XmlElement("Audio")]
        public List<Audio> Audios;
        [XmlAttribute]
        public int SubCnt;
        [XmlElement("Sub")]
        public List<Sub> Subs;
        [XmlAttribute] public int Length;
        [XmlAttribute]
        public int Type;
        [XmlAttribute]
        public string TypeName;
        [XmlAttribute]
        public int SourceId;
        [XmlAttribute]
        public string SourceName;
        [XmlAttribute] public DateTime LDate;
        [XmlAttribute] public DateTime Date;
        [XmlAttribute]
        public DateTime Update;
        [XmlAttribute] public string Storage;
        [XmlAttribute] public string Source;
        [XmlAttribute] public bool HasComment;
        [XmlAttribute] public string Other;
        [XmlAttribute] public DateTime ViewDate;
        [XmlAttribute] public int State;
        [XmlAttribute]
        public bool StateCrcOk;
        [XmlAttribute] public bool StateCrcFailed;
        [XmlAttribute] public bool StateCrcUnverified;
        [XmlAttribute] public bool StateIsV2;
        [XmlAttribute] public bool StateIsV3;
        [XmlAttribute] public bool StateIsV4;
        [XmlAttribute] public bool StateIsV5;
        [XmlAttribute] public bool IsWathed;
        [XmlAttribute] public string StateVersionName;
        [XmlAttribute]
        public string MyOther;
        [XmlAttribute]
        public int MyState;
        [XmlAttribute]
        public string MyStateString;
        [XmlAttribute] public string MyStateIcon;
        [XmlAttribute] public bool MyStateUnknown;
        [XmlAttribute] public bool MyStateOnHdd;
        [XmlAttribute] public bool MyStateOnCD;
        [XmlAttribute] public bool MyStateDeleted;
        [XmlAttribute] public int MyFileState;
        [XmlAttribute] public string MyFileStateString;
        [XmlAttribute] public string MyFileStateIcon;
        [XmlAttribute]
        public bool MyFileStateNormal;
        [XmlAttribute] public bool MyFileStateInvalidCrc;
        [XmlAttribute] public bool MyFileStateSelfEdited;
        [XmlAttribute] public bool MyFileStateSelfRipped;
        [XmlAttribute] public bool MyFileStateOnDvd;
        [XmlAttribute] public bool MyFileStateOnVhs;
        [XmlAttribute] public bool MyFileStateOnTV;
        [XmlAttribute] public bool MyFileStateTheater;
        [XmlAttribute] public bool MyFileStateOther;
        [XmlAttribute] public bool MyFileStateRelease;
        [XmlAttribute]
        public bool MyFileStateShared;


        [XmlElement("FileRel")]
        public List<FileRel> FileFileRelations;

        public override string ToString() { return this.Ed2kName; }
    }

    [Serializable]
    public class Video
    {
        [XmlAttribute]
        public int AR;
        [XmlAttribute]
        public string ARName;
        [XmlAttribute]
        public float Fps;
        [XmlAttribute]
        public int BitRate;
        [XmlAttribute]
        public int CodecId;
        [XmlAttribute]
        public string CodecName;
        [XmlAttribute]
        public int Flags;

        public override string ToString() { return this.CodecName; }
    }

    [Serializable]
    public class Audio
    {
        [XmlAttribute]
        public int Type;
        [XmlAttribute]
        public string TypeName;
        [XmlAttribute]
        public int BitRate;
        [XmlAttribute]
        public int CodecId;
        [XmlAttribute]
        public string CodecName;
        [XmlAttribute]
        public int ChanType;
        [XmlAttribute]
        public string ChanTypeName;
        [XmlAttribute]
        public int LangId;
        [XmlAttribute]
        public string LangName;
        [XmlAttribute]
        public string LangSName;

        public override string ToString() { return this.LangName; }
    }

    [Serializable]
    public class Sub
    {
        [XmlAttribute]
        public int Flags;
        [XmlAttribute]
        public int Type;
        [XmlAttribute]
        public string TypeName;
        [XmlAttribute]
        public int SubId;
        [XmlAttribute]
        public string SubName;
        [XmlAttribute]
        public string SubSName;

        public override string ToString() { return this.SubName; }
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
}