///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2020, Liberty Software LLC
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include <tcl.h>

#include <initializer_list>
#include <set>
#include <variant>
#include <tuple>

#include "opendb/db.h"

namespace gui {

// An object selected in the gui.  Monostate is for a null value.
using Selected = std::variant<std::monostate, odb::dbInst*, odb::dbNet*>;

// A collection of selected objects
using SelectionSet = std::set<Selected>;

// This is an API that the Renderer instances will use to do their
// rendering.  This is subclassed in the gui module and hides Qt from
// the clients.  Clients will only deal with this API and not Qt itself,
// which contains the Qt dependencies to the gui module.
class Painter
{
 public:
  struct Color
  {
    constexpr Color() : r(0), g(0), b(0), a(255) {}
    constexpr Color(int r, int g, int b, int a = 255) : r(r), g(g), b(b), a(a)
    {
    }

    int r;
    int g;
    int b;
    int a;
  };

  static inline const Color black{0x00, 0x00, 0x00, 0xff};
  static inline const Color white{0xff, 0xff, 0xff, 0xff};
  static inline const Color dark_gray{0x80, 0x80, 0x80, 0xff};
  static inline const Color gray{0xa0, 0xa0, 0xa4, 0xff};
  static inline const Color light_gray{0xc0, 0xc0, 0xc0, 0xff};
  static inline const Color red{0xff, 0x00, 0x00, 0xff};
  static inline const Color green{0x00, 0xff, 0x00, 0xff};
  static inline const Color blue{0x00, 0x00, 0xff, 0xff};
  static inline const Color cyan{0x00, 0xff, 0xff, 0xff};
  static inline const Color magenta{0xff, 0x00, 0xff, 0xff};
  static inline const Color yellow{0xff, 0xff, 0x00, 0xff};
  static inline const Color dark_red{0x80, 0x00, 0x00, 0xff};
  static inline const Color dark_green{0x00, 0x80, 0x00, 0xff};
  static inline const Color dark_blue{0x00, 0x00, 0x80, 0xff};
  static inline const Color dark_cyan{0x00, 0x80, 0x80, 0xff};
  static inline const Color dark_magenta{0x80, 0x00, 0x80, 0xff};
  static inline const Color dark_yellow{0x80, 0x80, 0x00, 0xff};
  static inline const Color transparent{0x00, 0x00, 0x00, 0x00};

  virtual ~Painter() = default;

  // Set the pen to whatever the user has chosen for this layer
  virtual void setPen(odb::dbTechLayer* layer, bool cosmetic = false) = 0;

  // Set the pen to an RGBA value
  virtual void setPen(const Color& color, bool cosmetic = false) = 0;

  // Set the brush to whatever the user has chosen for this layer
  virtual void setBrush(odb::dbTechLayer* layer) = 0;

  // Set the brush to whatever the user has chosen for this layer
  virtual void setBrush(const Color& color) = 0;

  // Draw a rect with coordinates in DBU with the current pen/brush
  virtual void drawRect(const odb::Rect& rect) = 0;

  // Draw a line with coordinates in DBU with the current pen
  virtual void drawLine(const odb::Point& p1, const odb::Point& p2) = 0;

  // Draw a line with coordinates in DBU with the current pen
  void drawLine(int xl, int yl, int xh, int yh)
  {
    drawLine(odb::Point(xl, yl), odb::Point(xh, yh));
  }
};

// This is an interface for classes that wish to be called to render
// on the layout.  Clients will subclass and register their instances with
// the Gui singleton.
class Renderer
{
 public:
  // Automatically unregisters this renderable
  virtual ~Renderer();

  // Draw on top of the particular layer.
  virtual void drawLayer(odb::dbTechLayer* /* layer */, Painter& /* painter */)
  {
  }

  // Draw on top of the layout in general after the layers
  // have been drawn.
  virtual void drawObjects(Painter& /* painter */) {}
};

// This is the API for the rest of the program to interact with the
// GUI.  This class is accessed by the GUI implementation to interact
// with the rest of the system.  This class itself doesn't hold the
// GUI implementation.  This class is a singleton.
class Gui
{
 public:
  // Registered renderers must remain valid until they are
  // unregistered.
  void register_renderer(Renderer* renderer);
  void unregister_renderer(Renderer* renderer);

  // Force an immediate redraw.
  void redraw();

  // Waits for the user to click continue before returning
  // Draw events are processed while paused.
  void pause();

  const std::set<Renderer*>& renderers() { return renderers_; }

  static Gui* get();

 private:
  std::set<Renderer*> renderers_;
  static Gui* singleton_;
};

// The main entry point
int start_gui(int argc, char* argv[]);

}  // namespace gui