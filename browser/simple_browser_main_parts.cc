// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/browser/simple_browser_main_parts.h"

#include "base/base_switches.h"
#include "base/bind.h"
#include "base/command_line.h"
#include "base/files/file_path.h"
#include "base/message_loop/message_loop.h"
#include "base/threading/thread.h"
#include "base/threading/thread_restrictions.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/main_function_params.h"
#include "content/public/common/url_constants.h"
#include "content/simple/browser/simple_web_contents_delegate.h"
#include "content/simple/browser/simple_browser_context.h"
#include "content/simple/browser/simple_net_log.h"
//#include "content/simple/common/simple_switches.h"
#include "content/simple/ui/MainFrm.h"
#include "grit/net_resources.h"
#include "net/base/filename_util.h"
#include "net/base/net_module.h"
#include "ui/base/resource/resource_bundle.h"
#include "url/gurl.h"
#include "webkit/browser/quota/quota_manager.h"

namespace content {

  namespace {

    // Default quota for each origin is 5MB.
    const int kDefaultLayoutTestQuotaBytes = 5 * 1024 * 1024;

    GURL GetStartupURL() {
      CommandLine* command_line = CommandLine::ForCurrentProcess();
      const CommandLine::StringVector& args = command_line->GetArgs();

      if (args.empty())
        return GURL("http://www.baidu.com/");

      GURL url(args[0]);
      if (url.is_valid() && url.has_scheme())
        return url;

      return net::FilePathToFileURL(base::FilePath(args[0]));
    }

    base::StringPiece PlatformResourceProvider(int key) {
      if (key == IDR_DIR_HEADER_HTML) {
        base::StringPiece html_data =
          ui::ResourceBundle::GetSharedInstance().GetRawDataResource(
          IDR_DIR_HEADER_HTML);
        return html_data;
      }
      return base::StringPiece();
    }

  }  // namespace

  SimpleBrowserMainParts::SimpleBrowserMainParts(
    const MainFunctionParams& parameters)
    : BrowserMainParts(), parameters_(parameters), run_message_loop_(true) {}

  SimpleBrowserMainParts::~SimpleBrowserMainParts() {
  }


  void SimpleBrowserMainParts::PreMainMessageLoopRun() {
    //net_log_.reset(new SimpleNetLog("content_simple"));
    //browser_context_.reset(new SimpleBrowserContext(false, net_log_.get()));
    //off_the_record_browser_context_.reset(new SimpleBrowserContext(true, net_log_.get()));
    browser_context_.reset(new SimpleBrowserContext(false, NULL));
    off_the_record_browser_context_.reset(new SimpleBrowserContext(true, NULL));
    

    //SimpleWebContentsDelegate::Initialize();
    //net::NetModule::SetResourceProvider(PlatformResourceProvider);

    //SimpleWebContentsDelegate::CreateNewWindow(browser_context_.get(),
    //  GetStartupURL(),
    //  NULL,
    //  MSG_ROUTING_NONE,
    //  gfx::Size());

    main_ui_.reset(new CMainFrame);
    main_ui_->m_browser_main = this;
    main_ui_->CreateEx();
    main_ui_->ShowWindow(SW_SHOWDEFAULT);

    if (parameters_.ui_task) {
      parameters_.ui_task->Run();
      delete parameters_.ui_task;
      run_message_loop_ = false;
    }
  }

  bool SimpleBrowserMainParts::MainMessageLoopRun(int* result_code)  {
    return !run_message_loop_;
  }

  void SimpleBrowserMainParts::PostMainMessageLoopRun() {
    browser_context_.reset();
    off_the_record_browser_context_.reset();
  }

}  // namespace
