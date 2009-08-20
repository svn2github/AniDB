// Copyright (C) 2009 DvdKhl
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.using System;

using System;
using System.Collections.Generic;
using System.Xml;
using AVDump2Lib.Hashes;

namespace AVDump2Lib.MediaInfoLib {
    public class Info {
        public static object CreateInfoLog(string filePath, eLogType logType, params object[] args) {
            object result = null;
            MediaInfo mi = new MediaInfo();
            mi.Option("Internet", "No");
            mi.Option("Info_Version", "0.7.0.0;AVDump2;1");

            if(!System.IO.Path.IsPathRooted(filePath)) filePath = System.IO.Path.Combine(Environment.CurrentDirectory, filePath);

            mi.Open(filePath);
            switch(logType) {
                case eLogType.AVDump:
                    result = CreateAVDumpLog(mi, (List<HashContainer>)args[0], (long)args[1]);
                    break;
                case eLogType.MediaInfo:
                    result = CreateMediaInfoDump(mi);
                    break;
            }
            mi.Close();

            return result;
        }

        static XmlDocument CreateAVDumpLog(MediaInfo mi, List<HashContainer> hashes, long size) {
            XmlDocument xmlDoc = new XmlDocument();
            XmlNode node, subNode;
            int streamCount;

            node = xmlDoc.AppendChild(xmlDoc.CreateElement("file"));
            if(hashes != null) {
                foreach(HashContainer hash in hashes) {
                    AppendLeaf(xmlDoc, node, hash.Name, BaseConverter.ToString(hash.Hash, 16), null);
                    if(hash.AltHash != null) AppendLeaf(xmlDoc, node, hash.Name, BaseConverter.ToString(hash.AltHash, 16), null);//Handle Ed2k screwup
                }
            }

            AppendLeaf(xmlDoc, node, "size", size.ToString(), null);

            node = node.AppendChild(xmlDoc.CreateElement("avmf"));
            AppendLeaf(xmlDoc, node, "duration", mi.Get(StreamKind.General, 0, "Duration"), null);
            AppendLeaf(xmlDoc, node, "extension", mi.Get(StreamKind.General, 0, "FileExtension"), null);
            //AppendLeaf(xmlDoc, node, "date", mi.Get(StreamKind.General, 0, ""));
            AppendLeaf(xmlDoc, node, "chapters", mi.Get(StreamKind.General, 0, "ChaptersCount"), null);
            AppendLeaf(xmlDoc, node, "app", mi.Get(StreamKind.General, 0, "Encoded_Application"), null);
            AppendLeaf(xmlDoc, node, "lib", mi.Get(StreamKind.General, 0, "Encoded_Library"), null);

            streamCount = mi.Count_Get(StreamKind.Video);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("video"));
                AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Video, i, "StreamSize"), null);
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(StreamKind.Video, i, "Title"), null);
                AppendLeaf(xmlDoc, subNode, "duration", mi.Get(StreamKind.Video, i, "Duration"), null);
                //AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Video, i, ""));
                AppendLeaf(xmlDoc, subNode, "bitrate", mi.Get(StreamKind.Video, i, "BitRate"), null);
                AppendLeaf(xmlDoc, subNode, "fourcc", mi.Get(StreamKind.Video, i, "CodecID"), null); //Codec/CC
                AppendLeaf(xmlDoc, subNode, "fps", mi.Get(StreamKind.Video, i, "FrameRate"), null);
                AppendLeaf(xmlDoc, subNode, "setting", mi.Get(StreamKind.Video, i, "Encoded_Library") + " - " + mi.Get(StreamKind.Video, i, "Encoded_Library_Settings"), null);
                //AppendLeaf(xmlDoc, subNode, "private", mi.Get(StreamKind.Video, i, ""));
                AppendLeaf(xmlDoc, subNode, "encoder", mi.Get(StreamKind.Video, i, "Encoded_Library"), null);
                AppendLeaf(xmlDoc, subNode, "sample_count", mi.Get(StreamKind.Video, i, "FrameCount"), null);
                AppendLeaf(xmlDoc, subNode, "chroma", mi.Get(StreamKind.Video, i, "Colorimetry"), null);
                AppendLeaf(xmlDoc, subNode, "structure", mi.Get(StreamKind.Video, i, "Interlacement"), null);

                Dictionary<string, string> attrDict = new Dictionary<string, string>();
                attrDict["width"] = mi.Get(StreamKind.Video, i, "Width");
                attrDict["height"] = mi.Get(StreamKind.Video, i, "Height");
                AppendLeaf(xmlDoc, subNode, "res_p", null, attrDict);
                //AppendLeaf(xmlDoc, subNode, "res_d", mi.Get(StreamKind.Video, i, ""));
            }

            streamCount = mi.Count_Get(StreamKind.Audio);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("audio"));
                AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Audio, i, "StreamSize"), null);
                AppendLeaf(xmlDoc, subNode, "duration", mi.Get(StreamKind.Audio, i, "Duration"), null);
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(StreamKind.Audio, i, "Title"), null);
                AppendLeaf(xmlDoc, subNode, "language", mi.Get(StreamKind.Audio, i, "Language"), null);
                //AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Audio, i, ""));
                AppendLeaf(xmlDoc, subNode, "twocc", mi.Get(StreamKind.Audio, i, "Codec"), null);
                AppendLeaf(xmlDoc, subNode, "channels", mi.Get(StreamKind.Audio, i, "Channel(s)"), null);
                AppendLeaf(xmlDoc, subNode, "sampling_rate", mi.Get(StreamKind.Audio, i, "SamplingRate"), null);
                AppendLeaf(xmlDoc, subNode, "mode", mi.Get(StreamKind.Audio, i, "BitRate_Mode"), null);
                AppendLeaf(xmlDoc, subNode, "sample_count", mi.Get(StreamKind.Audio, i, "SamplingCount"), null);

            }

            streamCount = mi.Count_Get(StreamKind.Text);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("subtitles"));
                //AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Text, i, ""));
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(StreamKind.Text, i, "Title"), null);
                AppendLeaf(xmlDoc, subNode, "language", mi.Get(StreamKind.Text, i, "Language"), null);
                AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Text, i, "Codec"), null);
            }

            return xmlDoc;
        }

        static string CreateMediaInfoDump(MediaInfo mi) {
            mi.Option("Complete", "1");
            return mi.Inform();
        }

        static void AppendLeaf(XmlDocument xmlDoc, XmlNode node, string name, string value, Dictionary<string, string> attributes) {
            if((attributes == null || attributes.Count == 0) && string.IsNullOrEmpty(value)) return;

            XmlNode leaf = xmlDoc.CreateElement(name);
            if(!string.IsNullOrEmpty(value)) leaf.AppendChild(xmlDoc.CreateTextNode(value));

            if(attributes != null && attributes.Count == 0) {
                foreach(KeyValuePair<string, string> kvp in attributes) {
                    leaf.Attributes.Append(xmlDoc.CreateAttribute(kvp.Key)).Value = kvp.Value;
                }
            }

            node.AppendChild(leaf);
        }
    }

    [FlagsAttribute]
    public enum eLogType {
        AVDump = 1<<0,
        MediaInfo = 1<<1
    }
}
