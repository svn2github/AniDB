// MD4Managed.cs - Message Digest 4 Managed Implementation
//
// Author:
//	Sebastien Pouliot (sebastien@ximian.com)
//
// (C) 2003 Motus Technologies Inc. (http://www.motus.com)
// Copyright (C) 2004-2005 Novell, Inc (http://www.novell.com)
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

using System;
using System.Collections.Generic;
using System.Text;
using System.Security.Cryptography;

namespace AVDump2Lib.Hashes {
    /// <summary>SLOW</summary>
    public class Md4_Alt : HashAlgorithm {
        private uint[] state;
        private byte[] buffer;
        private uint[] count;
        private uint[] x;

        private const int S11 = 3;
        private const int S12 = 7;
        private const int S13 = 11;
        private const int S14 = 19;
        private const int S21 = 3;
        private const int S22 = 5;
        private const int S23 = 9;
        private const int S24 = 13;
        private const int S31 = 3;
        private const int S32 = 9;
        private const int S33 = 11;
        private const int S34 = 15;

        private byte[] digest;

        //--- constructor -----------------------------------------------------------

        public Md4_Alt() {
            // we allocate the context memory
            state = new uint[4];
            count = new uint[2];
            buffer = new byte[64];
            digest = new byte[16];
            // temporary buffer in MD4Transform that we don't want to keep allocate on each iteration
            x = new uint[16];
            // the initialize our context
            Initialize();
        }

        public override void Initialize() {
            count[0] = 0;
            count[1] = 0;
            state[0] = 0x67452301;
            state[1] = 0xefcdab89;
            state[2] = 0x98badcfe;
            state[3] = 0x10325476;
            // Zeroize sensitive information
            Array.Clear(buffer, 0, 64);
            Array.Clear(x, 0, 16);
        }

        protected override void HashCore(byte[] array, int ibStart, int cbSize) {
            /* Compute number of bytes mod 64 */
            int index = (int)((count[0] >> 3) & 0x3F);
            /* Update number of bits */
            count[0] += (uint)(cbSize << 3);
            if(count[0] < (cbSize << 3)) count[1]++;
            count[1] += (uint)(cbSize >> 29);

            int partLen = 64 - index;
            int i = 0;
            /* Transform as many times as possible. */
            if(cbSize >= partLen) {
                //MD4_memcpy((POINTER)&context->buffer[index], (POINTER)input, partLen);
                Buffer.BlockCopy(array, ibStart, buffer, index, partLen);
                MD4Transform(state, buffer, 0);

                for(i = partLen;i + 63 < cbSize;i += 64) {
                    // MD4Transform (context->state, &input[i]);
                    MD4Transform(state, array, i);
                }
                index = 0;
            }

            /* Buffer remaining input */
            //MD4_memcpy ((POINTER)&context->buffer[index], (POINTER)&input[i], inputLen-i);
            Buffer.BlockCopy(array, ibStart + i, buffer, index, (cbSize - i));
        }

        protected override byte[] HashFinal() {
            /* Save number of bits */
            byte[] bits = new byte[8];
            Encode(bits, count);

            /* Pad out to 56 mod 64. */
            uint index = ((count[0] >> 3) & 0x3f);
            int padLen = (int)((index < 56) ? (56 - index) : (120 - index));
            HashCore(Padding(padLen), 0, padLen);

            /* Append length (before padding) */
            HashCore(bits, 0, 8);

            /* Store state in digest */
            Encode(digest, state);

            // Zeroize sensitive information.
            Initialize();

            return digest;
        }

        //--- private methods ---------------------------------------------------

        private byte[] Padding(int nLength) {
            if(nLength > 0) {
                byte[] padding = new byte[nLength];
                padding[0] = 0x80;
                return padding;
            }
            return null;
        }

        /* F, G and H are basic MD4 functions. */
        private uint F(uint x, uint y, uint z) {
            return (uint)(((x) & (y)) | ((~x) & (z)));
        }

        private uint G(uint x, uint y, uint z) {
            return (uint)(((x) & (y)) | ((x) & (z)) | ((y) & (z)));
        }

        private uint H(uint x, uint y, uint z) {
            return (uint)((x) ^ (y) ^ (z));
        }

        /* ROTATE_LEFT rotates x left n bits. */
        private uint ROL(uint x, byte n) {
            return (uint)(((x) << (n)) | ((x) >> (32 - (n))));
        }

        /* FF, GG and HH are transformations for rounds 1, 2 and 3 */
        /* Rotation is separate from addition to prevent recomputation */
        private void FF(ref uint a, uint b, uint c, uint d, uint x, byte s) {
            a += F(b, c, d) + x;
            a = ROL(a, s);
        }

