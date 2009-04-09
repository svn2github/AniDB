/******************************************************************************
 *
 * Jacksum version 1.7.0 - checksum utility in Java
 * Copyright (C) 2001-2006 Dipl.-Inf. (FH) Johann Nepomuk Loefflmann,
 * All Rights Reserved, http://www.jonelo.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * E-mail: jonelo@jonelo.de
 *
 *****************************************************************************/

/*
 * This class implements the "Rocksoft^tm Model CRC Algorithm"
 * in the Java programming language
 *
 * For more information on the Rocksoft^tm Model CRC Algorithm, see the document
 * titled "A Painless Guide to CRC Error Detection Algorithms" by Ross
 * Williams (ross@guest.adelaide.edu.au.). This document is likely to be in
 * "ftp.adelaide.edu.au/pub/rocksoft"
 * Note: Rocksoft is a trademark of Rocksoft Pty Ltd, Adelaide, Australia.
 */

package hashing;

import java.security.NoSuchAlgorithmException;
import utils.GeneralString;
import utils.Service;


public class CrcGeneric extends AbstractChecksum  {

    private int width;         // Width in bits [8..64], width is one bit less than the raw poly width
    private long poly;         // The algorithm's polynomial which is specified without its top bit
    private long initialValue; // Initial register value
    private boolean refIn;     // Reflect input bytes?
    private boolean refOut;    // Reflect output CRC?
    private long xorOut;       // XOR this to output CRC
    private long[] table;      // Precomputed values
    private long topBit;       // Stores the value (2 ^ width)
    private long maskAllBits;  // Stores the value (2 ^ width) - 1
    private long maskHelp;     // Stores the value (2 ^ (width-8)) -1


    /**
     * Constructor with all parameters as defined in the
     * Rocksoft^tm Model CRC Algorithm
     * @param initialValue the initial register value
     * @param width the width of the value in bits
     * @param poly The algorithm's polynomial (without the highest bit)
     * @param refIn Reflect input bytes?
     * @param refOut Reflect output CRC?
     * @param xorOut XOR this to output CRC
     * @throws java.security.NoSuchAlgorithmException if the parameter cannot be used to create a correct object
     */
    public CrcGeneric(int width, long poly,
            long initialValue, boolean refIn,
            boolean refOut, long xorOut) throws NoSuchAlgorithmException {
        super();
        this.width = width;
        this.poly = poly;
        this.initialValue = initialValue;
        this.refIn = refIn;
        this.refOut = refOut;
        this.xorOut = xorOut;
        init();
    }


    /**
     * Constructor with a String parameter
     * @param props All parameters as defined in the Rocksoft^tm Model CRC Algorithm separated by a comma
     *        Example: crc:32,04C11DB7,FFFFFFFF,true,true,FFFFFFFF
     * @throws java.security.NoSuchAlgorithmException if the parameter cannot be used to create a correct object
     */
    public CrcGeneric(String props) throws NoSuchAlgorithmException {
        String[] array = GeneralString.split(props, ","); // we need compatibility with JRE 1.3
        if (array.length != 6) throw new NoSuchAlgorithmException("Can't create the algorithm, 6 parameters are expected");
        try {
            width = Integer.parseInt(array[0]);
            poly = Long.parseLong(array[1], 16);
            initialValue = new java.math.BigInteger(array[2], 16).longValue(); //Long.parseLong(array[2], 16);
            refIn = array[3].equalsIgnoreCase("true");
            refOut = array[4].equalsIgnoreCase("true");
            xorOut = new java.math.BigInteger(array[5], 16).longValue();
        } catch (NumberFormatException e) {
            throw new NoSuchAlgorithmException("Unknown algorithm: invalid parameters. " + e.toString());
        }
        init();
    }

    /**
     * Initializes the table, and a few other useful values
     */
    private void init() throws NoSuchAlgorithmException {
        topBit = 1L << (width - 1);       // stores the value (2 ^ width)
        maskAllBits = ~0L >>> (64-width); // stores the value (2 ^ width) - 1
        maskHelp = maskAllBits >>> 8;     // stores the value (2 ^ (width-8)) -1
        check();
        fillTable();
        reset();
    }


    /**
     * Checks the parameters of the object
     */
    private void check() throws NoSuchAlgorithmException {
        if (width < 8 || width > 64) {
            throw new NoSuchAlgorithmException("Error: width has to be in range [8..64].");
        }

        if (poly != (poly & maskAllBits)) {
            throw new NoSuchAlgorithmException("Error: invalid polynomial for the " + width + " bit CRC.");
        }

        if (initialValue != (initialValue & maskAllBits)) {
            throw new NoSuchAlgorithmException("Error: invalid init value for the " + width + " bit CRC.");
        }

        if (xorOut != (xorOut & maskAllBits)) {
            throw new NoSuchAlgorithmException("Error: invalid xorOut value for the " + width + " bit CRC.");
        }
    }


    /**
     * Resets the checksum object to its initial values for further use
     */
    public void reset() {
        length = 0;
        // Load the register with an initial value.
        value = initialValue;

        if (refIn) {
            // Reflect the initial value.
            value = reflect(value, width);
        }
    }


    /**
     * The toString() method is derived from the AbstractChecksum
     * @return a String which is understood by the constructor
     */
    public String getString() {
        StringBuffer sb = new StringBuffer();
        int nibbles = width / 4 + ((width % 4 > 0) ? 1 : 0);
        sb.append(width);
        sb.append(",");
        sb.append(Service.hexformat(poly, nibbles).toUpperCase());
        sb.append(",");
        sb.append(Service.hexformat(initialValue, nibbles).toUpperCase());
        sb.append(",");
        sb.append(refIn? "true": "false");
        sb.append(",");
        sb.append(refOut? "true": "false");
        sb.append(",");
        sb.append(Service.hexformat(xorOut, nibbles).toUpperCase());
        return sb.toString();
    }


