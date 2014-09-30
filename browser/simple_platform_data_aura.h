// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_BROWSER_SIMPLE_PLATFORM_DATA_AURA_H_
#define CONTENT_SIMPLE_BROWSER_SIMPLE_PLATFORM_DATA_AURA_H_

#include "base/memory/scoped_ptr.h"
#include "ui/aura/window_tree_host.h"

namespace aura {
namespace client {
class DefaultCaptureClient;
class FocusClient;
class WindowTreeClient;
}
}

namespace gfx {
class Size;
}

namespace ui {
class EventHandler;
}

namespace content {

class SimplePlatformDataAura {
 public:
   explicit SimplePlatformDataAura(const gfx::Size& initial_size);
   ~SimplePlatformDataAura();

  void ShowWindow();
  void ResizeWindow(const gfx::Size& size);

  aura::WindowTreeHost* host() { return host_.get(); }

 private:
  scoped_ptr<aura::WindowTreeHost> host_;
  scoped_ptr<aura::client::FocusClient> focus_client_;
  scoped_ptr<aura::client::DefaultCaptureClient> capture_client_;
  scoped_ptr<aura::client::WindowTreeClient> window_tree_client_;
  scoped_ptr<ui::EventHandler> ime_filter_;

  DISALLOW_COPY_AND_ASSIGN(SimplePlatformDataAura);
};

}  // namespace content

#endif  // CONTENT_SIMPLE_BROWSER_SIMPLE_PLATFORM_DATA_AURA_H_
