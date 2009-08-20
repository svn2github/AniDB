using System;
using System.Collections.Generic;
using System.Text;
using System.Security.Cryptography;
using System.IO;
using System.Net.Sockets;
using System.Net;
using System.IO.Compression;
using System.Diagnostics;
using System.Xml;

namespace AVDump2Lib.Dump {
    public sealed class ACreq {
        private XmlDocument xmlDoc;
        private UdpClient com;
        private IPEndPoint e;
        private string uPassword;
        private string uName;
        private string cVersion;
        private string cName;
        private string cKey = "communicating";
        private byte rndByte;
        private object tag;

        private ACreq(int cPort) {
            e = new IPEndPoint(IPAddress.Any, cPort);
            com = new UdpClient(e);
        }

        public enum eStatus {
            UnkownError,
            CryptographicError,
            WrongPassOrName,
            ACreqSent
        }

        public delegate void SuccessEvent(eStatus status, object tag, object reason);
        public static event SuccessEvent Success;
        public static void DoACreq(string cName, string cVersion, int cPort, string uName, string uPassword, XmlDocument xmlDoc, object tag) {
            ACreq aCreq = new ACreq(cPort);
            aCreq.cVersion = cVersion;
            aCreq.cName = cName;
            aCreq.tag = tag;
            aCreq.uName = uName;
            aCreq.uPassword = uPassword;
            aCreq.xmlDoc = xmlDoc;

            aCreq.DoACreq();
        }


        private void DoACreq() {
            rndByte = (byte)(new Random().Next(256));
			Debug.Print("rndByte: " + rndByte);

			Debug.Print("Listen");
            com.BeginReceive(new AsyncCallback(ServerReply), null);

			Debug.Print("EstablishSession");
            EstablishSession();
        }

        private void EstablishSession() {
            try {
                string plainText = uName + "|" + cName + "|" + cVersion + "|" + cKey;
				Debug.Print("Loginstring: " + plainText);

				Debug.Print("Get bytearray and encrypt");
                byte[] b = System.Text.Encoding.UTF8.GetBytes(plainText);
				PrintArray(b, "Byte array:");
                b = AESEncrypt(b, GenIVKey(), PaddingMode.PKCS7);
				PrintArray(b, "Encrypted Byte array:");

				Debug.Print("Create Msg");
                byte[] msg = CreateMsg(b, 2, rndByte);

				Debug.Print("Send msg");
                com.Send(msg, msg.Length, "api.anidb.info", 9000);
                //com.Send(msg, msg.Length, "ommina.homeip.net", 9002);
            } catch(Exception e) {
                Success(eStatus.UnkownError, tag, e);
                Debug.Print(e.StackTrace);
            }
        }

