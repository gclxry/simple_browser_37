// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_BROWSER_SIMPLE_CONTENT_BROWSER_CLIENT_H_
#define CONTENT_SIMPLE_BROWSER_SIMPLE_CONTENT_BROWSER_CLIENT_H_

#include <string>

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/content_browser_client.h"

namespace content {

  class SimpleBrowserContext;
  class SimpleBrowserMainParts;
//class ShellResourceDispatcherHostDelegate;

class SimpleContentBrowserClient : public ContentBrowserClient {
 public:
  // Gets the current instance.
  //static SimpleContentBrowserClient* Get();

  //static void SetSwapProcessesForRedirect(bool swap);

  SimpleContentBrowserClient();
  virtual ~SimpleContentBrowserClient();

  // ContentBrowserClient overrides.
  virtual BrowserMainParts* CreateBrowserMainParts(
      const MainFunctionParams& parameters) OVERRIDE;
  //virtual void RenderProcessWillLaunch(RenderProcessHost* host) OVERRIDE;
  virtual net::URLRequestContextGetter* CreateRequestContext(
    BrowserContext* browser_context,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) OVERRIDE;
  virtual net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
    BrowserContext* browser_context,
    const base::FilePath& partition_path,
    bool in_memory,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) OVERRIDE;
  //virtual bool IsHandledURL(const GURL& url) OVERRIDE;
  //virtual void AppendExtraCommandLineSwitches(base::CommandLine* command_line,
  //                                            int child_process_id) OVERRIDE;
  //virtual void OverrideWebkitPrefs(RenderViewHost* render_view_host,
  //                                 const GURL& url,
  //                                 WebPreferences* prefs) OVERRIDE;
  //virtual void ResourceDispatcherHostCreated() OVERRIDE;
  //virtual AccessTokenStore* CreateAccessTokenStore() OVERRIDE;
  //virtual std::string GetDefaultDownloadName() OVERRIDE;
  //virtual WebContentsViewDelegate* GetWebContentsViewDelegate(
  //    WebContents* web_contents) OVERRIDE;
  //virtual QuotaPermissionContext* CreateQuotaPermissionContext() OVERRIDE;
  //virtual SpeechRecognitionManagerDelegate*
  //    GetSpeechRecognitionManagerDelegate() OVERRIDE;
  virtual net::NetLog* GetNetLog() OVERRIDE;
  //virtual bool ShouldSwapProcessesForRedirect(ResourceContext* resource_context,
  //                                            const GURL& current_url,
  //                                            const GURL& new_url) OVERRIDE;


  //virtual void PreSpawnRenderer(sandbox::TargetPolicy* policy,
  //                              bool* success) OVERRIDE;

  SimpleBrowserContext* browser_context();
  SimpleBrowserContext* off_the_record_browser_context();
  //ShellResourceDispatcherHostDelegate* resource_dispatcher_host_delegate() {
  //  return resource_dispatcher_host_delegate_.get();
  //}
  SimpleBrowserMainParts* simple_browser_main_parts() {
    return simple_browser_main_parts_;
  }

 private:
   SimpleBrowserContext* SimpleBrowserContextForBrowserContext(
     BrowserContext* content_browser_context);

   //scoped_ptr<ShellResourceDispatcherHostDelegate>
   //  resource_dispatcher_host_delegate_;

   base::FilePath webkit_source_dir_;

   SimpleBrowserMainParts* simple_browser_main_parts_;
};

}  // namespace content

#endif  // CONTENT_SIMPLE_BROWSER_SIMPLE_CONTENT_BROWSER_CLIENT_H_