        private void GG(ref uint a, uint b, uint c, uint d, uint x, byte s) {
            a += G(b, c, d) + x + 0x5a827999;
            a = ROL(a, s);
        }

        private void HH(ref uint a, uint b, uint c, uint d, uint x, byte s) {
            a += H(b, c, d) + x + 0x6ed9eba1;
            a = ROL(a, s);
        }

        private void Encode(byte[] output, uint[] input) {
            for(int i = 0, j = 0;j < output.Length;i++, j += 4) {
                output[j] = (byte)(input[i]);
                output[j + 1] = (byte)(input[i] >> 8);
                output[j + 2] = (byte)(input[i] >> 16);
                output[j + 3] = (byte)(input[i] >> 24);
            }
        }

        private void Decode(uint[] output, byte[] input, int index) {
            for(int i = 0, j = index;i < output.Length;i++, j += 4) {
                output[i] = (uint)((input[j]) | (input[j + 1] << 8) | (input[j + 2] << 16) | (input[j + 3] << 24));
            }
        }

        private void MD4Transform(uint[] state, byte[] block, int index) {
            uint a = state[0];
            uint b = state[1];
            uint c = state[2];
            uint d = state[3];

            Decode(x, block, index);

            /* Round 1 */
            FF(ref a, b, c, d, x[0], S11); /* 1 */
            FF(ref d, a, b, c, x[1], S12); /* 2 */
            FF(ref c, d, a, b, x[2], S13); /* 3 */
            FF(ref b, c, d, a, x[3], S14); /* 4 */
            FF(ref a, b, c, d, x[4], S11); /* 5 */
            FF(ref d, a, b, c, x[5], S12); /* 6 */
            FF(ref c, d, a, b, x[6], S13); /* 7 */
            FF(ref b, c, d, a, x[7], S14); /* 8 */
            FF(ref a, b, c, d, x[8], S11); /* 9 */
            FF(ref d, a, b, c, x[9], S12); /* 10 */
            FF(ref c, d, a, b, x[10], S13); /* 11 */
            FF(ref b, c, d, a, x[11], S14); /* 12 */
            FF(ref a, b, c, d, x[12], S11); /* 13 */
            FF(ref d, a, b, c, x[13], S12); /* 14 */
            FF(ref c, d, a, b, x[14], S13); /* 15 */
            FF(ref b, c, d, a, x[15], S14); /* 16 */

            /* Round 2 */
            GG(ref a, b, c, d, x[0], S21); /* 17 */
            GG(ref d, a, b, c, x[4], S22); /* 18 */
            GG(ref c, d, a, b, x[8], S23); /* 19 */
            GG(ref b, c, d, a, x[12], S24); /* 20 */
            GG(ref a, b, c, d, x[1], S21); /* 21 */
            GG(ref d, a, b, c, x[5], S22); /* 22 */
            GG(ref c, d, a, b, x[9], S23); /* 23 */
            GG(ref b, c, d, a, x[13], S24); /* 24 */
            GG(ref a, b, c, d, x[2], S21); /* 25 */
            GG(ref d, a, b, c, x[6], S22); /* 26 */
            GG(ref c, d, a, b, x[10], S23); /* 27 */
            GG(ref b, c, d, a, x[14], S24); /* 28 */
            GG(ref a, b, c, d, x[3], S21); /* 29 */
            GG(ref d, a, b, c, x[7], S22); /* 30 */
            GG(ref c, d, a, b, x[11], S23); /* 31 */
            GG(ref b, c, d, a, x[15], S24); /* 32 */

            HH(ref a, b, c, d, x[0], S31); /* 33 */
            HH(ref d, a, b, c, x[8], S32); /* 34 */
            HH(ref c, d, a, b, x[4], S33); /* 35 */
            HH(ref b, c, d, a, x[12], S34); /* 36 */
            HH(ref a, b, c, d, x[2], S31); /* 37 */
            HH(ref d, a, b, c, x[10], S32); /* 38 */
            HH(ref c, d, a, b, x[6], S33); /* 39 */
            HH(ref b, c, d, a, x[14], S34); /* 40 */
            HH(ref a, b, c, d, x[1], S31); /* 41 */
            HH(ref d, a, b, c, x[9], S32); /* 42 */
            HH(ref c, d, a, b, x[5], S33); /* 43 */
            HH(ref b, c, d, a, x[13], S34); /* 44 */
            HH(ref a, b, c, d, x[3], S31); /* 45 */
            HH(ref d, a, b, c, x[11], S32); /* 46 */
            HH(ref c, d, a, b, x[7], S33); /* 47 */
            HH(ref b, c, d, a, x[15], S34); /* 48 */

            state[0] += a;
            state[1] += b;
            state[2] += c;
            state[3] += d;
        }
    }