        private void ServerReply(IAsyncResult cb) {
            byte[] reply;
            try {
                reply = com.EndReceive(cb, ref e);
				Debug.Print("Servermsg recieved");
				PrintArray(reply, "reply:");
			} catch(Exception e) {
                Success(eStatus.UnkownError, tag, e);
                Debug.Print(e.StackTrace);
                return;
            }

            byte[] remoteHash;
            byte[] localHash = new byte[16];
            byte[] encryptedRemoteHash = new byte[16];

            byte[] rndKey = new byte[16];
            Buffer.BlockCopy(reply, 0, encryptedRemoteHash, 0, 16);
            Buffer.BlockCopy(reply, 16, rndKey, 0, 16);
			PrintArray(encryptedRemoteHash, "encryptedRemoteHash:");
			PrintArray(rndKey, "rndKey:");

            try {
				Debug.Print("GenIVKey");
                Buffer.BlockCopy(GenIVKey(rndKey, uName, uPassword), 16, localHash, 0, 16);
				PrintArray(localHash, "localHash");

				Debug.Print("Decrypting");
                remoteHash = AESDecrypt(encryptedRemoteHash, GenIVKey(rndKey, uName, uPassword), PaddingMode.None);
				PrintArray(remoteHash, "remoteHash");

                for(int i = 0;i < 16;i++) {
                    if(localHash[i] != remoteHash[i]) {
						Debug.Print("WrongPassOrName");
                        if(Success != null) Success(eStatus.WrongPassOrName, tag, e);
                        return;
                    }
                }

            } catch(CryptographicException e) {
                if(Success != null) Success(eStatus.CryptographicError, tag, e);
                Debug.Print(e.StackTrace);
                return;
            }

			Debug.Print("SendCreq");
            SendCreq(rndKey);
        }
        private void SendCreq(byte[] rndKey) {
			Debug.Print("Convert Creq to bytearray");
            byte[] b = System.Text.Encoding.UTF8.GetBytes(xmlDoc.OuterXml);

			Debug.Print("Compress bytearray");
            using(MemoryStream memStream = new MemoryStream()) {
                using(Ionic.Zlib.ZlibStream deflater = new Ionic.Zlib.ZlibStream(memStream, Ionic.Zlib.CompressionMode.Compress, Ionic.Zlib.CompressionLevel.BEST_COMPRESSION, true)) {
                    deflater.Write(b, 0, b.Length);
                    deflater.Flush();
                    /*b = new byte[memStream.Length + 2];
                    b[0] = 0x78; b[1] = 0x9C;
                    Buffer.BlockCopy(memStream.ToArray(), 0, b, 2, (int)memStream.Length);*/
                }
                b = memStream.ToArray();
            }

			Debug.Print("Encrypt compressed bytearray");
            b = AESEncrypt(b, GenIVKey(rndKey, uName, uPassword), PaddingMode.Zeros);
            b = CreateMsg(b, 3, rndByte);

			com.Send(b, b.Length, "api.anidb.info", 9000);
            //com.Send(b, b.Length, "ommina.homeip.net", 9002);
			Debug.Print("Creq Sent");
        }


        private static byte[] CreateMsg(byte[] b, byte packageType, byte rnd) {
            byte[] msg = new byte[b.Length + 2];
            msg[0] = packageType;
            msg[1] = rnd;
            Buffer.BlockCopy(b, 0, msg, 2, (int)b.Length);

            return msg;
        }

        private static byte[] GenIVKey() {
            byte[] key = new byte[32];
            int r = 134;
            for(int i = 0;i < 16;i++) {
                r = (r * 255 + 1) % 99;
                key[i + 16] = (byte)r; //key
                key[i] = key[31 - i]; //iv
            }

            return key;
        }
        private static byte[] GenIVKey(byte[] rndKey, string uName, string uPassword) {
            byte[] key = new byte[32];
            Buffer.BlockCopy(System.Text.Encoding.UTF8.GetBytes("6666666666666666"), 0, key, 0, 16);//IV

            byte[] bUName = System.Text.Encoding.UTF8.GetBytes(uName);
            byte[] bUPassword = System.Text.Encoding.UTF8.GetBytes(uPassword);
            byte[] hash = new byte[48 + bUName.Length + bUPassword.Length];


            MD5 md5 = MD5CryptoServiceProvider.Create();
            md5.Initialize();
            md5.TransformBlock(rndKey, 0, rndKey.Length, null, 0);
            md5.TransformBlock(bUPassword, 0, bUPassword.Length, null, 0);
            md5.TransformBlock(rndKey, 0, rndKey.Length, null, 0);
            md5.TransformBlock(bUName, 0, bUName.Length, null, 0);
            md5.TransformFinalBlock(rndKey, 0, rndKey.Length);
            Buffer.BlockCopy(md5.Hash, 0, key, 16, 16); //KEY

            return key;
        }

