/**
 * File: PciManager.cpp
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
#include "PciManager.h"


/**
 * Add listener to the chain.
 */
void PciManagerClass::add(PciListener* listener) {

  // -- A listener should be registered only once.
  this->remove(listener);
  
  if(this->_firstListener == NULL) {
  
    // -- This is the first listener being registered.
    this->_firstListener = listener;
    
  } else {
  
    PciListener* lastlistener = this->_firstListener;
    // -- Find the last listener, to build a chain.
    while(lastlistener->pciNextListener != NULL) {
      lastlistener = lastlistener->pciNextListener;
    }
    // -- Last listener found, let's add this listener to the end of the chain.
    lastlistener->pciNextListener = listener;
    
  }
  listener->pciNextListener = NULL;
}


/**
 * Remove listener from the chain.
 */
void PciManagerClass::remove(PciListener* listener) {
  if(this->_firstListener != NULL) {
    if(this->_firstListener == listener) {
      // -- This was the first listener.
      this->_firstListener = listener->pciNextListener;
    } else {
      PciListener* lastlistener = this->_firstListener;
      // -- Find this listener in the chain.
      while(lastlistener->pciNextListener != NULL) {
        if(lastlistener->pciNextListener == listener) {
          // -- Remove the listener with joining the chain.
          lastlistener->pciNextListener = listener->pciNextListener;
          break;
        }
        lastlistener = lastlistener->pciNextListener;
      }
    }
  }
}

void PciManagerClass::registerListener(byte pin, PciListener* listener) {
  // -- PCINT vector will be 0, 1 or 2 depending on the pin.
  // --  digitalPinToPCICRbit(pin) will calculate which one it is.
  listener->pciVector = digitalPinToPCICRbit(pin);
  listener->pciPin = pin;

  volatile uint8_t* pcicr = digitalPinToPCICR(pin);
  *pcicr |= (1 << listener->pciVector);
  volatile uint8_t* pcmsk = digitalPinToPCMSK(pin);
  *pcmsk |= (1 << digitalPinToPCMSKbit(pin));
  
  this->add(listener);
}

void PciManagerClass::removeListener(PciListener* listenerToRemove) {
  this->remove(listenerToRemove);
  boolean hasMoreListenersOnSamePin = false;
  boolean hasMoreListenersOnSameVector = false;

  PciListener* listener = _firstListener;
  while(listener != NULL) {
    if(listener->pciPin == listenerToRemove->pciPin) {
      hasMoreListenersOnSamePin = true;
      hasMoreListenersOnSameVector = true;
      break;
    }
    if(listener->pciVector == listenerToRemove->pciVector) {
      hasMoreListenersOnSameVector = true;
    }
    listener = listener->pciNextListener;
  }
  
  if(!hasMoreListenersOnSamePin) {
    // -- Remove mask if no other uses this pin.
    volatile uint8_t* pcmsk = digitalPinToPCMSK(listenerToRemove->pciPin);
    *pcmsk &= ~(1 << digitalPinToPCMSKbit(listenerToRemove->pciPin));

    if(!hasMoreListenersOnSameVector) {
      // -- Remove vector registration if no listeners for this vector.
      volatile uint8_t* pcicr = digitalPinToPCICR(listenerToRemove->pciPin);
      *pcicr &= ~(1 << listenerToRemove->pciVector);
    }
  }
}

/**
 * Walk through the chain and call all listener.
 */
void PciManagerClass::callListeners(byte pciVectorId) {
  // -- (If first is NULL, than nothing is registered.)
  PciListener* listener = _firstListener;
  while(listener != NULL) {
    if(listener->pciVector == pciVectorId) {
      listener->pciHandleInterrupt(pciVectorId);
    }
    listener = listener->pciNextListener;
  }
}

/**
 * Global interrupt handling implementations.
 */
ISR(PCINT0_vect){
  PciManager.callListeners(0);
}
ISR(PCINT1_vect){
  PciManager.callListeners(1);
}
ISR(PCINT2_vect){
  PciManager.callListeners(2);
}


/**
 * Create a singleton from this manager class.
 */
PciManagerClass PciManager;
