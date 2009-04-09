/******************************************************************************
 *
 * Sugar for Java, 2.0.0
 * Copyright (C) 2001-2006  Dipl.-Inf. (FH) Johann Nepomuk Loefflmann,
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

/**
 * This exception is thrown if the program should exit
 */
public class ExitException extends Exception {
	private static final long serialVersionUID = -1091703718210433207L;
	private int exitcode;

   public ExitException(String s) {
      super(s);
      exitcode = 0;
   }

   public ExitException(String s, int exitcode) {
      super(s);
      this.exitcode = exitcode;
   }

   public int getExitCode() {
      return exitcode;
   }

}
