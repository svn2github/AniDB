/******************************************************************************
 *
 * Sugar for Java, 1.3.0
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

public final class GeneralProgram {

    /**
     * Exits if the JVM does not fulfil the requirements,
     * does nothing under a free JVM
     * @param version Java version (e. g. "1.3.1")
     */
    public final static void requiresMinimumJavaVersion(final String version) {
        try {
            String ver = System.getProperty("java.vm.version");
            // no java check under non J2SE-compatible VMs
            if (isJ2SEcompatible() && (ver.compareTo(version) < 0)) {
                System.out.println("ERROR: a newer Java VM is required."
                        +"\nVendor of your Java VM:        "+System.getProperty("java.vm.vendor")
                        +"\nVersion of your Java VM:       "+ver
                        +"\nRequired minimum J2SE version: "+ version);

                // let's shut down the entire VM
                // no suitable Java VM has been found
                System.exit(1);
            }
        } catch (Throwable t) {
            System.out.println("uncaught exception: " + t);
            t.printStackTrace();
        }
    }

    public static boolean isSupportFor(String version) {
        return isJ2SEcompatible() ?
            (System.getProperty("java.version").compareTo(version) >= 0)
            :
            false;
    }

    public static boolean isJ2SEcompatible() {
        String vendor=System.getProperty("java.vm.vendor");
        if ( // gij
             vendor.startsWith("Free Software Foundation") ||
             // kaffe
             vendor.startsWith("Kaffe.org")
            ) return false;
        return true;
    }

}