    /**
     * Get the name of the algorithm
     * @return the name of the algorithm as String
     */
    public String getName() {
        if (name == null) {
            return getString();
        } else
            return name;
    }


    /**
     * Set the initial register value
     * @param initialValue the initial register value
     */
    public void setInitialValue(long initialValue) {
        this.initialValue = initialValue;
    }


    /**
     * Get the initial register value
     * @return the initial register value
     */
    public long getInitialValue() {
        return initialValue;
    }


    /**
     * Set the width in bits
     * @param width the width in bits
     */
    public void setWidth(int width) {
        this.width = width;
    }


    /**
     * Get the width in bits
     * @return the width in bits
     */
    public int getWidth() {
        return width;
    }


    /**
     * Set the algorithm's polynomial
     * @param poly the algorithm's polynomial
     */
    public void setPoly(long poly) {
        this.poly = poly;
    }


    /**
     * Get the algorithm's polynomial
     * @return the algorithm's polynomial
     */
    public long getPoly() {
        return this.poly;
    }


    /**
     * Reflect input bytes?
     * @param refIn reflect input bytes?
     */
    public void setRefIn(boolean refIn) {
        this.refIn = refIn;
    }


    /**
     * Should input bytes be reflected?
     * @return should input bytes be reflected?
     */
    public boolean getRefIn() {
        return this.refIn;
    }


    /**
     * Set whether the output CRC should be reflected
     * @param refOut should the output CRC be reflected?
     */
    public void setRefOut(boolean refOut) {
        this.refOut = refOut;
    }


    /**
     * Get whether the output CRC should be reflected
     * @return should the output CRC be reflected?
     */
    public boolean getRefOut() {
        return this.refOut;
    }


    /**
     * Set the XOR parameter
     * @param xorOut the XOR parameter
     */
    public void setXorOut(long xorOut) {
        this.xorOut = xorOut;
    }


    /**
     * Get the XOR parameter
     * @return the XOR parameter
     */
    public long getXorOut() {
        return xorOut;
    }


    /**
     * Reflects a value by taking the least significant bits into account
     * Example: reflect(0x3e23L, 3) == 0x3e26
     *
     * @param value the value which should be reflected
     * @param bits the number of bits to be reflected
     * @return the value with the bottom bits [0,64] reflected.
     */
    private static long reflect(long value, int bits) {
        long temp = 0L;
        for (int i = 0; i < bits; i++) {
            temp <<= 1L;
            temp |= (value & 1L);
            value >>>= 1L;
        }
        return (value << bits) | temp;
    }


    /**
     * Precomputes all 256 values
     */
    private void fillTable() {
        long remainder;
        boolean mybit;
        table = new long[256];

        // perform binary long division, a bit at a time
        for (int dividend = 0; dividend < 256; dividend++) {

            // initialize the remainder
            remainder = (long)dividend;

            if (refIn) { // reflection?
                remainder = reflect(remainder, 8);
            }

            remainder <<= (width - 8);

            for (int bit = 0; bit < 8; bit++) {
                // try to divide the current data bit
                mybit = ((remainder & topBit) != 0);
                remainder <<= 1;
                if (mybit) {
                    remainder ^= poly;
                }
            }

            if (refIn) { // reflection?
                remainder = reflect(remainder, width);
            }
            // save the result in the table
            table[dividend] = (remainder & maskAllBits);
        }
    }


    /**
     * Updates the checksum with the specified byte
     * @param b the byte
     */
    public void update(byte b) {
        // divide the byte by the polynomial
        int index;
        if (refIn) {
            // Compute the index into the precomputed CRC table.
            index = ((int)(value ^ b) & 0xff);

            // Slide the value a full byte to the right.
            value >>>= 8;

            // Clear the bits of the highest byte of the value.
            value &= maskHelp;

        } else {
            // Compute the index into the precomputed CRC table.
            index = ((int)((value >>> (width-8)) ^ b) & 0xff);

            // Slide the value a full byte to the left.
            value <<= 8;
        }

        // xor the value with the appropriate precomputed table value
        value ^= (table[index]);
        length++;
    }


    /**
     * Updates the checksum with the specified byte
     * @param b the byte
     */
    public void update(int b) {
        update((byte)(b & 0xFF));
    }


    /**
     * Returns the value of the checksum
     * @return the value of the checksum
     */
    public long getValue() {
        return getFinal();
    }


    /**
     * Returns the CRC value for the input processed so far
     * @return the CRC value for the input processed so far
     */
    private long getFinal() {
        long remainder = value; // save the value

        // is output reflection still necessary?
        if (refIn != refOut) {
            remainder = reflect(remainder, width);
        }
        // the final remainder is the CRC result
        return (remainder ^ xorOut) & maskAllBits;
    }


    /**
     * Returns the result of the computation as byte array
     * @return the result of the computation as byte array
     */
    public byte[] getByteArray() {
        long finalvalue = getFinal();
        byte array[] = new byte[width / 8  + ((width % 8 > 0) ? 1 : 0)];

        for (int i = array.length - 1; i > -1; i--) {
            array[i] = (byte)(finalvalue & 0xFF);
            finalvalue >>>= 8;
        }
        return array;
    }

}
