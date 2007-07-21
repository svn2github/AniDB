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

#include "hash-base.h"

HashChecksum::HashChecksum(){
	value=0;
    length=0;
    hex=false;
    uppercase=false;
}
void HashChecksum::reset() {
	value = 0;
	length = 0;
}
void HashChecksum::update(char b) {
	update((int)(b & 0xFF));
}
void HashChecksum::update(int b) {
	length++;
}
void HashChecksum::update(char* chars, int offset, int length) {
	for (int i=offset; i < length; i++)
		update(chars[i]);
}
void HashChecksum::update(char* chars, int length) {
	update(chars, 0, length);
}
unsigned int HashChecksum::getValue() {
	return value;
}
unsigned int HashChecksum::getLength() {
	return length;
}
void HashChecksum::setHex(bool b){
	hex = b;
}
void HashChecksum::setUpperCase(bool b){
	uppercase = b;
}
