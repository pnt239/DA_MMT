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

#ifndef VACA_SLIDER_H
#define VACA_SLIDER_H

#include "Vaca/base.h"
#include "Vaca/Widget.h"

namespace Vaca {

#define SliderStyle		(ChildStyle +				\
				 FocusableStyle +			\
				 Style(TBS_HORZ | TBS_BOTH |		\
				       TBS_NOTICKS | TBS_AUTOTICKS, 0))

class VACA_DLL Slider : public Widget
{
public:

  static const int MinLimit;
  static const int MaxLimit;

  Slider(Widget* parent, Style style = SliderStyle);
  Slider(int minValue, int maxValue, int value,
	 Widget* parent, Style style = SliderStyle);
  virtual ~Slider();

  Orientation getOrientation();
  void setOrientation(Orientation orientation);

  Sides getSides();
  void setSides(Sides sides);

  void setTickVisible(bool state);
  void setTickFreq(int freq);

  int getMinValue();
  int getMaxValue();
  void getRange(int& minValue, int& maxValue);
  void setRange(int minValue, int maxValue);

  int getValue();
  void setValue(int posValue);

  int getLineSize();
  void setLineSize(int lineSize);

  int getPageSize();
  void setPageSize(int pageSize);

  // signals
  Signal1<void, Event&> Change; ///< @see onChange

protected:
  // events
  virtual void onPreferredSize(Size& sz);
  virtual void onScroll(Orientation orientation, int code);
  
  // new events
  virtual void onChange(Event& ev);
  
};

} // namespace Vaca

#endif // VACA_SLIDER_H