    public class Md4 : HashAlgorithm {
        public static int HASHLENGTH = 16;
        public static int BLOCKLENGTH = 64;
        private static uint A0, B0, C0, D0;
        private static string HASH0 = "31D6CFE0D16AE931B73C59D7E0C089C0";

        private uint A, B, C, D;
        private long length;
        private byte[] buffer;

        public Md4() {
            A0 = 0x67452301U;
            B0 = 0xEFCDAB89U;
            C0 = 0x98BADCFEU;
            D0 = 0x10325476U;
            buffer = new byte[BLOCKLENGTH];
        }

        protected override void HashCore(byte[] array, int ibStart, int cbSize) {
            int n = (int)(length % BLOCKLENGTH);
            int partLen = BLOCKLENGTH - n;
            int i = 0;
            length += cbSize;

            if(cbSize >= partLen){
                System.Buffer.BlockCopy(array, ibStart, buffer, n, partLen);
                TransformMd4Block(buffer, 0);
                i = partLen;
                while(i + BLOCKLENGTH - 1 < cbSize){
                    TransformMd4Block(array, ibStart + i);
                    i += BLOCKLENGTH;
                }
                n = 0;
            }
            if(i < cbSize){
                System.Buffer.BlockCopy(array, ibStart + i, buffer, n, cbSize - i);
            }
        }

        protected override byte[] HashFinal() {
            byte[] tail = PadBuffer();
            HashCore(tail, 0, tail.Length);

            return new byte[] {
                (byte)(A), (byte)(A >> 8), (byte)(A >> 16), (byte)(A >> 24), 
                (byte)(B), (byte)(B >> 8), (byte)(B >> 16), (byte)(B >> 24), 
                (byte)(C), (byte)(C >> 8), (byte)(C >> 16), (byte)(C >> 24), 
                (byte)(D), (byte)(D >> 8), (byte)(D >> 16), (byte)(D >> 24) 
            };
        }

        public override void Initialize() {
            A = A0;
            B = B0;
            C = C0;
            D = D0;
        }

        protected byte[] PadBuffer() {
            int padding;
            int n = (int)(length % BLOCKLENGTH);
            if(n < 56) padding = 56 - n; else padding = 120 - n;
            long bits = length << 3;

            byte[] pad = new byte[padding + 8];

            pad[0] = 0x80;
            pad[padding] = (byte)(bits & 0xFF);
            pad[padding + 1] = (byte)(bits >> 8 & 0xFF);
            pad[padding + 2] = (byte)(bits >> 16 & 0xFF);
            pad[padding + 3] = (byte)(bits >> 24 & 0xFF);
            pad[padding + 4] = (byte)(bits >> 32 & 0xFF);
            pad[padding + 5] = (byte)(bits >> 40 & 0xFF);
            pad[padding + 6] = (byte)(bits >> 48 & 0xFF);
            pad[padding + 7] = (byte)(bits >> 56 & 0xFF);
            return pad;
        }

