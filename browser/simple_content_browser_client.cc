// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/browser/simple_content_browser_client.h"

#include "base/base_switches.h"
#include "base/command_line.h"
#include "base/file_util.h"
#include "base/files/file.h"
#include "base/path_service.h"
#include "base/strings/utf_string_conversions.h"
#include "content/public/browser/render_process_host.h"
#include "content/public/browser/resource_dispatcher_host.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
//#include "content/public/common/url_constants.h"
//#include "content/shell/browser/shell.h"
//#include "content/shell/browser/shell_browser_context.h"
//#include "content/shell/browser/shell_browser_main_parts.h"
//#include "content/shell/browser/shell_devtools_delegate.h"
//#include "content/shell/browser/shell_message_filter.h"
//#include "content/shell/browser/shell_net_log.h"
//#include "content/shell/browser/shell_quota_permission_context.h"
//#include "content/shell/browser/shell_resource_dispatcher_host_delegate.h"
//#include "content/shell/browser/shell_web_contents_view_delegate_creator.h"
//#include "content/shell/browser/webkit_test_controller.h"
//#include "content/shell/common/shell_messages.h"
//#include "content/shell/common/shell_switches.h"
//#include "content/shell/common/webkit_test_helpers.h"
//#include "content/shell/geolocation/shell_access_token_store.h"
#include "net/url_request/url_request_context_getter.h"
#include "url/gurl.h"
#include "webkit/common/webpreferences.h"

#include "content/common/sandbox_win.h"
#include "sandbox/win/src/sandbox.h"


