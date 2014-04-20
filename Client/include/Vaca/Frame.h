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

#ifndef VACA_FRAME_H
#define VACA_FRAME_H

#include <vector>

#include "Vaca/base.h"
#include "Vaca/Widget.h"
#include "Vaca/WidgetClass.h"
#include "Vaca/Register.h"
#include "Vaca/DockArea.h"
#include "Vaca/Icon.h"

namespace Vaca {

class CloseEvent;
class Menu;
class MenuBar;
class MenuItem;
class StatusBar;
class Thread;

#define WithCaptionFrameStyle		(Style(WS_CAPTION, 0))

#define WithSystemMenuFrameStyle	(Style(WS_SYSMENU, 0))

#define MinimizableFrameStyle		(Style(WS_MINIMIZEBOX, 0))

#define MaximizableFrameStyle		(Style(WS_MAXIMIZEBOX, 0))

#define ResizableFrameStyle		(Style(WS_SIZEBOX, 0))

#define InitiallyMinimizedFrameStyle	(Style(WS_MINIMIZE, 0))

#define InitiallyMaximizedFrameStyle	(Style(WS_MAXIMIZE, 0))

#define FrameStyle			(ContainerStyle +		\
					 WithCaptionFrameStyle +	\
					 WithSystemMenuFrameStyle +	\
					 MinimizableFrameStyle +	\
					 MaximizableFrameStyle +	\
					 ResizableFrameStyle)

/**
 * Represents the Win32 class used by Frame.
 */
class FrameClass : public WidgetClass
{
public:
  static WidgetClassName getClassName()
  { return WidgetClassName("Vaca.Frame"); }
};

/**
 * A window with title-bar that can be minimized, maximized, etc.
 */
class VACA_DLL Frame : public Register<FrameClass>, public Widget
{
  MenuBar* m_menuBar;		       // The menu bar.
  StatusBar* m_statusBar;	       // The status bar.
  std::vector<DockArea*> m_dockAreas;  // Areas where you can dock a tool-bar.
  bool m_counted;		       // true if this Frame is counted in the frames' list
  Icon m_smallIcon;
  Icon m_bigIcon;

public:

  Frame(const String& title, Widget* parent = NULL, Style style = FrameStyle);
  Frame(const WidgetClassName& className, const String& title, Widget* parent = NULL, Style style = FrameStyle);
  virtual ~Frame();

  virtual void setVisible(bool visible);

  MenuBar* getMenuBar();
  virtual MenuBar* setMenuBar(MenuBar* menuBar);

  void setSmallIcon(const Icon& icon);
  void setBigIcon(const Icon& icon);
  void setIcon(ResourceId iconId);

  Size getNonClientSize();
  virtual Rect getLayoutBounds();
  
  void addDockArea(DockArea* dockArea);
  void removeDockArea(DockArea* dockArea);
  void defaultDockAreas();
  void deleteDockAreas();
  std::vector<DockArea*> getDockAreas();
  DockArea* getDockArea(Side side);
  virtual DockArea* getDefaultDockArea();

  virtual void layout();
  virtual bool isLayoutFree();
  virtual bool keepSynchronized();

  Signal1<void, Event&> Activate;     ///< @see onActivate
  Signal1<void, Event&> Deactivate;   ///< @see onDeactivate
  Signal1<void, CloseEvent&> Close;   ///< @see onClose
  Signal2<void, CardinalDirection, Rect&> Resizing; ///< @see onResizing

  virtual bool preTranslateMessage(Message& msg);

protected:
  // events
  virtual void onPreferredSize(Size& sz);
  virtual void onResize(const Size& sz);
  virtual bool onCommand(CommandId id);
  virtual void onUpdateIndicators();
  virtual void onRemoveChild(Widget* child);

  // new events
  virtual void onActivate(Event& ev);
  virtual void onDeactivate(Event& ev);
  virtual void onClose(CloseEvent& ev);
  virtual void onResizing(CardinalDirection dir, Rect& rc);

  virtual bool wndProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

private:

  void initialize(const String& title);
  void updateMenuItem(MenuItem* menuItem);
  Container getSynchronizedGroup();
  
};

} // namespace Vaca

#endif // VACA_FRAME_H
