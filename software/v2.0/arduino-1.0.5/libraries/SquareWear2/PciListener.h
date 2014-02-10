/**
 * File: PciListener.h
 * Description:
 * PciManager library is an easy to use Pin Change Interrupt manager for Arduino.
 *
 * Author: Balazs Kelemen
 * Contact: prampec+arduino@gmail.com
 * Copyright: 2012 Balazs Kelemen
 * Copying permission statement:
    This file is part of PciManager.

    PciManager is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef PCI_LISTENER_H
#define PCI_LISTENER_H

#include <Arduino.h>

/**
 * PinChangeInterrupt listener abstract class. Please override this class with a custom pciHandleInterrupt() callback.
 */
class PciListener {
  public:
    /**
     * This method will be called by the PciManager on pin change. The function will be fired if any of the registered pins
     * are fired within the same vector.
     *  vector - Vector can be 0, 1 or 2 depending on the changed pin. (This parameter is not very useful.)
     */
    virtual void pciHandleInterrupt(byte vector) { };

    /** The pin being registered by this listener */
    byte pciPin;
    
    /** The PCI vector this pin belongs to. */
    byte pciVector;

    /** For internal use. Do not modify. */
    PciListener* pciNextListener;
};

#endif

