/*
 * Copyright (C) 2015  Andreas Rohner
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <wiringPi.h> //contains arduino-like commands (digitalWrite(), etc.)
#include "stdint.h" //contains "uint8_t" variable type
#include <stdlib.h> //contains "rand()"

#ifndef RFTRANSMITTER_H_
#define RFTRANSMITTER_H_

//#if ARDUINO >= 100
//#include "Arduino.h"
//#else
//#include "WProgram.h"
//#include "pins_arduino.h"
//#endif


class RFTransmitter {
    uint8_t packageId;

    const uint8_t nodeId, outputPin;
    // Pulse lenght in microseconds
    const unsigned int pulseLength;
    // Backoff period for repeated sends in milliseconds
    unsigned int backoffDelay;
    // How often a reliable package is resent
    uint8_t resendCount;
    uint8_t lineState;

    void send0() {
      lineState = !lineState;
      digitalWrite(outputPin, lineState);
      delayMicroseconds(pulseLength << 1);
    }

    void send1() {
      digitalWrite(outputPin, !lineState);
      delayMicroseconds(pulseLength);
      digitalWrite(outputPin, lineState);
      delayMicroseconds(pulseLength);
    }

    void send00() {
      send0();
      delayMicroseconds(pulseLength << 1);
    }
    void send01() {
      send1();
      delayMicroseconds(pulseLength << 1);
    }
    void send10() {
      send1();
      send0();
    }
    void send11() {
      send1();
      send1();
    }

    void sendByte(uint8_t data) {
      uint8_t i = 4;
      do {
        switch(data & 3) {
        case 0:
          send00();
          break;
        case 1:
          send01();
          break;
        case 2:
          send10();
          break;
        case 3:
          send11();
          break;
        }
        data >>= 2;
      } while(--i);
    }

    void sendByteRed(uint8_t data) {
      sendByte(data);
      sendByte(data);
      sendByte(data);
    }

    void sendPackage(uint8_t *data, uint8_t len);

  public:
    RFTransmitter(uint8_t outputPin, uint8_t nodeId = 0, unsigned int pulseLength = 100,
        unsigned int backoffDelay = 100, uint8_t resendCount = 1) : packageId(0), nodeId(nodeId), outputPin(outputPin),
        pulseLength(pulseLength), backoffDelay(backoffDelay), resendCount(resendCount) {

      pinMode(outputPin, OUTPUT);
      digitalWrite(outputPin, LOW);
      lineState = LOW;
    }

    void setBackoffDelay(unsigned int millies) { backoffDelay = millies; }
    void send(uint8_t *data, uint8_t len);
    void resend(uint8_t *data, uint8_t len);
//    void print(char *message) { send((uint8_t *)message, strlen(message)); }
//    void print(unsigned int value, uint8_t base = DEC) {
//      char printBuf[5];
//      uint8_t len = 0;
//
//      for (uint8_t i = sizeof(printBuf) - 1; i >= 0; --i, ++len) {
//        printBuf[i] = "0123456789ABCDEF"[value % base];
//        value /= base;
//      }
//
//      uint8_t *data = (uint8_t *)printBuf;
//      while (*data == '0' && len > 1) {
//        --len;
//       ++data;
//      }
//
//      send(data, len);
//    }
};

#endif /* RFTRANSMITTER_H_ */
