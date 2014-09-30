// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_SIMPLE_BROWSER_SIMPLE_NET_LOG_H_
#define CONTENT_SIMPLE_BROWSER_SIMPLE_NET_LOG_H_

#include <string>

#include "base/memory/scoped_ptr.h"
#include "net/base/net_log_logger.h"

namespace content {

  class SimpleNetLog : public net::NetLog {
  public:
    explicit SimpleNetLog(const std::string& app_name);
    virtual ~SimpleNetLog();

  private:
    scoped_ptr<net::NetLogLogger> net_log_logger_;

    DISALLOW_COPY_AND_ASSIGN(SimpleNetLog);
  };

}  // namespace content

#endif  // CONTENT_SIMPLE_BROWSER_SIMPLE_NET_LOG_H_
