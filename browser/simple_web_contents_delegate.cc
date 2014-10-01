// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "content/simple/browser/simple_web_contents_delegate.h"

#include "base/auto_reset.h"
#include "base/command_line.h"
#include "base/message_loop/message_loop.h"
#include "base/path_service.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_util.h"
#include "base/strings/stringprintf.h"
#include "base/strings/utf_string_conversions.h"
#include "content/public/browser/devtools_manager.h"
#include "content/public/browser/navigation_controller.h"
#include "content/public/browser/navigation_entry.h"
#include "content/public/browser/render_view_host.h"
#include "content/public/browser/web_contents.h"
#include "content/public/browser/web_contents_observer.h"
#include "content/public/common/renderer_preferences.h"
//#include "content/shell/browser/notify_done_forwarder.h"
#include "content/simple/browser/simple_browser_main_parts.h"
#include "content/simple/browser/simple_content_browser_client.h"
//#include "content/simple/browser/simple_devtools_frontend.h"
//#include "content/simple/browser/simple_javascript_dialog_manager.h"
//#include "content/simple/browser/webkit_test_controller.h"
//#include "content/simple/common/simple_messages.h"
//#include "content/simple/common/simple_switches.h"

namespace content {

  const int SimpleWebContentsDelegate::kDefaultTestWindowWidthDip = 800;
  const int SimpleWebContentsDelegate::kDefaultTestWindowHeightDip = 600;

  std::vector<SimpleWebContentsDelegate*> SimpleWebContentsDelegate::windows_;

  bool SimpleWebContentsDelegate::quit_message_loop_ = true;

  SimpleWebContentsDelegate::SimpleWebContentsDelegate(WebContents* web_contents)
    : WebContentsObserver(web_contents),
    is_fullscreen_(false),
    window_(NULL),
    url_edit_view_(NULL),
    headless_(false) {
    const CommandLine& command_line = *CommandLine::ForCurrentProcess();
    windows_.push_back(this);
  }
  SimpleWebContentsDelegate::SimpleWebContentsDelegate(HWND hwnd)
  {
    parent_hwnd_ = hwnd;
  }
  SimpleWebContentsDelegate::~SimpleWebContentsDelegate() {
    PlatformCleanUp();

    for (size_t i = 0; i < windows_.size(); ++i) {
      if (windows_[i] == this) {
        windows_.erase(windows_.begin() + i);
        break;
      }
    }

    if (windows_.empty() && quit_message_loop_) {
      if (headless_)
        PlatformExit();
      base::MessageLoop::current()->PostTask(FROM_HERE,
        base::MessageLoop::QuitClosure());
    }
  }

  SimpleWebContentsDelegate* SimpleWebContentsDelegate::CreateShell(WebContents* web_contents,
    const gfx::Size& initial_size) {
    SimpleWebContentsDelegate* shell = new SimpleWebContentsDelegate(web_contents);
    shell->PlatformCreateWindow(initial_size.width(), initial_size.height());

    shell->web_contents_.reset(web_contents);
    web_contents->SetDelegate(shell);

    shell->PlatformSetContents();
    shell->PlatformResizeSubViews();

    return shell;
  }

  void SimpleWebContentsDelegate::CloseAllWindows() {
    base::AutoReset<bool> auto_reset(&quit_message_loop_, false);
    DevToolsManager::GetInstance()->CloseAllClientHosts();
    std::vector<SimpleWebContentsDelegate*> open_windows(windows_);
    for (size_t i = 0; i < open_windows.size(); ++i)
      open_windows[i]->Close();
    PlatformExit();
    base::MessageLoop::current()->RunUntilIdle();
  }


  SimpleWebContentsDelegate* SimpleWebContentsDelegate::FromRenderViewHost(RenderViewHost* rvh) {
    for (size_t i = 0; i < windows_.size(); ++i) {
      if (windows_[i]->web_contents() &&
        windows_[i]->web_contents()->GetRenderViewHost() == rvh) {
        return windows_[i];
      }
    }
    return NULL;
  }

