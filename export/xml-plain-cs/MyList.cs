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
        public int Id;
        public int Eps;
        public int EpsSpecial;
        public int EpsTotal { get { return this.Eps + this.EpsSpecial; } }
        public int YearStart, YearEnd;
        public string Year { get { return this.YearStart.ToString() + (this.YearStart == this.YearEnd ? "" : "-" + this.YearEnd); } }

        public string Url;
        public string Other;
        public DateTime Date;

        protected long _Update;
        public long Update { get { return this._Update; } set { this._Update = value; this.UpdateDate = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc).AddSeconds(value); } }
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
        private long FDateUnix;
        [XmlAttribute]
        public long DateUnix { get { return this.FDateUnix; } set { this.FDateUnix = value; this.Date = new DateTime(1970, 1, 1, 0, 0, 0, DateTimeKind.Utc).AddSeconds(value); } }
        [XmlIgnore]
        public DateTime Date;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class Company
    {
        public int Id;
        public string Name;
        public string ShortName;
        public string OtherName;
        public int Type;
        public string Url;
        public string PicUrl;
        public int APType;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class Group
    {
        public int Id;
        public int AGId;
        public string Name;
        public string SName;
        public string State;
        public int LastEp;
        public DateTime LastUp;
        public int Votes;
        public float Rating;
        public int Cmts;
        public int EpCnt;
        public int SEpCnt;
        public float MyVote;
        public DateTime? MyVoteDate;

        public override string ToString() { return this.Name; }
    }

    [Serializable]
    public class Ep
    {
        public int Id;
        public string EpNo;
        public string Name;
        public string NameRomaji;
        public bool HasNameRomaji { get { return !string.IsNullOrEmpty(this.NameRomaji); } }
        public string NameKanji;
        public bool HasNameKanji { get { return !string.IsNullOrEmpty(this.NameKanji); } }
        public int Length;
        public DateTime Aired;
        public string Other;
        public float Rating;
        public int Votes;
        public float MyVote;
        public DateTime MyVoteDate;
        public DateTime Date;
        public DateTime Update;
        public int State;
        public bool StateSpecial;
        public bool StateRecap;
        public bool StateOp;
        public bool StateEnd;
        public bool StateIsWatched;
        public bool StateHasFile;
        public int MyState;
        public string MyStateString;
        public string MyStateIcon;
        public bool MyStateUndef;
        public bool MyStateUnknown;
        public bool MyStateOnHdd;
        public bool MyStateOnCD;
        public bool MyStateDeleted;

        [XmlElement("File")]
        public List<File> Files;

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
        [XmlElement("FileEpRel")]
        public List<FileEpRel> FileEpisodeRelations;

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