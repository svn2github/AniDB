/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package aniAdd2;

import aniAdd2.communication.ICommunication;
import aniAdd2.exts.IAA2Extension;

/**
 *
 * @author Arokh
 */
public interface IAniAdd2 extends ICommunication, Iterable<IAA2Extension> {
    IAA2Extension getExtension(Class extensionClass);
}
