// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_BROWSER_SIMPLE_BROWSER_MAIN_PARTS_H_
#define CONTENT_SIMPLE_BROWSER_SIMPLE_BROWSER_MAIN_PARTS_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_main_parts.h"
#include "content/public/common/main_function_params.h"


namespace base {
  class Thread;
}

namespace net {
  class NetLog;
}

class CMainFrame;

namespace content {

  class SimpleBrowserContext;
  //class SimpleDevToolsDelegate;
  //class SimplePluginServiceFilter;

  class SimpleBrowserMainParts : public BrowserMainParts {
  public:
    explicit SimpleBrowserMainParts(const MainFunctionParams& parameters);
    virtual ~SimpleBrowserMainParts();

    // BrowserMainParts overrides.
    //virtual void PreEarlyInitialization() OVERRIDE;
    //virtual void PreMainMessageLoopStart() OVERRIDE;
    //virtual void PostMainMessageLoopStart() OVERRIDE;
    virtual void PreMainMessageLoopRun() OVERRIDE;
    virtual bool MainMessageLoopRun(int* result_code) OVERRIDE;
    virtual void PostMainMessageLoopRun() OVERRIDE;


    SimpleBrowserContext* browser_context() { return browser_context_.get(); }
    SimpleBrowserContext* off_the_record_browser_context() {
      return off_the_record_browser_context_.get();
    }

    net::NetLog* net_log() { return net_log_.get(); }

    scoped_ptr<SimpleBrowserContext> browser_context_;
  private:
    scoped_ptr<net::NetLog> net_log_;
    
    scoped_ptr<SimpleBrowserContext> off_the_record_browser_context_;
    scoped_ptr<CMainFrame> main_ui_;

    // For running content_browsertests.
    const MainFunctionParams parameters_;
    bool run_message_loop_;
    DISALLOW_COPY_AND_ASSIGN(SimpleBrowserMainParts);
  };

}  // namespace content

#endif  // CONTENT_SIMPLE_BROWSER_SIMPLE_BROWSER_MAIN_PARTS_H_
