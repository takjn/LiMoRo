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
#ifndef Sd2CardExt_h
#define Sd2CardExt_h
#include "Sd2Card.h"
class Sd2CardExt : public Sd2Card {
 public:
  Sd2CardExt(void) : Sd2Card() {}
  uint8_t readExtDataPort(uint8_t mio, uint8_t func, uint16_t addr, uint8_t* dst);
  uint8_t readExtMemory(uint8_t mio, uint8_t func, uint32_t addr, uint16_t count, uint8_t* dst);
  uint8_t writeExtDataPort(uint8_t mio, uint8_t func, uint16_t addr, const uint8_t* src);
  uint8_t writeExtMemory(uint8_t mio, uint8_t func, uint32_t addr, uint16_t count, const uint8_t* src);
  uint8_t writeExtMask(uint8_t mio, uint8_t func, uint32_t addr, uint8_t mask, const uint8_t* src);
 protected:
  uint8_t readExt(uint32_t arg, uint8_t* src, uint16_t count);
  uint8_t writeExt(uint32_t arg, const uint8_t* src, uint16_t count);
};
#endif  // Sd2CardExt_h
