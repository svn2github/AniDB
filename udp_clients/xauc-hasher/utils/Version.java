/******************************************************************************
 *
 * Sugar for Java 1.3
 * Copyright (C) 2001-2005  Dipl.-Inf. (FH) Johann Nepomuk Loefflmann,
 * All Rights Reserved, http://www.jonelo.de
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * @author jonelo@jonelo.de
 *
 *****************************************************************************/

package utils;

import java.util.StringTokenizer;


@SuppressWarnings("unchecked")
public class Version implements Comparable
{
  private int
    major,
    sub,
    minor;

  public Version(int major, int sub) {
    this(major, sub, 0);
  }

  public Version(int major, int sub, int minor) {
    this.major=major;
    this.sub=sub;
    this.minor=minor;
  }

  public Version(String version) {
    major=0; sub=0; minor=0;
    StringTokenizer st = new StringTokenizer(version,".");
    if (st.hasMoreTokens()) major = Integer.parseInt(st.nextToken());
    if (st.hasMoreTokens()) sub = Integer.parseInt(st.nextToken());
    if (st.hasMoreTokens()) minor = Integer.parseInt(st.nextToken());
  }

  public String toString() {
    StringBuffer sb = new StringBuffer(8); // XX.YY.ZZ
    sb.append(major);
    sb.append('.');
    sb.append(sub);
    sb.append('.');
    sb.append(minor);
    return sb.toString();
  }

  public int getMajor() {
    return major;
  }

  public int getSub() {
    return sub;
  }

  public int getMinor() {
    return minor;
  }

  public int compareTo(Object o) {
    Version v = (Version)o;
    if (this.equals(v)) return 0;

    if (
         (major > v.getMajor()) ||
       ( (major == v.getMajor()) && (sub > v.getSub()) ) ||
       ( (major == v.getMajor()) && (sub == v.getSub()) && (minor > v.getMinor()) )
    ) return 1;
    else return -1;
  }

  public boolean equals(Object o) {
    if (!(o instanceof Version)) return false;
    Version v = (Version)o;
    return ((major == v.getMajor()) && (sub == v.getSub()) && (minor == v.getMinor()));
  }

  public int hashCode() {
     return major*10000+sub*100+minor;
  }

}
