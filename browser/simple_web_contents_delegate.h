// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef CONTENT_SIMPLE_BROWSER_SIMPLE_WEB_CONTENTS_DELEGATE_H_
#define CONTENT_SIMPLE_BROWSER_SIMPLE_WEB_CONTENTS_DELEGATE_H_


#include <vector>

#include "base/basictypes.h"
#include "base/callback_forward.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "build/build_config.h"
#include "content/public/browser/web_contents_delegate.h"
#include "content/public/browser/web_contents_observer.h"
#include "ipc/ipc_channel.h"
#include "ui/gfx/native_widget_types.h"
#include "ui/gfx/size.h"

namespace views {
  class Widget;
  class ViewsDelegate;
}

class GURL;
namespace content {

//#if defined(USE_AURA)
//  class ShellPlatformDataAura;
//#endif

  class BrowserContext;
  class SiteInstance;
  class WebContents;

  // This represents one window of the Content Shell, i.e. all the UI including
  // buttons and url bar, as well as the web content area.
  class SimpleWebContentsDelegate : public WebContentsDelegate,
    public WebContentsObserver {
  public:
    static const int kDefaultTestWindowWidthDip;
    static const int kDefaultTestWindowHeightDip;

    virtual ~SimpleWebContentsDelegate();

    void LoadURL(const GURL& url);
    void LoadURLForFrame(const GURL& url, const std::string& frame_name);
    void LoadDataWithBaseURL(const GURL& url,
      const std::string& data,
      const GURL& base_url);
    void GoBackOrForward(int offset);
    void Reload();
    void Stop();
    void UpdateNavigationControls(bool to_different_document);
    void Close();
    void ShowDevTools();
    void ShowDevToolsForElementAt(int x, int y);
    void ShowDevToolsForTest(const std::string& settings,
      const std::string& frontend_url);
    void CloseDevTools();
#if defined(OS_MACOSX)
    // Resizes the web content view to the given dimensions.
    void SizeTo(const gfx::Size& content_size);
#endif

    // Do one time initialization at application startup.
    static void Initialize();

    static SimpleWebContentsDelegate* CreateNewWindow(BrowserContext* browser_context,
      const GURL& url,
      SiteInstance* site_instance,
      int routing_id,
      const gfx::Size& initial_size);

    // Returns the Shell object corresponding to the given RenderViewHost.
    static SimpleWebContentsDelegate* FromRenderViewHost(RenderViewHost* rvh);

    // Returns the currently open windows.
    static std::vector<SimpleWebContentsDelegate*>& windows() { return windows_; }

    // Closes all windows and returns. This runs a message loop.
    static void CloseAllWindows();

    WebContents* web_contents() const { return web_contents_.get(); }
    gfx::NativeWindow window() { return window_; }

    // WebContentsDelegate
    virtual WebContents* OpenURLFromTab(WebContents* source,
      const OpenURLParams& params) OVERRIDE;
    virtual void AddNewContents(WebContents* source,
      WebContents* new_contents,
      WindowOpenDisposition disposition,
      const gfx::Rect& initial_pos,
      bool user_gesture,
      bool* was_blocked) OVERRIDE;
    virtual void LoadingStateChanged(WebContents* source,
      bool to_different_document) OVERRIDE;
    virtual void ToggleFullscreenModeForTab(WebContents* web_contents,
      bool enter_fullscreen) OVERRIDE;
    virtual bool IsFullscreenForTabOrPending(
      const WebContents* web_contents) const OVERRIDE;
    virtual void RequestToLockMouse(WebContents* web_contents,
      bool user_gesture,
      bool last_unlocked_by_target) OVERRIDE;
    virtual void CloseContents(WebContents* source) OVERRIDE;
    virtual bool CanOverscrollContent() const OVERRIDE;
    virtual void DidNavigateMainFramePostCommit(
      WebContents* web_contents) OVERRIDE;
    virtual JavaScriptDialogManager* GetJavaScriptDialogManager() OVERRIDE;
    virtual bool AddMessageToConsole(WebContents* source,
      int32 level,
      const base::string16& message,
      int32 line_no,
      const base::string16& source_id) OVERRIDE;
    virtual void RendererUnresponsive(WebContents* source) OVERRIDE;
    virtual void ActivateContents(WebContents* contents) OVERRIDE;
    virtual void DeactivateContents(WebContents* contents) OVERRIDE;
    virtual void WorkerCrashed(WebContents* source) OVERRIDE;
    virtual bool HandleContextMenu(const content::ContextMenuParams& params)
      OVERRIDE;
    virtual void WebContentsFocused(WebContents* contents) OVERRIDE;

  private:
    enum UIControl {
      BACK_BUTTON,
      FORWARD_BUTTON,
      STOP_BUTTON
    };

    explicit SimpleWebContentsDelegate(WebContents* web_contents);

    // Helper to create a new Shell given a newly created WebContents.
    static SimpleWebContentsDelegate* CreateShell(WebContents* web_contents,
      const gfx::Size& initial_size);

    // Helper for one time initialization of application
    static void PlatformInitialize(const gfx::Size& default_window_size);
    // Helper for one time deinitialization of platform specific state.
    static void PlatformExit();

    // Adjust the size when Blink sends 0 for width and/or height.
    // This happens when Blink requests a default-sized window.
    static gfx::Size AdjustWindowSize(const gfx::Size& initial_size);

    // All the methods that begin with Platform need to be implemented by the
    // platform specific Shell implementation.
    // Called from the destructor to let each platform do any necessary cleanup.
    void PlatformCleanUp();
    // Creates the main window GUI.
    void PlatformCreateWindow(int width, int height);
    // Links the WebContents into the newly created window.
    void PlatformSetContents();
    // Resize the content area and GUI.
    void PlatformResizeSubViews();
    // Enable/disable a button.
    void PlatformEnableUIControl(UIControl control, bool is_enabled);
    // Updates the url in the url bar.
    void PlatformSetAddressBarURL(const GURL& url);
    // Sets whether the spinner is spinning.
    void PlatformSetIsLoading(bool loading);
    // Set the title of shell window
    void PlatformSetTitle(const base::string16& title);
    // User right-clicked on the web view
    bool PlatformHandleContextMenu(const content::ContextMenuParams& params);

#if defined(TOOLKIT_VIEWS)
    void PlatformWebContentsFocused(WebContents* contents);
#endif

    gfx::NativeView GetContentView();

    // WebContentsObserver
    virtual void TitleWasSet(NavigationEntry* entry, bool explicit_set) OVERRIDE;

    void InnerShowDevTools(const std::string& settings,
      const std::string& frontend_url);
    void OnDevToolsWebContentsDestroyed();

    //scoped_ptr<ShellJavaScriptDialogManager> dialog_manager_;
    scoped_ptr<WebContents> web_contents_;

    bool is_fullscreen_;

    gfx::NativeWindow window_;
    gfx::NativeEditView url_edit_view_;

    gfx::Size content_size_;



//#if defined(TOOLKIT_VIEWS)
//    static views::ViewsDelegate* views_delegate_;
//    views::Widget* window_widget_;
//#endif // defined(TOOLKIT_VIEWS)
//    static ShellPlatformDataAura* platform_;
//

    bool headless_;

    // A container of all the open windows. We use a vector so we can keep track
    // of ordering.
    static std::vector<SimpleWebContentsDelegate*> windows_;

    // True if the destructur of Shell should post a quit closure on the current
    // message loop if the destructed Shell object was the last one.
    static bool quit_message_loop_;
  };

}  // namespace content

#endif  // CONTENT_SIMPLE_BROWSER_SIMPLE_WEB_CONTENTS_DELEGATE_H_
