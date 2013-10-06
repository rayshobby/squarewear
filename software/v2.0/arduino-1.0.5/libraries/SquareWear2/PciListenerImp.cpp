/**
 * File: PciListenerImp.cpp
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

#include "Arduino.h"
#include "PciListenerImp.h"
#include <Task.h>

PciListenerImp::PciListenerImp(byte pin, void (*callback)(byte changeKind)) {
  this->_callback = callback;
  
  pinMode(pin, INPUT);
}

void PciListenerImp::pciHandleInterrupt(byte vect) {
  volatile int val = digitalRead(this->pciPin);
  this->_callback(val);
}