  // static
  void SimpleWebContentsDelegate::Initialize() {
    PlatformInitialize(
      gfx::Size(kDefaultTestWindowWidthDip, kDefaultTestWindowHeightDip));
  }

  gfx::Size SimpleWebContentsDelegate::AdjustWindowSize(const gfx::Size& initial_size) {
    if (!initial_size.IsEmpty())
      return initial_size;
    return gfx::Size(kDefaultTestWindowWidthDip, kDefaultTestWindowHeightDip);
  }

  SimpleWebContentsDelegate* SimpleWebContentsDelegate::CreateNewWindow(BrowserContext* browser_context,
    const GURL& url,
    SiteInstance* site_instance,
    int routing_id,
    const gfx::Size& initial_size) {
    WebContents::CreateParams create_params(browser_context, site_instance);
    create_params.routing_id = routing_id;
    create_params.initial_size = AdjustWindowSize(initial_size);
    WebContents* web_contents = WebContents::Create(create_params);
    SimpleWebContentsDelegate* shell = CreateShell(web_contents, create_params.initial_size);
    if (!url.is_empty())
      shell->LoadURL(url);
    return shell;
  }

  void SimpleWebContentsDelegate::CreateNewTab(BrowserContext* browser_context,
    const GURL& url,
    SiteInstance* site_instance,
    int routing_id,
    const gfx::Size& initial_size) {
    WebContents::CreateParams create_params(browser_context, site_instance);
    create_params.routing_id = routing_id;
    create_params.initial_size = AdjustWindowSize(initial_size);
    WebContents* web_contents = WebContents::Create(create_params);

    //SimpleWebContentsDelegate* shell = CreateShell(web_contents, create_params.initial_size);
    //if (!url.is_empty())
    //  shell->LoadURL(url);
    //return shell;

    PlatformCreateWindow(initial_size.width(), initial_size.height());
    web_contents_.reset(web_contents);
    web_contents->SetDelegate(this);
    PlatformSetContents();
    PlatformResizeSubViews();

    if (!url.is_empty())
      LoadURL(url);
  }

  void SimpleWebContentsDelegate::LoadURL(const GURL& url) {
    LoadURLForFrame(url, std::string());
  }

  void SimpleWebContentsDelegate::LoadURLForFrame(const GURL& url, const std::string& frame_name) {
    NavigationController::LoadURLParams params(url);
    params.transition_type = PageTransitionFromInt(
      PAGE_TRANSITION_TYPED | PAGE_TRANSITION_FROM_ADDRESS_BAR);
    params.frame_name = frame_name;
    web_contents_->GetController().LoadURLWithParams(params);
    web_contents_->Focus();
  }

  void SimpleWebContentsDelegate::LoadDataWithBaseURL(const GURL& url, const std::string& data,
    const GURL& base_url) {
    const GURL data_url = GURL("data:text/html;charset=utf-8," + data);
    NavigationController::LoadURLParams params(data_url);
    params.load_type = NavigationController::LOAD_TYPE_DATA;
    params.base_url_for_data_url = base_url;
    params.virtual_url_for_data_url = url;
    params.override_user_agent = NavigationController::UA_OVERRIDE_FALSE;
    web_contents_->GetController().LoadURLWithParams(params);
    web_contents_->Focus();
  }

  void SimpleWebContentsDelegate::AddNewContents(WebContents* source,
    WebContents* new_contents,
    WindowOpenDisposition disposition,
    const gfx::Rect& initial_pos,
    bool user_gesture,
    bool* was_blocked) {
    CreateShell(new_contents, AdjustWindowSize(initial_pos.size()));
  }

  void SimpleWebContentsDelegate::GoBackOrForward(int offset) {
    web_contents_->GetController().GoToOffset(offset);
    web_contents_->Focus();
  }

  void SimpleWebContentsDelegate::Reload() {
    web_contents_->GetController().Reload(false);
    web_contents_->Focus();
  }

  void SimpleWebContentsDelegate::Stop() {
    web_contents_->Stop();
    web_contents_->Focus();
  }

