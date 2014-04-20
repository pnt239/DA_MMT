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

#ifndef VACA_IMAGE_H
#define VACA_IMAGE_H

#include "Vaca/base.h"
#include "Vaca/ResourceId.h"
#include "Vaca/Size.h"
#include "Vaca/GdiObject.h"
#include "Vaca/SmartPtr.h"

namespace Vaca {

class Graphics;
class Image;

/**
 * Used to destroy the HBITMAP handle of a Image.
 * 
 * @internal
 */
class VACA_DLL ImageHandle : public GdiObject<HBITMAP>
{
  friend class Image;
  HDC m_hdc;
  Graphics* m_graphics;
public:
  ImageHandle();
  ImageHandle(HBITMAP handle);
  virtual ~ImageHandle();
};

/**
 * A smart pointer to an image.
 *
 * This is a SmartPtr, so if you copy instances of images they will be
 * referencing to the same place. You have to use Image#clone method
 * to create real copies of the Image.
 *
 * Example
 * @code
 * Image img1(Size(32, 32));
 * Image img2 = img1;		// img1 and img2 references the same Image
 * Image img3 = img1.clone(); 
 * assert(img == img2);
 * assert(img != img2);
 * @endcode
 *
 * @win32
 *   This is a @msdn{HBITMAP} wrapper.
 * @endwin32
 */
class VACA_DLL Image : private SmartPtr<ImageHandle>
{
public:
  Image();
  explicit Image(ResourceId imageId);
  explicit Image(const String& fileName);
  explicit Image(const Size& sz);
  Image(const Size& sz, int depth);
  Image(const Size& sz, Graphics& g);
  Image(const Image& image);
  virtual ~Image();

  bool isValid() const { return get()->isValid(); }

  int getWidth() const;
  int getHeight() const;
  Size getSize() const;
  int getDepth() const;

  Graphics& getGraphics();

  HBITMAP getHandle() const;

  Image& operator=(const Image& image);

  Image clone() const;

private:

  void copyTo(Image& image) const;
  
};

} // namespace Vaca

#endif // VACA_IMAGE_H
