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
using AVDump2Lib.Information.Parser.MediaInfoLib;
using AVDump2Lib.Hashing.Algorithms;
using System.IO;
using AVDump2Lib.Hashing.Tools;

namespace AVDump2Lib.Information {
    public class Info {
        public static string CreateInfoLog(string filePath, eLogType logType, params object[] args) {
            string result = null;
            MediaInfo mi = new MediaInfo();
            mi.Option("Internet", "No");
            mi.Option("Info_Version", "0.7.2.1;AVDump2;1");

            if(!Path.IsPathRooted(filePath)) filePath = Path.Combine(Environment.CurrentDirectory, filePath);

            mi.Open(filePath);
            switch(logType) {
                case eLogType.AVDump:
					StringWriter sw = new StringWriter();
                    CreateAVDumpLog(mi, (List<HashContainer>)args[0], (long)args[1]).Save(sw);
					result = sw.ToString();
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
                    /*AppendLeaf(xmlDoc, node, hash.Name, BaseConverter.ToString(hash.Hash, 16), null);

					if(hash.HashAlgorithm is Ed2k) {
						Ed2k ed2k = (Ed2k)hash.HashAlgorithm;
						if(!ed2k.BlueIsRed()) {
							AppendLeaf(xmlDoc, node, hash.Name, BaseConverter.ToString(ed2k.BlueHash, 16), null);
						}
					}*/
                }
            }

            AppendLeaf(xmlDoc, node, "size", size.ToString(), null);

            node = node.AppendChild(xmlDoc.CreateElement("avmf"));
            AppendLeaf(xmlDoc, node, "duration", mi.Get(eStreamKind.General, 0, "Duration"), null);
            AppendLeaf(xmlDoc, node, "extension", mi.Get(eStreamKind.General, 0, "FileExtension"), null);
            //AppendLeaf(xmlDoc, node, "date", mi.Get(StreamKind.General, 0, ""));
            AppendLeaf(xmlDoc, node, "chapters", mi.Get(eStreamKind.General, 0, "ChaptersCount"), null);
            AppendLeaf(xmlDoc, node, "app", mi.Get(eStreamKind.General, 0, "Encoded_Application"), null);
            AppendLeaf(xmlDoc, node, "lib", mi.Get(eStreamKind.General, 0, "Encoded_Library"), null);

            streamCount = mi.Count_Get(eStreamKind.Video);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("video"));
                AppendLeaf(xmlDoc, subNode, "size", mi.Get(eStreamKind.Video, i, "StreamSize"), null);
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(eStreamKind.Video, i, "Title"), null);
                AppendLeaf(xmlDoc, subNode, "duration", mi.Get(eStreamKind.Video, i, "Duration"), null);
                //AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Video, i, ""));
                AppendLeaf(xmlDoc, subNode, "bitrate", mi.Get(eStreamKind.Video, i, "BitRate"), null);
                AppendLeaf(xmlDoc, subNode, "fourcc", mi.Get(eStreamKind.Video, i, "CodecID"), null); //Codec/CC
                AppendLeaf(xmlDoc, subNode, "fps", mi.Get(eStreamKind.Video, i, "FrameRate"), null);
                AppendLeaf(xmlDoc, subNode, "setting", mi.Get(eStreamKind.Video, i, "Encoded_Library") + " - " + mi.Get(eStreamKind.Video, i, "Encoded_Library_Settings"), null);
                //AppendLeaf(xmlDoc, subNode, "private", mi.Get(StreamKind.Video, i, ""));
                AppendLeaf(xmlDoc, subNode, "encoder", mi.Get(eStreamKind.Video, i, "Encoded_Library"), null);
                AppendLeaf(xmlDoc, subNode, "sample_count", mi.Get(eStreamKind.Video, i, "FrameCount"), null);
                AppendLeaf(xmlDoc, subNode, "chroma", mi.Get(eStreamKind.Video, i, "Colorimetry"), null);
                AppendLeaf(xmlDoc, subNode, "structure", mi.Get(eStreamKind.Video, i, "Interlacement"), null);

                Dictionary<string, string> attrDict = new Dictionary<string, string>();
                attrDict["width"] = mi.Get(eStreamKind.Video, i, "Width");
                attrDict["height"] = mi.Get(eStreamKind.Video, i, "Height");
                AppendLeaf(xmlDoc, subNode, "res_p", null, attrDict);
                //AppendLeaf(xmlDoc, subNode, "res_d", mi.Get(StreamKind.Video, i, ""));
            }

            streamCount = mi.Count_Get(eStreamKind.Audio);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("audio"));
                AppendLeaf(xmlDoc, subNode, "size", mi.Get(eStreamKind.Audio, i, "StreamSize"), null);
                AppendLeaf(xmlDoc, subNode, "duration", mi.Get(eStreamKind.Audio, i, "Duration"), null);
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(eStreamKind.Audio, i, "Title"), null);
                AppendLeaf(xmlDoc, subNode, "language", mi.Get(eStreamKind.Audio, i, "Language"), null);
                //AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Audio, i, ""));
                AppendLeaf(xmlDoc, subNode, "twocc", mi.Get(eStreamKind.Audio, i, "Codec"), null);
                AppendLeaf(xmlDoc, subNode, "channels", mi.Get(eStreamKind.Audio, i, "Channel(s)"), null);
                AppendLeaf(xmlDoc, subNode, "sampling_rate", mi.Get(eStreamKind.Audio, i, "SamplingRate"), null);
                AppendLeaf(xmlDoc, subNode, "mode", mi.Get(eStreamKind.Audio, i, "BitRate_Mode"), null);
                AppendLeaf(xmlDoc, subNode, "sample_count", mi.Get(eStreamKind.Audio, i, "SamplingCount"), null);

            }

            streamCount = mi.Count_Get(eStreamKind.Text);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("subtitles"));
                //AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Text, i, ""));
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(eStreamKind.Text, i, "Title"), null);
                AppendLeaf(xmlDoc, subNode, "language", mi.Get(eStreamKind.Text, i, "Language"), null);
                AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(eStreamKind.Text, i, "Codec"), null);
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