namespace content {

namespace {

SimpleContentBrowserClient* g_browser_client;
bool g_swap_processes_for_redirect = false;


}  // namespace

//SimpleContentBrowserClient* SimpleContentBrowserClient::Get() {
//  return g_browser_client;
//}
//
//void SimpleContentBrowserClient::SetSwapProcessesForRedirect(bool swap) {
//  g_swap_processes_for_redirect = swap;
//}

SimpleContentBrowserClient::SimpleContentBrowserClient(){
  DCHECK(!g_browser_client);
  g_browser_client = this;
}

SimpleContentBrowserClient::~SimpleContentBrowserClient() {
  g_browser_client = NULL;
}

//BrowserMainParts* SimpleContentBrowserClient::CreateBrowserMainParts(
//    const MainFunctionParams& parameters) {
//  shell_browser_main_parts_ = new ShellBrowserMainParts(parameters);
//  return shell_browser_main_parts_;
//}

//void SimpleContentBrowserClient::RenderProcessWillLaunch(
//    RenderProcessHost* host) {
//}
//
//net::URLRequestContextGetter* SimpleContentBrowserClient::CreateRequestContext(
//    BrowserContext* content_browser_context,
//    ProtocolHandlerMap* protocol_handlers,
//    URLRequestInterceptorScopedVector request_interceptors) {
//  ShellBrowserContext* shell_browser_context =
//      ShellBrowserContextForBrowserContext(content_browser_context);
//  return shell_browser_context->CreateRequestContext(
//      protocol_handlers, request_interceptors.Pass());
//}
//
//net::URLRequestContextGetter*
//SimpleContentBrowserClient::CreateRequestContextForStoragePartition(
//    BrowserContext* content_browser_context,
//    const base::FilePath& partition_path,
//    bool in_memory,
//    ProtocolHandlerMap* protocol_handlers,
//    URLRequestInterceptorScopedVector request_interceptors) {
//  ShellBrowserContext* shell_browser_context =
//      ShellBrowserContextForBrowserContext(content_browser_context);
//  return shell_browser_context->CreateRequestContextForStoragePartition(
//      partition_path,
//      in_memory,
//      protocol_handlers,
//      request_interceptors.Pass());
//}
//
//bool SimpleContentBrowserClient::IsHandledURL(const GURL& url) {
//  if (!url.is_valid())
//    return false;
//  DCHECK_EQ(url.scheme(), StringToLowerASCII(url.scheme()));
//  // Keep in sync with ProtocolHandlers added by
//  // ShellURLRequestContextGetter::GetURLRequestContext().
//  static const char* const kProtocolList[] = {
//      url::kBlobScheme,
//      url::kFileSystemScheme,
//      kChromeUIScheme,
//      kChromeDevToolsScheme,
//      url::kDataScheme,
//      url::kFileScheme,
//  };
//  for (size_t i = 0; i < arraysize(kProtocolList); ++i) {
//    if (url.scheme() == kProtocolList[i])
//      return true;
//  }
//  return false;
//}
//
//void SimpleContentBrowserClient::AppendExtraCommandLineSwitches(
//    CommandLine* command_line, int child_process_id) {
//  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kDumpRenderTree))
//    command_line->AppendSwitch(switches::kDumpRenderTree);
//  if (CommandLine::ForCurrentProcess()->HasSwitch(
//      switches::kEnableFontAntialiasing))
//    command_line->AppendSwitch(switches::kEnableFontAntialiasing);
//  if (CommandLine::ForCurrentProcess()->HasSwitch(
//      switches::kExposeInternalsForTesting))
//    command_line->AppendSwitch(switches::kExposeInternalsForTesting);
//  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kStableReleaseMode))
//    command_line->AppendSwitch(switches::kStableReleaseMode);
//  if (CommandLine::ForCurrentProcess()->HasSwitch(
//          switches::kEnableCrashReporter)) {
//    command_line->AppendSwitch(switches::kEnableCrashReporter);
//  }
//  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kCrashDumpsDir)) {
//    command_line->AppendSwitchPath(
//        switches::kCrashDumpsDir,
//        CommandLine::ForCurrentProcess()->GetSwitchValuePath(
//            switches::kCrashDumpsDir));
//  }
//  if (CommandLine::ForCurrentProcess()->HasSwitch(
//          switches::kEnableLeakDetection))
//    command_line->AppendSwitch(switches::kEnableLeakDetection);
//  if (CommandLine::ForCurrentProcess()->HasSwitch(
//        switches::kRegisterFontFiles)) {
//    command_line->AppendSwitchASCII(
//        switches::kRegisterFontFiles,
//        CommandLine::ForCurrentProcess()->GetSwitchValueASCII(
//            switches::kRegisterFontFiles));
//  }
//}
//
//void SimpleContentBrowserClient::OverrideWebkitPrefs(
//    RenderViewHost* render_view_host,
//    const GURL& url,
//    WebPreferences* prefs) {
//  if (!CommandLine::ForCurrentProcess()->HasSwitch(switches::kDumpRenderTree))
//    return;
//  WebKitTestController::Get()->OverrideWebkitPrefs(prefs);
//}
//
//void SimpleContentBrowserClient::ResourceDispatcherHostCreated() {
//  resource_dispatcher_host_delegate_.reset(
//      new ShellResourceDispatcherHostDelegate());
//  ResourceDispatcherHost::Get()->SetDelegate(
//      resource_dispatcher_host_delegate_.get());
//}
//
//std::string SimpleContentBrowserClient::GetDefaultDownloadName() {
//  return "download";
//}
//
//WebContentsViewDelegate* SimpleContentBrowserClient::GetWebContentsViewDelegate(
//    WebContents* web_contents) {
//#if !defined(USE_AURA)
//  return CreateShellWebContentsViewDelegate(web_contents);
//#else
//  return NULL;
//#endif
//}
//
//QuotaPermissionContext*
//SimpleContentBrowserClient::CreateQuotaPermissionContext() {
//  return new ShellQuotaPermissionContext();
//}
//
//SpeechRecognitionManagerDelegate*
//    SimpleContentBrowserClient::GetSpeechRecognitionManagerDelegate() {
//  return new ShellSpeechRecognitionManagerDelegate();
//}
//
//net::NetLog* SimpleContentBrowserClient::GetNetLog() {
//  return shell_browser_main_parts_->net_log();
//}
//
//bool SimpleContentBrowserClient::ShouldSwapProcessesForRedirect(
//    ResourceContext* resource_context,
//    const GURL& current_url,
//    const GURL& new_url) {
//  return g_swap_processes_for_redirect;
//}
//
//
//
//void SimpleContentBrowserClient::PreSpawnRenderer(sandbox::TargetPolicy* policy,
//                                                 bool* success) {
//  // Add sideloaded font files for testing. See also DIR_WINDOWS_FONTS
//  // addition in |StartSandboxedProcess|.
//  std::vector<std::string> font_files = GetSideloadFontFiles();
//  for (std::vector<std::string>::const_iterator i(font_files.begin());
//      i != font_files.end();
//      ++i) {
//    policy->AddRule(sandbox::TargetPolicy::SUBSYS_FILES,
//        sandbox::TargetPolicy::FILES_ALLOW_READONLY,
//        base::UTF8ToWide(*i).c_str());
//  }
//}
//
//
//ShellBrowserContext* SimpleContentBrowserClient::browser_context() {
//  return shell_browser_main_parts_->browser_context();
//}
//
//ShellBrowserContext*
//    SimpleContentBrowserClient::off_the_record_browser_context() {
//  return shell_browser_main_parts_->off_the_record_browser_context();
//}
//
//AccessTokenStore* SimpleContentBrowserClient::CreateAccessTokenStore() {
//  return new ShellAccessTokenStore(browser_context());
//}
//
//ShellBrowserContext*
//SimpleContentBrowserClient::ShellBrowserContextForBrowserContext(
//    BrowserContext* content_browser_context) {
//  if (content_browser_context == browser_context())
//    return browser_context();
//  DCHECK_EQ(content_browser_context, off_the_record_browser_context());
//  return off_the_record_browser_context();
//}

}  // namespace content