        static byte[] AESEncrypt(byte[] b, byte[] ik, PaddingMode pm) {

            byte[] IV = new byte[16];
            byte[] Key = new byte[16];
            Buffer.BlockCopy(ik, 0, IV, 0, 16);
            Buffer.BlockCopy(ik, 16, Key, 0, 16);


           // Check arguments.
            if(b == null) throw new ArgumentNullException("plainText");
            if(Key == null || Key.Length <= 0) throw new ArgumentNullException("Key");
            if(IV == null || IV.Length <= 0) throw new ArgumentNullException("Key");

            // Declare the streams used
            // to encrypt to an in memory
            // array of bytes.
            MemoryStream msEncrypt = null;
            CryptoStream csEncrypt = null;
            //StreamWriter swEncrypt = null;

            // Declare the RijndaelManaged object
            // used to encrypt the data.
            RijndaelManaged aesAlg = null;

            try {
                // Create a RijndaelManaged object
                // with the specified key and IV.
                aesAlg = new RijndaelManaged();
                aesAlg.Key = Key;
                aesAlg.IV = IV;
                aesAlg.Padding = pm;

                // Create a decrytor to perform the stream transform.
                ICryptoTransform encryptor = aesAlg.CreateEncryptor(aesAlg.Key, aesAlg.IV);

                // Create the streams used for encryption.
                msEncrypt = new MemoryStream();
                csEncrypt = new CryptoStream(msEncrypt, encryptor, CryptoStreamMode.Write);
                //swEncrypt = new StreamWriter(csEncrypt);

                //Write all data to the stream.
                csEncrypt.Write(b, 0, b.Length);
                //swEncrypt.Write(plainText);

            } finally {
                // Clean things up.

                // Close the streams.
                //if(swEncrypt != null) swEncrypt.Close();
                if(csEncrypt != null) csEncrypt.Close();
                if(msEncrypt != null) msEncrypt.Close();

                // Clear the RijndaelManaged object.
                if(aesAlg != null) aesAlg.Clear();
            }

            // Return the encrypted bytes from the memory stream.
            return msEncrypt.ToArray();

        }
        static byte[] AESDecrypt(byte[] cipherText, byte[] ik, PaddingMode pm) {

            byte[] IV = new byte[16];
            byte[] Key = new byte[16];
            Buffer.BlockCopy(ik, 0, IV, 0, 16);
            Buffer.BlockCopy(ik, 16, Key, 0, 16);

            // Check arguments.
            if(cipherText == null || cipherText.Length <= 0) throw new ArgumentNullException("cipherText");
            if(Key == null || Key.Length <= 0) throw new ArgumentNullException("Key");
            if(IV == null || IV.Length <= 0) throw new ArgumentNullException("Key");

            // TDeclare the streams used
            // to decrypt to an in memory
            // array of bytes.
            MemoryStream msDecrypt = null;
            CryptoStream csDecrypt = null;

            // Declare the RijndaelManaged object
            // used to decrypt the data.
            RijndaelManaged aesAlg = null;

            // Declare the string used to hold
            // the decrypted bytes.
            byte[] decryptedBytes = null;

            try {
                // Create a RijndaelManaged object
                // with the specified key and IV.
                aesAlg = new RijndaelManaged();
                aesAlg.Key = Key;
                aesAlg.IV = IV;
                aesAlg.Padding = pm;

                // Create a decrytor to perform the stream transform.
                ICryptoTransform decryptor = aesAlg.CreateDecryptor(aesAlg.Key, aesAlg.IV);

                // Create the streams used for decryption.
                msDecrypt = new MemoryStream(cipherText);
                csDecrypt = new CryptoStream(msDecrypt, decryptor, CryptoStreamMode.Read);

                // Read the decrypted bytes from the decrypting stream
                // and place them in a bytearray.
                decryptedBytes = new byte[cipherText.Length];
                int intRead = csDecrypt.Read(decryptedBytes, 0, decryptedBytes.Length);
                Array.Resize<byte>(ref decryptedBytes, intRead);

            } catch(CryptographicException e) {
                csDecrypt = null;
                msDecrypt = null;
                aesAlg = null;
                throw;

            } finally {
                // Clean things up.
                
                // Close the streams.
                if(csDecrypt != null) csDecrypt.Close();
                if(msDecrypt != null) msDecrypt.Close();

                // Clear the RijndaelManaged object.
                if(aesAlg != null) aesAlg.Clear();
            }

            return decryptedBytes;

        }

        static void PrintArray(byte[] b, string msg) {
            PrintArray(b, 0, b.Length, msg);
        }
        static void PrintArray(byte[] b, int offset, int count, string msg) {
            Debug.Write(msg + " " + b[offset]);
            for(int i = offset + 1;i < offset+count;i++) Debug.Write(", " + b[i]);
            Debug.Print("");
        }
    }
}