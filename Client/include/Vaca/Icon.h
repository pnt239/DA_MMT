// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005, 2006, 2007, 2008, David A. Capello
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in
//   the documentation and/or other materials provided with the
//   distribution.
// * Neither the name of the Vaca nor the names of its contributors
//   may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef VACA_ICON_H
#define VACA_ICON_H

#include "Vaca/base.h"
#include "Vaca/Size.h"
#include "Vaca/ResourceId.h"
#include "Vaca/SmartPtr.h"
#include "Vaca/GdiObject.h"

namespace Vaca {

class String;

/**
 * Used to destroy the HICON handle from GdiObject.
 * 
 * @internal
 */
struct Win32DestroyIcon
{
  static void destroy(HICON handle)
  {
    ::DestroyIcon(handle);
  }
};

/**
 * A smart pointer to an icon.
 *
 * This is a SmartPtr, so if you copy instances of icons they will be
 * referencing to the same place. You can't clone icons because you can't
 * modify them.
 *
 * Example
 * @code
 * Icon icon1(Size(32, 32));
 * Icon icon2 = icon1;       // icon1 and icon2 references to the same Icon
 * assert(icon1 == icon2);
 * @endcode
 *
 * @warning Win32: This is a @msdn{HICON} wrapper.
 */
class VACA_DLL Icon : private SmartPtr<GdiObject<HICON, Win32DestroyIcon> >
{
public:
  Icon();
  Icon(const Icon& icon);
  explicit Icon(ResourceId iconId, const Size& sz = Size(0, 0));
  explicit Icon(const String& fileName, const Size& sz = Size(0, 0));
  explicit Icon(HICON handle);
  virtual ~Icon();

  HICON getHandle() const;
};

} // namespace Vaca

#endif // VACA_ICON_H
