// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_BROWSER_SIMPLE_URL_REQUEST_CONTEXT_GETTER_H_
#define CONTENT_SIMPLE_BROWSER_SIMPLE_URL_REQUEST_CONTEXT_GETTER_H_

#include "base/compiler_specific.h"
#include "base/files/file_path.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "content/public/browser/content_browser_client.h"
#include "net/url_request/url_request_context_getter.h"
#include "net/url_request/url_request_job_factory.h"

namespace base {
  class MessageLoop;
}

namespace net {
  class HostResolver;
  class MappedHostResolver;
  class NetworkDelegate;
  class NetLog;
  class ProxyConfigService;
  class URLRequestContextStorage;
}

namespace content {

  class SimpleURLRequestContextGetter : public net::URLRequestContextGetter {
  public:
    SimpleURLRequestContextGetter(
      bool ignore_certificate_errors,
      const base::FilePath& base_path,
      base::MessageLoop* io_loop,
      base::MessageLoop* file_loop,
      ProtocolHandlerMap* protocol_handlers,
      URLRequestInterceptorScopedVector request_interceptors,
      net::NetLog* net_log);

    // net::URLRequestContextGetter implementation.
    virtual net::URLRequestContext* GetURLRequestContext() OVERRIDE;
    virtual scoped_refptr<base::SingleThreadTaskRunner>
      GetNetworkTaskRunner() const OVERRIDE;

    net::HostResolver* host_resolver();

  protected:
    virtual ~SimpleURLRequestContextGetter();

  private:
    bool ignore_certificate_errors_;
    base::FilePath base_path_;
    base::MessageLoop* io_loop_;
    base::MessageLoop* file_loop_;
    net::NetLog* net_log_;

    scoped_ptr<net::ProxyConfigService> proxy_config_service_;
    scoped_ptr<net::NetworkDelegate> network_delegate_;
    scoped_ptr<net::URLRequestContextStorage> storage_;
    scoped_ptr<net::URLRequestContext> url_request_context_;
    ProtocolHandlerMap protocol_handlers_;
    URLRequestInterceptorScopedVector request_interceptors_;

    DISALLOW_COPY_AND_ASSIGN(SimpleURLRequestContextGetter);
  };

}  // namespace content

#endif  // CONTENT_SIMPLE_BROWSER_SIMPLE_URL_REQUEST_CONTEXT_GETTER_H_
