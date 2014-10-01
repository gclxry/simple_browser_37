// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/browser/simple_browser_context.h"

#include "base/bind.h"
#include "base/command_line.h"
#include "base/environment.h"
#include "base/file_util.h"
#include "base/logging.h"
#include "base/path_service.h"
#include "base/threading/thread.h"
#include "content/public/browser/browser_thread.h"
#include "content/public/browser/resource_context.h"
#include "content/public/browser/storage_partition.h"
#include "content/public/common/content_switches.h"
#include "content/simple/browser/simple_url_request_context_getter.h"
//#include "content/shell/common/shell_switches.h"

#include "base/base_paths_win.h"


namespace content {

  class SimpleBrowserContext::SimpleResourceContext : public ResourceContext{
  public:
    SimpleResourceContext() : getter_(NULL) {}
    virtual ~SimpleResourceContext() {}

    // ResourceContext implementation:
    virtual net::HostResolver* GetHostResolver() OVERRIDE{
      CHECK(getter_);
      return getter_->host_resolver();
    }
    virtual net::URLRequestContext* GetRequestContext() OVERRIDE{
      CHECK(getter_);
      return getter_->GetURLRequestContext();
    }
    virtual bool AllowMicAccess(const GURL& origin) OVERRIDE{
      return false;
    }
    virtual bool AllowCameraAccess(const GURL& origin) OVERRIDE{
      return false;
    }

    void set_url_request_context_getter(SimpleURLRequestContextGetter* getter) {
      getter_ = getter;
    }

  private:
    SimpleURLRequestContextGetter* getter_;

    DISALLOW_COPY_AND_ASSIGN(SimpleResourceContext);
  };

  SimpleBrowserContext::SimpleBrowserContext(bool off_the_record,
    net::NetLog* net_log)
    : off_the_record_(off_the_record),
    net_log_(net_log),
    ignore_certificate_errors_(false),
    guest_manager_(NULL),
    resource_context_(new SimpleResourceContext) {
    InitWhileIOAllowed();
  }

  SimpleBrowserContext::~SimpleBrowserContext() {
    if (resource_context_) {
      BrowserThread::DeleteSoon(
        BrowserThread::IO, FROM_HERE, resource_context_.release());
    }
  }

  void SimpleBrowserContext::InitWhileIOAllowed() {
    CommandLine* cmd_line = CommandLine::ForCurrentProcess();

    CHECK(PathService::Get(base::DIR_LOCAL_APP_DATA, &path_));
    path_ = path_.Append(std::wstring(L"content_simple"));

    if (!base::PathExists(path_))
      base::CreateDirectory(path_);
  }

  base::FilePath SimpleBrowserContext::GetPath() const {
    return path_;
  }

  bool SimpleBrowserContext::IsOffTheRecord() const {
    return off_the_record_;
  }

  DownloadManagerDelegate* SimpleBrowserContext::GetDownloadManagerDelegate()  {
    //DownloadManager* manager = BrowserContext::GetDownloadManager(this);

    //if (!download_manager_delegate_.get()) {
    //  download_manager_delegate_.reset(new ShellDownloadManagerDelegate());
    //  download_manager_delegate_->SetDownloadManager(manager);
    //  CommandLine* cmd_line = CommandLine::ForCurrentProcess();
    //  if (cmd_line->HasSwitch(switches::kDumpRenderTree)) {
    //    download_manager_delegate_->SetDownloadBehaviorForTesting(
    //      path_.Append(FILE_PATH_LITERAL("downloads")));
    //  }
    //}

    //return download_manager_delegate_.get();
    return NULL;
  }


  net::URLRequestContextGetter* SimpleBrowserContext::GetRequestContext()  {
    return GetDefaultStoragePartition(this)->GetURLRequestContext();
  }

  net::URLRequestContextGetter* SimpleBrowserContext::CreateRequestContext(
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
    DCHECK(!url_request_getter_.get());
    url_request_getter_ = new SimpleURLRequestContextGetter(
      ignore_certificate_errors_,
      GetPath(),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::IO),
      BrowserThread::UnsafeGetMessageLoopForThread(BrowserThread::FILE),
      protocol_handlers,
      request_interceptors.Pass(),
      net_log_);
    resource_context_->set_url_request_context_getter(url_request_getter_.get());
    return url_request_getter_.get();
  }

  net::URLRequestContextGetter*
    SimpleBrowserContext::GetRequestContextForRenderProcess(
    int renderer_child_id)  {
      return GetRequestContext();
    }

  net::URLRequestContextGetter*
    SimpleBrowserContext::GetMediaRequestContext()  {
      return GetRequestContext();
    }

  net::URLRequestContextGetter*
    SimpleBrowserContext::GetMediaRequestContextForRenderProcess(
    int renderer_child_id)  {
      return GetRequestContext();
    }

  net::URLRequestContextGetter*
    SimpleBrowserContext::GetMediaRequestContextForStoragePartition(
    const base::FilePath& partition_path,
    bool in_memory) {
      return GetRequestContext();
    }

  net::URLRequestContextGetter*
    SimpleBrowserContext::CreateRequestContextForStoragePartition(
    const base::FilePath& partition_path,
    bool in_memory,
    ProtocolHandlerMap* protocol_handlers,
    URLRequestInterceptorScopedVector request_interceptors) {
      return NULL;
    }

  ResourceContext* SimpleBrowserContext::GetResourceContext()  {
    return resource_context_.get();
  }

  BrowserPluginGuestManager* SimpleBrowserContext::GetGuestManager() {
    return guest_manager_;
  }

  quota::SpecialStoragePolicy* SimpleBrowserContext::GetSpecialStoragePolicy() {
    return NULL;
  }

  PushMessagingService* SimpleBrowserContext::GetPushMessagingService() {
    return NULL;
  }

}  // namespace content
