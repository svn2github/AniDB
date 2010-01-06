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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

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
using System.Diagnostics;

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
					AppendLeaf(xmlDoc, node, hashExecute.Name.ToLower(), BaseConverter.ToString(hashExecute.HashObj.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse), null);

					if(hashExecute.HashObj is Ed2k) {
						Ed2k ed2k = (Ed2k)hashExecute.HashObj;
						if(!ed2k.BlueIsRed()) AppendLeaf(xmlDoc, node, hashExecute.Name.ToLower() + "_alt", BaseConverter.ToString(ed2k.BlueHash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse), null);
					}
				}
			}

			MediaInfo mediaInfo = CreateMediaInfoInstance();
			mediaInfo.Open(path);

			node = node.AppendChild(xmlDoc.CreateElement("avmf"));
			AppendLeaf(xmlDoc, node, "duration", (long.Parse(mediaInfo.Get(eStreamType.General, 0, "Duration")) / 1000d).ToString(CultureInfo.InvariantCulture), null);
			AppendLeaf(xmlDoc, node, "extension", mediaInfo.Get(eStreamType.General, 0, "FileExtension"), null);
			//AppendLeaf(xmlDoc, node, "date", mi.Get(StreamKind.General, 0, ""));
			AppendLeaf(xmlDoc, node, "chapters", mediaInfo.Get(eStreamType.General, 0, "ChaptersCount"), null);
			AppendLeaf(xmlDoc, node, "app", mediaInfo.Get(eStreamType.General, 0, "Encoded_Application"), null);
			AppendLeaf(xmlDoc, node, "lib", mediaInfo.Get(eStreamType.General, 0, "Encoded_Library"), null);

			streamCount = mediaInfo.Count_Get(eStreamType.Video);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("video"));
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamType.Video, i, "Title"), null);
				AppendLeaf(xmlDoc, subNode, "size", mediaInfo.Get(eStreamType.Video, i, "StreamSize"), null);
				AppendLeaf(xmlDoc, subNode, "duration", (long.Parse(mediaInfo.Get(eStreamType.Video, i, "Duration")) / 1000d).ToString(CultureInfo.InvariantCulture), null);
				//AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Video, i, ""));
				AppendLeaf(xmlDoc, subNode, "bitrate", mediaInfo.Get(eStreamType.Video, i, "BitRate"), null);
				AppendLeaf(xmlDoc, subNode, "fourcc", mediaInfo.Get(eStreamType.Video, i, "CodecID"), null); //Codec/CC
				AppendLeaf(xmlDoc, subNode, "fps", mediaInfo.Get(eStreamType.Video, i, "FrameRate"), null);
				AppendLeaf(xmlDoc, subNode, "encoder", mediaInfo.Get(eStreamType.Video, i, "Encoded_Library"), null);
				AppendLeaf(xmlDoc, subNode, "setting", mediaInfo.Get(eStreamType.Video, i, "Encoded_Library_Settings"), null);
				//AppendLeaf(xmlDoc, subNode, "private", mi.Get(StreamKind.Video, i, ""));
				AppendLeaf(xmlDoc, subNode, "sample_count", mediaInfo.Get(eStreamType.Video, i, "FrameCount"), null);
				AppendLeaf(xmlDoc, subNode, "chroma", mediaInfo.Get(eStreamType.Video, i, "Colorimetry"), null);
				AppendLeaf(xmlDoc, subNode, "structure", mediaInfo.Get(eStreamType.Video, i, "Interlacement"), null);

				AppendLeaf(xmlDoc, subNode, "res_p", null, new String[,] { { "width", mediaInfo.Get(eStreamType.Video, i, "Width") },
				                                                           { "height", mediaInfo.Get(eStreamType.Video, i, "Height") }});

				//AppendLeaf(xmlDoc, subNode, "dar", mediaInfo.Get(eStreamKind.Video, i, "DisplayAspectRatio"), null);
				string res_d = mediaInfo.Get(eStreamType.Video, i, "DisplayAspectRatio/String");
				if(res_d.Contains(":")) {
					string x, y;
					x = res_d.Split(':')[0];
					y = res_d.Split(':')[1];

					AppendLeaf(xmlDoc, subNode, "dar", null, new String[,] { { "width", x.ToString() },
				                                                               { "height", y.ToString() }});
				}
			}

			streamCount = mediaInfo.Count_Get(eStreamType.Audio);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("audio"));
				AppendLeaf(xmlDoc, subNode, "size", mediaInfo.Get(eStreamType.Audio, i, "StreamSize"), null);
				AppendLeaf(xmlDoc, subNode, "duration", (long.Parse(mediaInfo.Get(eStreamType.Audio, i, "Duration")) / 1000d).ToString(CultureInfo.InvariantCulture), null);
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamType.Audio, i, "Title"), null);
				AppendLeaf(xmlDoc, subNode, "bitrate", mediaInfo.Get(eStreamType.Audio, i, "BitRate"), null);
				AppendLeaf(xmlDoc, subNode, "language", mediaInfo.Get(eStreamType.Audio, i, "Language"), null);
				//AppendLeaf(xmlDoc, subNode, "identifier", mi.Get(StreamKind.Audio, i, ""));
				AppendLeaf(xmlDoc, subNode, "twocc", mediaInfo.Get(eStreamType.Audio, i, "Codec"), null);
				AppendLeaf(xmlDoc, subNode, "channels", mediaInfo.Get(eStreamType.Audio, i, "Channel(s)"), null);
				AppendLeaf(xmlDoc, subNode, "sampling_rate", mediaInfo.Get(eStreamType.Audio, i, "SamplingRate"), null);
				AppendLeaf(xmlDoc, subNode, "mode", mediaInfo.Get(eStreamType.Audio, i, "BitRate_Mode"), null);
				AppendLeaf(xmlDoc, subNode, "sample_count", mediaInfo.Get(eStreamType.Audio, i, "SamplingCount"), null);

			}

			streamCount = mediaInfo.Count_Get(eStreamType.Text);
			for(int i = 0;i < streamCount;i++) {
				subNode = node.AppendChild(xmlDoc.CreateElement("subtitles"));
				//AppendLeaf(xmlDoc, subNode, "size", mi.Get(StreamKind.Text, i, ""));
				AppendLeaf(xmlDoc, subNode, "title", mediaInfo.Get(eStreamType.Text, i, "Title"), null);
				AppendLeaf(xmlDoc, subNode, "language", mediaInfo.Get(eStreamType.Text, i, "Language"), null);
				AppendLeaf(xmlDoc, subNode, "identifier", mediaInfo.Get(eStreamType.Text, i, "Codec"), null);
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
		public static XmlDocument CreateMediaInfoXMLLog(string filePath, List<HashExecute> hashExecutes) {
			XmlDocument xmlDoc = new XmlDocument();
			XmlNode node, subNode;

			MediaInfo mediaInfo = CreateMediaInfoInstance();
			mediaInfo.Open(filePath);

			node = xmlDoc.AppendChild(xmlDoc.CreateElement("File"));
			subNode = node.AppendChild(xmlDoc.CreateElement("Hashes"));
			AppendLeaf(xmlDoc, subNode, "Size", (new FileInfo(filePath)).Length.ToString(), null);
			if(hashExecutes != null) {
				foreach(HashExecute hashExecute in hashExecutes) {
					AppendLeaf(xmlDoc, subNode, hashExecute.Name, BaseConverter.ToString(hashExecute.HashObj.Hash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse), null);

					if(hashExecute.HashObj is Ed2k) {
						Ed2k ed2k = (Ed2k)hashExecute.HashObj;
						if(!ed2k.BlueIsRed()) AppendLeaf(xmlDoc, subNode, "Ed2k_Alt", BaseConverter.ToString(ed2k.BlueHash, eBaseOption.Heximal | eBaseOption.Pad | eBaseOption.Reverse), null);
					}
				}
			}

			int streamCount, entryCount;
			string name, text, measure;
			foreach(eStreamType streamKind in Enum.GetValues(typeof(eStreamType))) {
				streamCount = mediaInfo.Count_Get(streamKind);

				for(int i = 0;i < streamCount;i++) {
					entryCount = mediaInfo.Count_Get(streamKind, i);
					subNode = node.AppendChild(xmlDoc.CreateElement(streamKind.ToString()));

					for(int j = 0;j < entryCount;j++) {
						name = mediaInfo.Get(streamKind, i, j, eInfoType.Name).Replace("/", "-").Replace("(", "").Replace(")", "");
						if(name.Equals("Count") || name.Equals("StreamCount") || name.Equals("StreamKind") || name.Equals("Chapters_Pos_End") || name.Equals("Chapters_Pos_Begin") || name.Contains("-String") || name.Equals("StreamKindID")) continue;
						if(name.Equals("Bits-Pixel*Frame")) name = "BitsPerPixel";

						text = mediaInfo.Get(streamKind, i, j, eInfoType.Text);
						measure = mediaInfo.Get(streamKind, i, j, eInfoType.Measure).Trim();

						if(name.IndexOfAny(new char[] { ')', ':' }) < 0 && !String.IsNullOrEmpty(text)) {
							AppendLeaf(xmlDoc, subNode, name, text, String.IsNullOrEmpty(measure) ? null : new string[,] { { "Unit", measure } });
						} else {
							Debug.Print(name + " " + text + " " + measure);
						}
					}
					if(streamKind == eStreamType.Menu) {
						int indexStart;
						int indexEnd;
						XmlNode chapterNode;

						if(int.TryParse(mediaInfo.Get(streamKind, i, "Chapters_Pos_Begin"), out indexStart) && int.TryParse(mediaInfo.Get(streamKind, i, "Chapters_Pos_End"), out indexEnd)) {
							chapterNode = subNode.AppendChild(xmlDoc.CreateElement("Chapters"));
							for(;indexStart < indexEnd;indexStart++) {
								AppendLeaf(xmlDoc, chapterNode, "Chapter", mediaInfo.Get(streamKind, i, indexStart, eInfoType.Text), new string[,] { { "TimeStamp", mediaInfo.Get(streamKind, i, indexStart, eInfoType.Name) } });
							}
						}
					}
				}
			}

			mediaInfo.Close();

			return xmlDoc;
		}

		private static XmlNode AppendLeaf(XmlDocument xmlDoc, XmlNode node, string name, string value, string[,] attributes) {
			if((attributes == null || attributes.Length == 0) && string.IsNullOrEmpty(value)) return null;

			XmlNode leaf = xmlDoc.CreateElement(name);
			if(!string.IsNullOrEmpty(value)) leaf.AppendChild(xmlDoc.CreateTextNode(value));

			if(attributes != null) {
				for(int i = 0;i < attributes.GetLength(0);i++) {
					leaf.Attributes.Append(xmlDoc.CreateAttribute(attributes[i, 0])).Value = attributes[i, 1];
				}
			}

			return node.AppendChild(leaf);
		}
		private static MediaInfo CreateMediaInfoInstance() {
			MediaInfo mi = new MediaInfo();
			mi.Option("Internet", "No");
			mi.Option("Info_Version", "0.7.2.1;AVDump2;1");
			return mi;
		}
	}
}
