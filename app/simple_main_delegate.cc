// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/app/simple_main_delegate.h"

#include "base/base_switches.h"
#include "base/command_line.h"
#include "base/cpu.h"
#include "base/files/file.h"
#include "base/files/file_path.h"
#include "base/lazy_instance.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "cc/base/switches.h"
#include "content/public/browser/browser_main_runner.h"
#include "content/public/common/content_switches.h"
#include "content/public/common/url_constants.h"
#include "content/public/test/layouttest_support.h"
//#include "content/shell/app/shell_breakpad_client.h"
//#include "content/shell/app/webkit_test_platform_support.h"
//#include "content/shell/browser/shell_browser_main.h"
#include "content/simple/browser/simple_content_browser_client.h"
//#include "content/shell/common/shell_switches.h"
//#include "content/shell/renderer/shell_content_renderer_client.h"
#include "net/cookies/cookie_monster.h"
#include "ui/base/resource/resource_bundle.h"
#include "ui/base/ui_base_paths.h"
#include "ui/base/ui_base_switches.h"
#include "ui/events/event_switches.h"
#include "ui/gfx/switches.h"
#include "ui/gl/gl_switches.h"

#include <initguid.h>
#include <windows.h>
#include "base/logging_win.h"
#include "components/breakpad/app/breakpad_win.h"



namespace content {

SimpleMainDelegate::SimpleMainDelegate() {
}

SimpleMainDelegate::~SimpleMainDelegate() {
}

//bool SimpleMainDelegate::BasicStartupComplete(int* exit_code) {
//  SetContentClient(&content_client_);
//  return false;
//}

//void SimpleMainDelegate::PreSandboxStartup() {
//  InitializeResourceBundle();
//}

//int SimpleMainDelegate::RunProcess(
//    const std::string& process_type,
//    const MainFunctionParams& main_function_params) {
//  if (!process_type.empty())
//    return -1;
//
//  // Android stores the BrowserMainRunner instance as a scoped member pointer
//  // on the SimpleMainDelegate class because of different object lifetime.
//  scoped_ptr<BrowserMainRunner> browser_runner_;
//  browser_runner_.reset(BrowserMainRunner::Create());
//  return SimpleBrowserMain(main_function_params, browser_runner_);
//}


//void SimpleMainDelegate::InitializeResourceBundle() {
//  base::FilePath pak_file;
//  base::FilePath pak_dir;
//  PathService::Get(base::DIR_MODULE, &pak_dir);
//
//  pak_file = pak_dir.Append(FILE_PATH_LITERAL("content_simple.pak"));
//  ui::ResourceBundle::InitSharedInstanceWithPakPath(pak_file);
//}

ContentBrowserClient* SimpleMainDelegate::CreateContentBrowserClient() {
  browser_client_.reset(new SimpleContentBrowserClient);
  return browser_client_.get();
}

//ContentRendererClient* SimpleMainDelegate::CreateContentRendererClient() {
//  renderer_client_.reset(new SimpleContentRendererClient);
//  return renderer_client_.get();
//}

}  // namespace content
