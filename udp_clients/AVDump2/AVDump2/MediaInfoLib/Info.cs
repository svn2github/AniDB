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
using System.Text;
using AVDump2Lib.MediaInfoLib;
using System.Reflection;
using System.IO;
using System.Xml;

namespace AVDump2Lib.MediaInfoLib {
    public class Info {
        public static object CreateInfoLog(string filePath, eLogType logType, params object[] args) {
            object result = null;
            MediaInfo mi = new MediaInfo();
            mi.Option("Internet", "No");
            mi.Option("Info_Version", "0.7.0.0;AVDump2;0.1");

            if(!System.IO.Path.IsPathRooted(filePath)) filePath = System.IO.Path.Combine(Environment.CurrentDirectory, filePath);

            mi.Open(filePath);
            switch(logType) {
                case eLogType.AVDump:
                    result = CreateAVDumpLog(mi, (Dictionary<string, string>)args[0], (long)args[1]);
                    break;
            }
            mi.Close();

            return result;
        }

        static XmlDocument CreateAVDumpLog(MediaInfo mi, Dictionary<string, string> hashes, long size) {
            XmlDocument xmlDoc = new XmlDocument();
            XmlNode node, subNode;
            int streamCount;

            node = xmlDoc.AppendChild(xmlDoc.CreateElement("file"));
            if(hashes != null) foreach(KeyValuePair<string, string> kvp in hashes) {
                AppendLeaf(xmlDoc, node, kvp.Key, kvp.Value);
            }
            AppendLeaf(xmlDoc, node, "size", size.ToString());

            node = node.AppendChild(xmlDoc.CreateElement("avmf"));
            AppendLeaf(xmlDoc, node, "duration", mi.Get(StreamKind.General, 0, "Duration"));
            AppendLeaf(xmlDoc, node, "extension", mi.Get(StreamKind.General, 0, "FileExtension"));
            //AppendLeaf(xmlDoc, node, "date", mi.Get(StreamKind.General, 0, ""));
            AppendLeaf(xmlDoc, node, "chapters", mi.Get(StreamKind.General, 0, "ChaptersCount"));
            AppendLeaf(xmlDoc, node, "app", mi.Get(StreamKind.General, 0, "Encoded_Application"));
            AppendLeaf(xmlDoc, node, "lib", mi.Get(StreamKind.General, 0, "Encoded_Library"));

            streamCount = mi.Count_Get(StreamKind.Video);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("video"));
                AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Video, i, "StreamSize"));
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(StreamKind.Video, i, "Title"));
                AppendLeaf(xmlDoc, subNode, "duration", mi.Get(StreamKind.Video, i, "Duration"));
                //AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Video, i, ""));
                AppendLeaf(xmlDoc, subNode, "bitrate", mi.Get(StreamKind.Video, i, "BitRate"));
                AppendLeaf(xmlDoc, subNode, "fourcc", mi.Get(StreamKind.Video, i, "CodecID")); //Codec/CC
                AppendLeaf(xmlDoc, subNode, "fps", mi.Get(StreamKind.Video, i, "FrameRate"));
                AppendLeaf(xmlDoc, subNode, "setting", mi.Get(StreamKind.Video, i, "Encoded_Library") + " - " + mi.Get(StreamKind.Video, i, "Encoded_Library_Settings"));
                //AppendLeaf(xmlDoc, subNode, "private", mi.Get(StreamKind.Video, i, ""));
                AppendLeaf(xmlDoc, subNode, "encoder", mi.Get(StreamKind.Video, i, "Encoded_Library"));
                AppendLeaf(xmlDoc, subNode, "sample_count", mi.Get(StreamKind.Video, i, "FrameCount"));
                AppendLeaf(xmlDoc, subNode, "chroma", mi.Get(StreamKind.Video, i, "Colorimetry"));
                AppendLeaf(xmlDoc, subNode, "structure", mi.Get(StreamKind.Video, i, "Interlacement"));

                Dictionary<string, string> attrDict = new Dictionary<string, string>();
                attrDict["width"] = mi.Get(StreamKind.Video, i, "Width");
                attrDict["height"] = mi.Get(StreamKind.Video, i, "Height");
                AppendLeaf(xmlDoc, subNode, "res_p", attrDict);
                //AppendLeaf(xmlDoc, subNode, "res_d", mi.Get(StreamKind.Video, i, ""));
            }

            streamCount = mi.Count_Get(StreamKind.Audio);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("audio"));
                AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Audio, i, "StreamSize"));
                AppendLeaf(xmlDoc, subNode, "duration", mi.Get(StreamKind.Audio, i, "Duration"));
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(StreamKind.Audio, i, "Title"));
                AppendLeaf(xmlDoc, subNode, "language", mi.Get(StreamKind.Audio, i, "Language"));
                //AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Audio, i, ""));
                AppendLeaf(xmlDoc, subNode, "twocc", mi.Get(StreamKind.Audio, i, "Codec"));
                AppendLeaf(xmlDoc, subNode, "channels", mi.Get(StreamKind.Audio, i, "Channel(s)"));
                AppendLeaf(xmlDoc, subNode, "sampling_rate", mi.Get(StreamKind.Audio, i, "SamplingRate"));
                AppendLeaf(xmlDoc, subNode, "mode", mi.Get(StreamKind.Audio, i, "BitRate_Mode"));
                AppendLeaf(xmlDoc, subNode, "sample_count", mi.Get(StreamKind.Audio, i, "SamplingCount"));

            }

            streamCount = mi.Count_Get(StreamKind.Text);
            for(int i = 0;i < streamCount;i++) {
                subNode = node.AppendChild(xmlDoc.CreateElement("subtitles"));
                //AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Text, i, ""));
                AppendLeaf(xmlDoc, subNode, "title", mi.Get(StreamKind.Text, i, "Title"));
                AppendLeaf(xmlDoc, subNode, "language", mi.Get(StreamKind.Text, i, "Language"));
                AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Text, i, "Codec"));
            }

            return xmlDoc;
        }

        static void AppendLeaf(XmlDocument xmlDoc, XmlNode node, string name, Dictionary<string, string> attributes) {
            if(attributes == null || attributes.Count == 0) return;

            XmlNode leaf = xmlDoc.CreateElement(name);
            foreach(KeyValuePair<string, string> kvp in attributes) {
                leaf.Attributes.Append(xmlDoc.CreateAttribute(kvp.Key)).Value = kvp.Value;
            }
            node.AppendChild(leaf);

        }
        static void AppendLeaf(XmlDocument xmlDoc, XmlNode node, string name, string value) {
            if(string.IsNullOrEmpty(value)) return;

            XmlNode leaf = xmlDoc.CreateElement(name);
            leaf.AppendChild(xmlDoc.CreateTextNode(value));
            node.AppendChild(leaf);
        }
    }

    public enum eLogType {
        AVDump
    }
}