  void SimpleWebContentsDelegate::UpdateNavigationControls(bool to_different_document) {
    int current_index = web_contents_->GetController().GetCurrentEntryIndex();
    int max_index = web_contents_->GetController().GetEntryCount() - 1;

    PlatformEnableUIControl(BACK_BUTTON, current_index > 0);
    PlatformEnableUIControl(FORWARD_BUTTON, current_index < max_index);
    PlatformEnableUIControl(STOP_BUTTON,
      to_different_document && web_contents_->IsLoading());
  }


  gfx::NativeView SimpleWebContentsDelegate::GetContentView() {
    if (!web_contents_)
      return NULL;
    return web_contents_->GetNativeView();
  }

  WebContents* SimpleWebContentsDelegate::OpenURLFromTab(WebContents* source,
    const OpenURLParams& params) {
    // CURRENT_TAB is the only one we implement for now.
    if (params.disposition != CURRENT_TAB)
      return NULL;
    NavigationController::LoadURLParams load_url_params(params.url);
    load_url_params.referrer = params.referrer;
    load_url_params.frame_tree_node_id = params.frame_tree_node_id;
    load_url_params.transition_type = params.transition;
    load_url_params.extra_headers = params.extra_headers;
    load_url_params.should_replace_current_entry =
      params.should_replace_current_entry;

    if (params.transferred_global_request_id != GlobalRequestID()) {
      load_url_params.is_renderer_initiated = params.is_renderer_initiated;
      load_url_params.transferred_global_request_id =
        params.transferred_global_request_id;
    }
    else if (params.is_renderer_initiated) {
      load_url_params.is_renderer_initiated = true;
    }

    source->GetController().LoadURLWithParams(load_url_params);
    return source;
  }

  void SimpleWebContentsDelegate::LoadingStateChanged(WebContents* source,
    bool to_different_document) {
    UpdateNavigationControls(to_different_document);
    PlatformSetIsLoading(source->IsLoading());
  }


  bool SimpleWebContentsDelegate::IsFullscreenForTabOrPending(const WebContents* web_contents) const {
    return is_fullscreen_;
  }

  void SimpleWebContentsDelegate::ToggleFullscreenModeForTab(WebContents* web_contents,
    bool enter_fullscreen) {
    if (is_fullscreen_ != enter_fullscreen) {
      is_fullscreen_ = enter_fullscreen;
      web_contents->GetRenderViewHost()->WasResized();
    }
  }

  void SimpleWebContentsDelegate::RequestToLockMouse(WebContents* web_contents,
    bool user_gesture,
    bool last_unlocked_by_target) {
    web_contents->GotResponseToLockMouseRequest(true);
  }

  void SimpleWebContentsDelegate::CloseContents(WebContents* source) {
    Close();
  }

  bool SimpleWebContentsDelegate::CanOverscrollContent() const {
#if defined(USE_AURA)
    return true;
#else
    return false;
#endif
  }

  void SimpleWebContentsDelegate::DidNavigateMainFramePostCommit(WebContents* web_contents) {
    PlatformSetAddressBarURL(web_contents->GetLastCommittedURL());
  }

  void SimpleWebContentsDelegate::ActivateContents(WebContents* contents) {
    contents->GetRenderViewHost()->Focus();
  }

  void SimpleWebContentsDelegate::DeactivateContents(WebContents* contents) {
    contents->GetRenderViewHost()->Blur();
  }


  bool SimpleWebContentsDelegate::HandleContextMenu(const content::ContextMenuParams& params) {
    return PlatformHandleContextMenu(params);
  }

  void SimpleWebContentsDelegate::WebContentsFocused(WebContents* contents) {
#if defined(TOOLKIT_VIEWS)
    PlatformWebContentsFocused(contents);
#endif
  }

  void SimpleWebContentsDelegate::TitleWasSet(NavigationEntry* entry, bool explicit_set) {
    if (entry)
      PlatformSetTitle(entry->GetTitle());
  }

}  // namespace content
