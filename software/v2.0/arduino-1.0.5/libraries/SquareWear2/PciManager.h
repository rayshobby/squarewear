/**
 * File: PciManager.h
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

#ifndef PCIMANAGER_H
#define PCIMANAGER_H

#include "PciListener.h"

/**
 * Register PCI listeners to pins with this Manager class.
 * The listener will be called on pin change of the vector of pins that this pin belongs to. So you always have
 * to check if your pin was changed or not. 
 */
class PciManagerClass
{
  public:
    /**
     * Register a pin for pin change interrupt.
     *  pin - The pin the listener to be registered for pinChange.
     *  listener - The lister that is called on pin change.
     */
    void registerListener(byte pin, PciListener* listener);
    /**
     * Remove registration of a pin for PCI.
     *  listener - The that should be removed.
     */
    void removeListener(PciListener* listener);

    /**
     * For internal use only.
     */
    void callListeners(byte pciVectorId);
  private:
    void add(PciListener* listener);
    void remove(PciListener* listener);
    PciListener* _firstListener;
};

extern PciManagerClass PciManager;

#endif
