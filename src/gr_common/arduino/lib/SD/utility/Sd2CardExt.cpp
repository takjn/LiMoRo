/*
Copyright (c) 2013, Toshiba Corporation. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/* Jul 11th 2017, Modified by Yuuki Okamiya for GR-LYCHEE*/

#include <Arduino.h>
#include "Sd2CardExt.h"

#include <SPI.h>

uint8_t const CMD48 = 0x30;
uint8_t const CMD49 = 0x31;
uint8_t const SD_CARD_ERROR_CMD48 = 0x80;
uint8_t const SD_CARD_ERROR_CMD49 = 0x81;

/** Send a byte to the card */
inline void spiSend(uint8_t b) {
#ifndef GRLYCHEE
	SPDR = b;
	while (!(SPSR & (1 << SPIF)));
#else
	SPI2.transfer(b);
#endif
}
/** Receive a byte from the card */
inline  uint8_t spiReceive(void) {
#ifndef GRLYCHEE
	spiSend(0xFF);
	return SPDR;
#else
	return SPI2.transfer(0xFF);
#endif
}

/** Perform Extention Read. */
uint8_t Sd2CardExt::readExt(uint32_t arg, uint8_t* dst, uint16_t count) {
  uint16_t i;
  // send command and argument.
  if (cardCommand(CMD48, arg) && cardCommand(CMD17, arg)) {
    error(SD_CARD_ERROR_CMD48);
    goto fail;
  }
  // wait for start block token.
  if (!waitStartBlock()) {
    goto fail;
  }
  // receive data
  for (i = 0; i < count; ++i) {
    dst[i] = spiReceive();
  }
  // skip dummy bytes and 16-bit crc.
  for (; i < 514; ++i) {
    spiReceive();
  }
  chipSelectHigh();
  spiSend(0xFF); // dummy clock to force FlashAir finish the command.
  return true;
 fail:
  chipSelectHigh();
  return false;
}
/** Perform Extention Write. */
uint8_t Sd2CardExt::writeExt(uint32_t arg, const uint8_t* src, uint16_t count) {
  uint16_t i;
  uint8_t status;
  // send command and argument.
  if (cardCommand(CMD49, arg) && cardCommand(CMD24, arg)) {
    error(SD_CARD_ERROR_CMD49);
    goto fail;
  }
  // send start block token.
  spiSend(DATA_START_BLOCK);
  // send data
  for (i = 0; i < count; ++i) {
    spiSend(src[i]);
  }
  // send dummy bytes until 512 bytes.
  for (; i < 512; ++i) {
    spiSend(0xFF);
  }
  // dummy 16-bit crc
  spiSend(0xFF);
  spiSend(0xFF);
  // wait a data response token
  status = spiReceive();
  if ((status & DATA_RES_MASK) != DATA_RES_ACCEPTED) {
    error(SD_CARD_ERROR_WRITE);
    goto fail;
  }
  // wait for flash programming to complete
  if (!waitNotBusy(SD_WRITE_TIMEOUT)) {
    error(SD_CARD_ERROR_WRITE_TIMEOUT);
    goto fail;
  }
  chipSelectHigh();
  return true;
 fail:
  chipSelectHigh();
  return false;
}

uint8_t Sd2CardExt::readExtDataPort(uint8_t mio, uint8_t func,
        uint16_t addr, uint8_t* dst) {
  uint32_t arg =
      (((uint32_t)mio & 0x1) << 31) |
      (mio ? (((uint32_t)func & 0x7) << 28) : (((uint32_t)func & 0xF) << 27)) |
      (((uint32_t)addr & 0x1FE00) << 9);
  return readExt(arg, dst, 512);
}
uint8_t Sd2CardExt::readExtMemory(uint8_t mio, uint8_t func,
        uint32_t addr, uint16_t count, uint8_t* dst) {
  uint32_t offset = addr & 0x1FF;
  if (offset + count > 512) count = 512 - offset;
  if (count == 0) return true;
  uint32_t arg =
      (((uint32_t)mio & 0x1) << 31) |
      (mio ? (((uint32_t)func & 0x7) << 28) : (((uint32_t)func & 0xF) << 27)) |
      ((addr & 0x1FFFF) << 9) |
      ((count - 1) & 0x1FF);
  return readExt(arg, dst, count);
}
uint8_t Sd2CardExt::writeExtDataPort(uint8_t mio, uint8_t func,
          uint16_t addr, const uint8_t* src) {
  uint32_t arg =
      (((uint32_t)mio & 0x1) << 31) |
      (mio ? (((uint32_t)func & 0x7) << 28) : (((uint32_t)func & 0xF) << 27)) |
      (((uint32_t)addr & 0x1FE00) << 9);
  return writeExt(arg, src, 512);
}
uint8_t Sd2CardExt::writeExtMemory(uint8_t mio, uint8_t func,
    uint32_t addr, uint16_t count, const uint8_t* src) {
  uint32_t arg =
      (((uint32_t)mio & 0x1) << 31) |
      (mio ? (((uint32_t)func & 0x7) << 28) : (((uint32_t)func & 0xF) << 27)) |
      ((addr & 0x1FFFF) << 9) |
      ((count - 1) & 0x1FF);
  return writeExt(arg, src, count);
}
uint8_t Sd2CardExt::writeExtMask(uint8_t mio, uint8_t func,
    uint32_t addr, uint8_t mask, const uint8_t* src) {
  uint32_t arg =
      (((uint32_t)mio & 0x1) << 31) |
      (mio ? (((uint32_t)func & 0x7) << 28) : (((uint32_t)func & 0xF) << 27)) |
      (0x1 << 26) |
      ((addr & 0x1FFFF) << 9) |
      mask;
  return writeExt(arg, src, 1);
}