        protected void TransformMd4Block(byte[] bytes, int i) {
            uint x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, xA, xB, xC, xD, xE, xF;
            x0 = (bytes[i] & 0xFFU) | (bytes[i + 1] & 0xFFU) << 8 | (bytes[i + 2] & 0xFFU) << 16 | (uint)(bytes[i + 3]) << 24;
            x1 = (bytes[i + 4] & 0xFFU) | (bytes[i + 5] & 0xFFU) << 8 | (bytes[i + 6] & 0xFFU) << 16 | (uint)(bytes[i + 7]) << 24;
            x2 = (bytes[i + 8] & 0xFFU) | (bytes[i + 9] & 0xFFU) << 8 | (bytes[i + 10] & 0xFFU) << 16 | (uint)(bytes[i + 11]) << 24;
            x3 = (bytes[i + 12] & 0xFFU) | (bytes[i + 13] & 0xFFU) << 8 | (bytes[i + 14] & 0xFFU) << 16 | (uint)(bytes[i + 15]) << 24;
            x4 = (bytes[i + 16] & 0xFFU) | (bytes[i + 17] & 0xFFU) << 8 | (bytes[i + 18] & 0xFFU) << 16 | (uint)(bytes[i + 19]) << 24;
            x5 = (bytes[i + 20] & 0xFFU) | (bytes[i + 21] & 0xFFU) << 8 | (bytes[i + 22] & 0xFFU) << 16 | (uint)(bytes[i + 23]) << 24;
            x6 = (bytes[i + 24] & 0xFFU) | (bytes[i + 25] & 0xFFU) << 8 | (bytes[i + 26] & 0xFFU) << 16 | (uint)(bytes[i + 27]) << 24;
            x7 = (bytes[i + 28] & 0xFFU) | (bytes[i + 29] & 0xFFU) << 8 | (bytes[i + 30] & 0xFFU) << 16 | (uint)(bytes[i + 31]) << 24;
            x8 = (bytes[i + 32] & 0xFFU) | (bytes[i + 33] & 0xFFU) << 8 | (bytes[i + 34] & 0xFFU) << 16 | (uint)(bytes[i + 35]) << 24;
            x9 = (bytes[i + 36] & 0xFFU) | (bytes[i + 37] & 0xFFU) << 8 | (bytes[i + 38] & 0xFFU) << 16 | (uint)(bytes[i + 39]) << 24;
            xA = (bytes[i + 40] & 0xFFU) | (bytes[i + 41] & 0xFFU) << 8 | (bytes[i + 42] & 0xFFU) << 16 | (uint)(bytes[i + 43]) << 24;
            xB = (bytes[i + 44] & 0xFFU) | (bytes[i + 45] & 0xFFU) << 8 | (bytes[i + 46] & 0xFFU) << 16 | (uint)(bytes[i + 47]) << 24;
            xC = (bytes[i + 48] & 0xFFU) | (bytes[i + 49] & 0xFFU) << 8 | (bytes[i + 50] & 0xFFU) << 16 | (uint)(bytes[i + 51]) << 24;
            xD = (bytes[i + 52] & 0xFFU) | (bytes[i + 53] & 0xFFU) << 8 | (bytes[i + 54] & 0xFFU) << 16 | (uint)(bytes[i + 55]) << 24;
            xE = (bytes[i + 56] & 0xFFU) | (bytes[i + 57] & 0xFFU) << 8 | (bytes[i + 58] & 0xFFU) << 16 | (uint)(bytes[i + 59]) << 24;
            xF = (bytes[i + 60] & 0xFFU) | (bytes[i + 61] & 0xFFU) << 8 | (bytes[i + 62] & 0xFFU) << 16 | (uint)(bytes[i + 63]) << 24;

            uint aa, bb, cc, dd;

            aa = A;
            bb = B;
            cc = C;
            dd = D;

            aa += ((bb & cc) | ((~bb) & dd)) + x0;
            aa = aa << 3 | aa >> -3;
            dd += ((aa & bb) | ((~aa) & cc)) + x1;
            dd = dd << 7 | dd >> -7;
            cc += ((dd & aa) | ((~dd) & bb)) + x2;
            cc = cc << 11 | cc >> -11;
            bb += ((cc & dd) | ((~cc) & aa)) + x3;
            bb = bb << 19 | bb >> -19;
            aa += ((bb & cc) | ((~bb) & dd)) + x4;
            aa = aa << 3 | aa >> -3;
            dd += ((aa & bb) | ((~aa) & cc)) + x5;
            dd = dd << 7 | dd >> -7;
            cc += ((dd & aa) | ((~dd) & bb)) + x6;
            cc = cc << 11 | cc >> -11;
            bb += ((cc & dd) | ((~cc) & aa)) + x7;
            bb = bb << 19 | bb >> -19;
            aa += ((bb & cc) | ((~bb) & dd)) + x8;
            aa = aa << 3 | aa >> -3;
            dd += ((aa & bb) | ((~aa) & cc)) + x9;
            dd = dd << 7 | dd >> -7;
            cc += ((dd & aa) | ((~dd) & bb)) + xA;
            cc = cc << 11 | cc >> -11;
            bb += ((cc & dd) | ((~cc) & aa)) + xB;
            bb = bb << 19 | bb >> -19;
            aa += ((bb & cc) | ((~bb) & dd)) + xC;
            aa = aa << 3 | aa >> -3;
            dd += ((aa & bb) | ((~aa) & cc)) + xD;
            dd = dd << 7 | dd >> -7;
            cc += ((dd & aa) | ((~dd) & bb)) + xE;
            cc = cc << 11 | cc >> -11;
            bb += ((cc & dd) | ((~cc) & aa)) + xF;
            bb = bb << 19 | bb >> -19;

            aa += ((bb & (cc | dd)) | (cc & dd)) + x0 + 0x5A827999U;
            aa = aa << 3 | aa >> -3;
            dd += ((aa & (bb | cc)) | (bb & cc)) + x4 + 0x5A827999U;
            dd = dd << 5 | dd >> -5;
            cc += ((dd & (aa | bb)) | (aa & bb)) + x8 + 0x5A827999U;
            cc = cc << 9 | cc >> -9;
            bb += ((cc & (dd | aa)) | (dd & aa)) + xC + 0x5A827999U;
            bb = bb << 13 | bb >> -13;
            aa += ((bb & (cc | dd)) | (cc & dd)) + x1 + 0x5A827999U;
            aa = aa << 3 | aa >> -3;
            dd += ((aa & (bb | cc)) | (bb & cc)) + x5 + 0x5A827999U;
            dd = dd << 5 | dd >> -5;
            cc += ((dd & (aa | bb)) | (aa & bb)) + x9 + 0x5A827999U;
            cc = cc << 9 | cc >> -9;
            bb += ((cc & (dd | aa)) | (dd & aa)) + xD + 0x5A827999U;
            bb = bb << 13 | bb >> -13;
            aa += ((bb & (cc | dd)) | (cc & dd)) + x2 + 0x5A827999U;
            aa = aa << 3 | aa >> -3;
            dd += ((aa & (bb | cc)) | (bb & cc)) + x6 + 0x5A827999U;
            dd = dd << 5 | dd >> -5;
            cc += ((dd & (aa | bb)) | (aa & bb)) + xA + 0x5A827999U;
            cc = cc << 9 | cc >> -9;
            bb += ((cc & (dd | aa)) | (dd & aa)) + xE + 0x5A827999U;
            bb = bb << 13 | bb >> -13;
            aa += ((bb & (cc | dd)) | (cc & dd)) + x3 + 0x5A827999U;
            aa = aa << 3 | aa >> -3;
            dd += ((aa & (bb | cc)) | (bb & cc)) + x7 + 0x5A827999U;
            dd = dd << 5 | dd >> -5;
            cc += ((dd & (aa | bb)) | (aa & bb)) + xB + 0x5A827999U;
            cc = cc << 9 | cc >> -9;
            bb += ((cc & (dd | aa)) | (dd & aa)) + xF + 0x5A827999U;
            bb = bb << 13 | bb >> -13;

            aa += (bb ^ cc ^ dd) + x0 + 0x6ED9EBA1U;
            aa = aa << 3 | aa >> -3;
            dd += (aa ^ bb ^ cc) + x8 + 0x6ED9EBA1U;
            dd = dd << 9 | dd >> -9;
            cc += (dd ^ aa ^ bb) + x4 + 0x6ED9EBA1U;
            cc = cc << 11 | cc >> -11;
            bb += (cc ^ dd ^ aa) + xC + 0x6ED9EBA1U;
            bb = bb << 15 | bb >> -15;
            aa += (bb ^ cc ^ dd) + x2 + 0x6ED9EBA1U;
            aa = aa << 3 | aa >> -3;
            dd += (aa ^ bb ^ cc) + xA + 0x6ED9EBA1U;
            dd = dd << 9 | dd >> -9;
            cc += (dd ^ aa ^ bb) + x6 + 0x6ED9EBA1U;
            cc = cc << 11 | cc >> -11;
            bb += (cc ^ dd ^ aa) + xE + 0x6ED9EBA1U;
            bb = bb << 15 | bb >> -15;
            aa += (bb ^ cc ^ dd) + x1 + 0x6ED9EBA1U;
            aa = aa << 3 | aa >> -3;
            dd += (aa ^ bb ^ cc) + x9 + 0x6ED9EBA1U;
            dd = dd << 9 | dd >> -9;
            cc += (dd ^ aa ^ bb) + x5 + 0x6ED9EBA1U;
            cc = cc << 11 | cc >> -11;
            bb += (cc ^ dd ^ aa) + xD + 0x6ED9EBA1U;
            bb = bb << 15 | bb >> -15;
            aa += (bb ^ cc ^ dd) + x3 + 0x6ED9EBA1U;
            aa = aa << 3 | aa >> -3;
            dd += (aa ^ bb ^ cc) + xB + 0x6ED9EBA1U;
            dd = dd << 9 | dd >> -9;
            cc += (dd ^ aa ^ bb) + x7 + 0x6ED9EBA1U;
            cc = cc << 11 | cc >> -11;
            bb += (cc ^ dd ^ aa) + xF + 0x6ED9EBA1U;
            bb = bb << 15 | bb >> -15;

            A += aa;
            B += bb;
            C += cc;
            D += dd;
        }
    }
}
