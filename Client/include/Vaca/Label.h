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

#ifndef VACA_LABEL_H
#define VACA_LABEL_H

#include "Vaca/base.h"
#include "Vaca/Widget.h"

namespace Vaca {

/**
 * Default style for Label.
 *
 * It is ChildStyle with Win32's SS_NOTIFY (to receive mouse notifications).
 */
#define LabelStyle		(ChildStyle + Style(SS_NOTIFY, 0))

/**
 * A simple label has left-alignment and is not wrapped in multiple lines.
 */
#define SimpleLabelStyle	(Style(SS_SIMPLE, 0))

/**
 * If the label is too long, ellipsis ("...") is used at the end of
 * each word instead of wrap them.
 */
#define WordEllipsisLabelStyle	(Style(SS_WORDELLIPSIS, 0))

/**
 * If the label is too long, ellipsis ("...") is used at the end of
 * the text. The text is not wrapped.
 */
#define EndEllipsisLabelStyle	(Style(SS_ENDELLIPSIS, 0))

/**
 * If the label is too long, ellipsis ("...") is used in the middle of
 * the text. It is useful to show file name paths, because the
 * ellipsis is used trying to show the file name part
 * (String#getFileName). The text is not wrapped.
 */
#define PathEllipsisLabelStyle	(Style(SS_PATHELLIPSIS, 0))

/**
 * A static label of text. It's just an informative label. Remember to
 * pass the LabelStyle to @link Label#Label Label's constructor@endlink
 * if you derived this class.
 *
 * @warning
 *   For people that want to mix up Win32 @c SS_ styles with the ones
 *   in @c LabelStyles, you should be aware to use them because some
 *   can overlap the @c SS_CENTER or @c SS_RIGHT, so you should override
 *   #getTextAlign and #getTextAlign methods (like CustomLabel does).
 */
class VACA_DLL Label : public Widget
{
public:

  Label(const String& text, Widget* parent, Style style = LabelStyle);
  virtual ~Label();

  virtual TextAlign getTextAlign();
  virtual void setTextAlign(TextAlign align);

protected:
  
  bool useWordWrap();
  int getFlagsForDrawString();

  // events
  virtual void onPreferredSize(Size& sz);
  virtual void onResize(const Size& sz);

};

} // namespace Vaca

#endif // VACA_LABEL_H
