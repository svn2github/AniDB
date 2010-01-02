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
using System.Security.Cryptography;
using System.Globalization;

namespace AVDump2Lib.Information {

	public class Info {
		public static XmlDocument CreateAVDumpLog(string path, List<HashExecute> hashExecutes) {
			XmlDocument xmlDoc = new XmlDocument();
			XmlNode node, subNode;
			int streamCount;


			node = xmlDoc.AppendChild(xmlDoc.CreateElement("file"));
			AppendLeaf(xmlDoc, node, "size", (new FileInfo(path)).Length.ToString(), null);

			if(hashExecutes != null) {
				foreach(HashExecute hashExecute in hashExecutes) {
					AppendLeaf(xmlDoc, node, hashExecute.Name, BaseConverter.ToString(hashExecute.HashObj.Hash, 16), null);

					if(hashExecute.HashObj is Ed2k) {
						Ed2k ed2k = (Ed2k)hashExecute.HashObj;
						if(!ed2k.BlueIsRed()) AppendLeaf(xmlDoc, node, hashExecute.Name, BaseConverter.ToString(ed2k.BlueHash, 16), new String[,] { { "Type", "Blue" } });
					}
				}
			}

			MediaInfo mediaInfo = CreateMediaInfoInstance();
			mediaInfo.Open(path);

			node = node.AppendChild(xmlDoc.CreateElement("avmf"));
			AppendLeaf(xmlDoc, node, "duration", (long.Parse(mediaInfo.Get(eStreamKind.General, 0, "Duration")) / 1000d).ToString(CultureInfo.InvariantCulture), null);
			AppendLeaf(xmlDoc, node, "extension", mediaInfo.Get(eStreamKind.General, 0, "FileExtension"), null);
			//AppendLeaf(xmlDoc, node, "date", mi.Get(StreamKind.General, 0, ""));
			AppendLeaf(xmlDoc, node, "chapters", mediaInfo.Get(eStreamKind.General, 0, "ChaptersCount"), null);
			AppendLeaf(xmlDoc, node, "app", mediaInfo.Get(eStreamKind.General, 0, "Encoded_Application"), null);
			AppendLeaf(xmlDoc, node, "lib", mediaInfo.Get(eStreamKind.General, 0, "Encoded_Library"), null);

			streamCount = mediaInfo.Count_Get(eStreamKind.Video);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("video"));
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamKind.Video, i, "Title"), null);
				AppendLeaf(xmlDoc, subNode, "size", mediaInfo.Get(eStreamKind.Video, i, "StreamSize"), null);
				AppendLeaf(xmlDoc, subNode, "duration", (long.Parse(mediaInfo.Get(eStreamKind.Video, i, "Duration")) / 1000d).ToString(CultureInfo.InvariantCulture), null);
				//AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Video, i, ""));
				AppendLeaf(xmlDoc, subNode, "bitrate", mediaInfo.Get(eStreamKind.Video, i, "BitRate"), null);
				AppendLeaf(xmlDoc, subNode, "fourcc", mediaInfo.Get(eStreamKind.Video, i, "CodecID"), null); //Codec/CC
				AppendLeaf(xmlDoc, subNode, "fps", mediaInfo.Get(eStreamKind.Video, i, "FrameRate"), null);
				AppendLeaf(xmlDoc, subNode, "encoder", mediaInfo.Get(eStreamKind.Video, i, "Encoded_Library"), null);
				AppendLeaf(xmlDoc, subNode, "setting", mediaInfo.Get(eStreamKind.Video, i, "Encoded_Library_Settings"), null);
				//AppendLeaf(xmlDoc, subNode, "private", mi.Get(StreamKind.Video, i, ""));
				AppendLeaf(xmlDoc, subNode, "sample_count", mediaInfo.Get(eStreamKind.Video, i, "FrameCount"), null);
				AppendLeaf(xmlDoc, subNode, "chroma", mediaInfo.Get(eStreamKind.Video, i, "Colorimetry"), null);
				AppendLeaf(xmlDoc, subNode, "structure", mediaInfo.Get(eStreamKind.Video, i, "Interlacement"), null);

				AppendLeaf(xmlDoc, subNode, "res_p", null, new String[,] { { "Width", mediaInfo.Get(eStreamKind.Video, i, "Width") },
				                                                           { "Height", mediaInfo.Get(eStreamKind.Video, i, "Height") }});

				string res_d = mediaInfo.Get(eStreamKind.Video, i, "DisplayAspectRatio/String");
				if(res_d.Contains(":")) {
					string x, y;
					x = res_d.Split(':')[0];
					y = res_d.Split(':')[1];

					AppendLeaf(xmlDoc, subNode, "res_d", null, new String[,] { { "Width", x.ToString() },
				                                                               { "Height", y.ToString() }});
				}
			}

			streamCount = mediaInfo.Count_Get(eStreamKind.Audio);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("audio"));
				AppendLeaf(xmlDoc, subNode, "size", mediaInfo.Get(eStreamKind.Audio, i, "StreamSize"), null);
				AppendLeaf(xmlDoc, subNode, "duration", (long.Parse(mediaInfo.Get(eStreamKind.Audio, i, "Duration")) / 1000d).ToString(CultureInfo.InvariantCulture), null);
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamKind.Audio, i, "Title"), null);
				AppendLeaf(xmlDoc, subNode, "bitrate", mediaInfo.Get(eStreamKind.Audio, i, "BitRate"), null);
				AppendLeaf(xmlDoc, subNode, "language", mediaInfo.Get(eStreamKind.Audio, i, "Language"), null);
				//AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Audio, i, ""));
				AppendLeaf(xmlDoc, subNode, "twocc", mediaInfo.Get(eStreamKind.Audio, i, "Codec"), null);
				AppendLeaf(xmlDoc, subNode, "channels", mediaInfo.Get(eStreamKind.Audio, i, "Channel(s)"), null);
				AppendLeaf(xmlDoc, subNode, "sampling_rate", mediaInfo.Get(eStreamKind.Audio, i, "SamplingRate"), null);
				AppendLeaf(xmlDoc, subNode, "mode", mediaInfo.Get(eStreamKind.Audio, i, "BitRate_Mode"), null);
				AppendLeaf(xmlDoc, subNode, "sample_count", mediaInfo.Get(eStreamKind.Audio, i, "SamplingCount"), null);

			}

			streamCount = mediaInfo.Count_Get(eStreamKind.Text);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("subtitles"));
				//AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Text, i, ""));
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamKind.Text, i, "Title"), null);
				AppendLeaf(xmlDoc, subNode, "language", mediaInfo.Get(eStreamKind.Text, i, "Language"), null);
				AppendLeaf(xmlDoc, subNode, "identifier", mediaInfo.Get(eStreamKind.Text, i, "Codec"), null);
			}
			mediaInfo.Close();

			return xmlDoc;
		}
		public static string CreateMediaInfoDump(string filePath) {
			MediaInfo mi = CreateMediaInfoInstance();

			mi.Open(filePath);
			mi.Option("Complete", "1");

			string dump = mi.Inform();

			mi.Close();

			return dump;
		}

		private static void AppendLeaf(XmlDocument xmlDoc, XmlNode node, string name, string value, string[,] attributes) {
			if((attributes == null || attributes.Length == 0) && string.IsNullOrEmpty(value)) return;

			XmlNode leaf = xmlDoc.CreateElement(name);
			if(!string.IsNullOrEmpty(value)) leaf.AppendChild(xmlDoc.CreateTextNode(value));

			if(attributes != null) {
				for(int i = 0;i < attributes.GetLength(0);i++) {
					leaf.Attributes.Append(xmlDoc.CreateAttribute(attributes[i, 0])).Value = attributes[i, 1];
				}
			}

			node.AppendChild(leaf);
		}
		private static MediaInfo CreateMediaInfoInstance() {
			MediaInfo mi = new MediaInfo();
			mi.Option("Internet", "No");
			mi.Option("Info_Version", "0.7.2.1;AVDump2;1");
			return mi;
		}
	}
}
