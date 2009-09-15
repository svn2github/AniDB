using System;
using System.Collections.Generic;
using System.Text;

namespace AVDump2Lib.Information.Parser.MediaInfoLib {
    public enum eStreamKind {
        General,
        Video,
        Audio,
        Text,
        Chapters,
        Image
    }

    public enum eInfoKind {
        Name,
        Text,
        Measure,
        Options,
        NameText,
        MeasureText,
        Info,
        HowTo
    }

    public enum eInfoOptions {
        ShowInInform,
        Support,
        ShowInSupported,
        TypeOfValue
    }

    public enum eInfoFileOptions {
        FileOption_Nothing = 0x00,
        FileOption_Recursive = 0x01,
        FileOption_CloseAll = 0x02,
        FileOption_Max = 0x04
    };

}
