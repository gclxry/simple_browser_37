// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_BROWSER_SIMPLE_BROWSER_CONTEXT_H_
#define CONTENT_SIMPLE_BROWSER_SIMPLE_BROWSER_CONTEXT_H_

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/browser_context.h"
#include "content/public/browser/content_browser_client.h"
#include "net/url_request/url_request_job_factory.h"

namespace net {
  class NetLog;
}

namespace content {

  class DownloadManagerDelegate;
  class ResourceContext;
  class SimpleURLRequestContextGetter;

  class SimpleBrowserContext : public BrowserContext {
  public:
    SimpleBrowserContext(bool off_the_record, net::NetLog* net_log);
    virtual ~SimpleBrowserContext();

    //void set_guest_manager_for_testing(
    //  BrowserPluginGuestManager* guest_manager) {
    //  guest_manager_ = guest_manager;
    //}

    // BrowserContext implementation.
    virtual base::FilePath GetPath() const OVERRIDE;
    virtual bool IsOffTheRecord() const OVERRIDE;
    virtual DownloadManagerDelegate* GetDownloadManagerDelegate() OVERRIDE;
    virtual net::URLRequestContextGetter* GetRequestContext() OVERRIDE;
    virtual net::URLRequestContextGetter* GetRequestContextForRenderProcess(
      int renderer_child_id) OVERRIDE;
    virtual net::URLRequestContextGetter* GetMediaRequestContext() OVERRIDE;
    virtual net::URLRequestContextGetter* GetMediaRequestContextForRenderProcess(
      int renderer_child_id) OVERRIDE;
    virtual net::URLRequestContextGetter*
      GetMediaRequestContextForStoragePartition(
      const base::FilePath& partition_path,
      bool in_memory) OVERRIDE;
    virtual ResourceContext* GetResourceContext() OVERRIDE;
    virtual BrowserPluginGuestManager* GetGuestManager() OVERRIDE;
    virtual quota::SpecialStoragePolicy* GetSpecialStoragePolicy() OVERRIDE;
    virtual PushMessagingService* GetPushMessagingService() OVERRIDE;

    net::URLRequestContextGetter* CreateRequestContext(
      ProtocolHandlerMap* protocol_handlers,
      URLRequestInterceptorScopedVector request_interceptors);
    net::URLRequestContextGetter* CreateRequestContextForStoragePartition(
      const base::FilePath& partition_path,
      bool in_memory,
      ProtocolHandlerMap* protocol_handlers,
      URLRequestInterceptorScopedVector request_interceptors);

  private:
    class SimpleResourceContext;

    // Performs initialization of the ShellBrowserContext while IO is still
    // allowed on the current thread.
    void InitWhileIOAllowed();

    bool off_the_record_;
    net::NetLog* net_log_;
    bool ignore_certificate_errors_;
    base::FilePath path_;
    BrowserPluginGuestManager* guest_manager_;
    scoped_ptr<SimpleResourceContext> resource_context_;
    scoped_refptr<SimpleURLRequestContextGetter> url_request_getter_;

    DISALLOW_COPY_AND_ASSIGN(SimpleBrowserContext);
  };

}  // namespace content

#endif  // CONTENT_SIMPLE_BROWSER_SIMPLE_BROWSER_CONTEXT_H_
