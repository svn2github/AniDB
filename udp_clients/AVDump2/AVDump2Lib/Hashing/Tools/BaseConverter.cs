using System;
using System.Collections.Generic;
using System.Text;

namespace AVDump2Lib.Hashes {
    public class BaseConverter {
        private static char[] map = new char[] {
            '0','1','2','3','4','5','6','7','8','9',
            'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'
        };

        public static byte[] ByteMultiply(byte[] b, int x) {
            int carry = 0;
            int result;

            for(int i = 0;i < b.Length;i++) {
                result = (b[i] + carry) * x;
                carry = result / 0xFF;
                b[i] = (byte)result;
            }
            if(carry != 0) throw new Exception();

            return b;
        }
        //public static byte[] ByteMultiply(byte[] b1, byte[] b2) { return null; }

        public static byte[] ByteDivision(byte[] b, int x) { return null; }
        //public static byte[] ByteDivision(byte[] b1, byte[] b2) { return null; }

        public static byte[] ByteAdd(byte[] b, int x) { return null; }
        //public static byte[] ByteAdd(byte[] b1, byte[] b2) { return null; }

        //public static byte[] ByteSubstract(byte[] b, int x) { return null; }
        public static byte[] ByteSubstract(byte[] b1, byte[] b2) { return null; }

        public static bool ByteEqual(byte[] b, int x) { return false; }
        //public static bool ByteEqual(byte[] b1, byte[] b2) { return false; }

        //public static bool ByteLessThan(byte[] b, int x) { return false; }
        //public static bool ByteLessThan(byte[] b1, byte[] b2) { return false; }

        //public static bool ByteGreaterThan(byte[] b, int x) { return false; }
        public static bool ByteGreaterThan(byte[] b1, byte[] b2) { return false; }

        public static string ToString(byte[] b, int baseNumber) {
            if(baseNumber == 16) {
                string hashStr = "";
                for(int i = 0;i < b.Length;i++) {
                    hashStr += b[i].ToString("X2");
                }
                return hashStr;
            }

            byte[] baseDigit = new byte[0];
            string baseStr = "";

            baseDigit = ByteAdd(baseDigit, 1);
            while(true) {
                baseDigit = ByteMultiply(baseDigit, baseNumber);
                if(ByteGreaterThan(baseDigit, b)) break;
            }
            baseDigit = ByteDivision(baseDigit, baseNumber);

            int count;

            while(!ByteEqual(baseDigit, 1)){
                count = 0;
                while(ByteGreaterThan(b, baseDigit)) {
                    b = ByteSubstract(b, baseDigit);
                    count++;
                }
                baseStr += map[count];
                baseDigit = ByteDivision(baseDigit, baseNumber);
            }

            count = 0;
            while(ByteGreaterThan(b, baseDigit)) {
                b = ByteSubstract(b, baseDigit);
                count++;
            }
            baseStr += map[count];


            return baseStr;
        }

    }
}
