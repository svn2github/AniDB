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
using System.Text;
using System.Collections;
using System.Diagnostics;

namespace AVDump2Lib.Hashes {
	public enum eBaseOption {
		Binary = 1,
		Base04 = 2,
		Octal = 3,
		Decimal = 4,
		Heximal = 5,
		Base32 = 6,
		Base32Z = 7,
		Base32Hex = 8,
		Base36 = 9,
		Base62 = 10,
		Base64 = 11,
		LowerCase = 1 << 10,
		Reverse = 1 << 11,
		Pad = 1 << 12,
		All = LowerCase | Reverse | Pad
	}

	public class BitVector {
		private byte[] bits;

		public int Count { get; private set; }
		public bool Reverse { get; private set; }

		public BitVector(BitVector bits) : this((byte[])bits.bits.Clone(), bits.bits.Length * 8, bits.Reverse) { }
		public BitVector(byte[] bits) : this((byte[])bits.Clone(), bits.Length * 8, false) { }
		public BitVector(int length) {
			bits = new byte[(length - 1) / 8 + 1];
			Count = length;
		}
		public BitVector(byte[] bits, int length, bool reverse) {
			this.Count = length;
			this.Reverse = reverse;

			if(bits.Length * 8 < length) {
				this.bits = new byte[length];
				Array.Copy(bits, this.bits, bits.Length);
			} else {
				this.bits = (byte[])bits.Clone();
			}
		}

		public bool this[int index] {
			get {
				int byteIndex, bitIndex;

				bitIndex = 1 << (index % 8);
				if(Reverse) index = Count - 1 - index;
				byteIndex = index / 8;

				return (bits[byteIndex] & bitIndex) != 0;
			}
			set {
				int byteIndex, bitIndex;

				bitIndex = 1 << (index % 8);
				if(Reverse) index = Count - 1 - index;
				byteIndex = index / 8;

				bits[byteIndex] = (byte)(value ? (bits[byteIndex] | bitIndex) : (bits[byteIndex] & ~bitIndex));
			}
		}

		public void ShiftLeft(int count) { throw new Exception(); }
		public void ShiftRight(int count) { throw new Exception(); }
	}
	public static class BaseConverter {
		public static string ToString(byte[] value, eBaseOption baseOption) {
			string symbols = Enum2Symbols(baseOption);
			byte[] radix = new byte[value.Length];
			radix[0] = (byte)symbols.Length;

			string result = ToString(new BitVector(value, value.Length * 8, (baseOption & eBaseOption.Reverse) != 0), new BitVector(radix), symbols);
			if((baseOption & eBaseOption.Pad) != 0) {
				int padCount =  (int)Math.Ceiling((value.Length<<3 ) *Math.Log(2, symbols.Length));
				result = result.PadLeft(padCount, symbols[0]);
			}

			return result;
		}

		private static string ToString(BitVector value, BitVector radix, string symbols) {
			string result = "";
			BitVector quotient;
			while(XGreaterThanOrEqualsY(value, radix)) {
				quotient = Divide(value, radix);
				result = symbols[ToInt(value)] + result;
				value = quotient;
			}
			result = symbols[ToInt(value)] + result;

			return result;
		}

		public static BitVector Divide(BitVector dividend, BitVector divisor) {
			divisor = new BitVector(divisor);
			int shiftCount = Align(dividend, divisor);
			if(shiftCount < 0) return new BitVector(dividend.Count);

			BitVector quotient = new BitVector(dividend.Count);

			while(shiftCount >= 0) {
				ShiftRight(quotient, 1);

				if(XGreaterThanOrEqualsY(dividend, divisor)) {
					Substract(dividend, divisor);
					quotient[0] = true;
				}
				ShiftLeft(divisor, 1);
				shiftCount--;
			}

			return quotient;
		}

		public static string ToBin(BitVector value) {
			string result = "";
			for(int i = 0;i < value.Count;i++) result = (value[i] ? "1" : "0") + result;
			return result;
		}

		public static int ToInt(BitVector value) {
			int result = 0;
			for(int i = 0;i < value.Count;i++) result += value[i] ? 1 << i : 0;
			return result;
		}

		public static bool XGreaterThanOrEqualsY(BitVector x, BitVector y) {
			for(int i = x.Count - 1;i >= 0;i--) if(x[i] ^ y[i]) return x[i];
			return true;
		}

		public static void Substract(BitVector minuend, BitVector subtrahend) {
			bool borrowed = false;

			for(int i = 0;i < minuend.Count;i++) {
				minuend[i] ^= borrowed ^ subtrahend[i];
				borrowed = (minuend[i] && subtrahend[i]) || (minuend[i] && borrowed) || (borrowed && subtrahend[i]);
			}
		}

		public static int Align(BitVector dividend, BitVector divisor) {
			int posDD = dividend.Count - 1;
			for(;posDD >= 0;posDD--) if(dividend[posDD]) break;

			int posDS = divisor.Count - 1;
			for(;posDS >= 0;posDS--) if(divisor[posDS]) break;

			if(posDS < posDD) ShiftRight(divisor, posDD - posDS);

			return posDD - posDS;
		}

		public static void ShiftRight(BitVector value, int count) {
			for(int i = value.Count - (1 + count);i >= 0;i--) value[i + count] = value[i];
			for(count--;count >= 0;count--) value[count] = false;
		}
		public static void ShiftLeft(BitVector value, int count) {
			for(int i = count;i < value.Count;i++) value[i - count] = value[i];
			for(count--;count >= 0;count--) value[value.Count - 1 - count] = false;
		}


		private static readonly string base2 = "01";
		private static readonly string base4 = "0123";
		private static readonly string base8 = "01234567";
		private static readonly string base10 = "0123456789";
		private static readonly string base16 = "0123456789ABCDEF";
		private static readonly string base32Hex = "0123456789ABCDEFGHIJKLMNOPQRSTUV";
		private static readonly string base32Z = "0123456789ABCDEFGHJKMNPQRSTVWXYZ";
		private static readonly string base32 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		private static readonly string base36 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		private static readonly string base62 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		private static readonly string base64 = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+/";

		private static string Enum2Symbols(eBaseOption symbol) {
			switch(symbol & ~eBaseOption.All) {
				case eBaseOption.Binary:
					return base2;
				case eBaseOption.Base04:
					return base4;
				case eBaseOption.Octal:
					return base8;
				case eBaseOption.Decimal:
					return base10;
				case eBaseOption.Heximal:
					return ((symbol & eBaseOption.LowerCase) == 0) ? base16 : base16.ToLower();
				case eBaseOption.Base32:
					return ((symbol & eBaseOption.LowerCase) == 0) ? base32 : base32.ToLower();
				case eBaseOption.Base32Z:
					return ((symbol & eBaseOption.LowerCase) == 0) ? base32Z : base32Z.ToLower();
				case eBaseOption.Base32Hex:
					return ((symbol & eBaseOption.LowerCase) == 0) ? base32Hex : base32Hex.ToLower();
				case eBaseOption.Base36:
					return ((symbol & eBaseOption.LowerCase) == 0) ? base36 : base36.ToLower();
				case eBaseOption.Base62:
					return base62;
				case eBaseOption.Base64:
					return base64;
				default:
					throw new ArgumentException();
			}
		}
	}

}
