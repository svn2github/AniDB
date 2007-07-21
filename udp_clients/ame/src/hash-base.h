// Copyright (C) 2006 epoximator
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
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#ifndef __MYHASH_H
#define __MYHASH_H

typedef __int64 mylong;

template<typename T> inline const T rol(const T& a,const unsigned int n=1) { return (a<<n)|(a>>((sizeof(T)<<3)-n)); }

class HashDigest{
public:
	virtual char *getName()=0;
	virtual int getHashSize()=0;
	virtual int getBlockSize()=0;
	virtual void update(char b)=0;
	virtual void update(char *in, int offset, int length)=0;
	virtual void digest(char* res)=0;
	virtual void reset()=0;
	virtual bool selfTest()=0;
	virtual HashDigest *clone()=0;
	virtual void dumpVar()=0;
};

class HashBase : public HashDigest {
protected:
	char *name;
	int hashSize;
	int blockSize;
	mylong count;
	char *buffer;
	
	HashBase(char *name, int hashSize, int blockSize);
	~HashBase();
public:
	char* getName();
	virtual int getHashSize();
	virtual int getBlockSize();
	virtual void update(char b);
	virtual void update(char* in, int offset, int len);
	virtual void digest(char* res);
	virtual void reset();

	virtual bool selfTest()=0;		
	virtual char* padBuffer(int &len)=0;
	virtual void getResult(char* res)=0;
	virtual void resetContext()=0;
	virtual void transform(char *in, int offset)=0;
	virtual void dumpVar()=0;
	static void format(char* chars, int len, bool uppercase, char *name);
};

class HashMD4 : public HashBase{
protected:
	static const int DIGEST_LENGTH = 16;
	static const int BLOCK_LENGTH = 64;

	static const int mA = 0x67452301;
	static const int mB = 0xefcdab89;
	static const int mC = 0x98badcfe;
	static const int mD = 0x10325476;

	static bool valid;
	char* DIGEST0;

	int a, b, c, d;
	int X[16];
public:
	HashMD4();
	HashMD4(HashMD4 *that);
	virtual bool selfTest();		
	virtual char* padBuffer(int &len);
	virtual void getResult(char* res);
	virtual void resetContext();
	virtual void transform(char *in, int offset);
	virtual HashDigest *clone();
	virtual void dumpVar();
};

class HashMD5 : public HashMD4{
public:
	HashMD5();
	HashMD5(HashMD5 *that);
	virtual bool selfTest();
	virtual void transform(char *in, int offset);
};

class HashChecksum{
protected:
	unsigned int value;
    unsigned int length;
    bool hex;
    bool uppercase;
public:
	HashChecksum();
	virtual unsigned int getValue();
	virtual unsigned int getLength();
	virtual void reset();
	virtual void update(char b);
	virtual void update(int b);
	virtual void update(char* chars, int offset, int length);
	virtual void update(char* chars, int length);
	virtual void setHex(bool b);
	virtual void setUpperCase(bool b);
};

class HashED2K : public HashChecksum{
private:
	HashMD4 md4;
	HashMD4 md4fin;

	static const int BLOCKSIZE = 9728000;
	char hash[16];
    void copyHash();
public:
	HashED2K();
	void reset();
	void update(char b);
	void update(char* buffer, int offset, size_t len);
	char* toString();
	void getHexValue(char* hex);
};

class HashCRC32 : public HashChecksum{
private:
	unsigned int crc;
public:
	HashCRC32();
	virtual void update(char b);
	void update(char* buffer, int offset, int len);
	void add_data(const char input[], unsigned int length);
	virtual unsigned int getValue();
};
#endif //__MYHASH_H
