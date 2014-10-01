// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef CONTENT_SIMPLE_UI_MAINFRM_H_
#define CONTENT_SIMPLE_UI_MAINFRM_H_

#include "stdafx.h"
#include <vector>
#include "resource.h"

#include "content/simple/browser/simple_browser_main_parts.h"
#include "content/simple/browser/simple_web_contents_delegate.h"
class CAddressBar;
class CSimpleClient;
class CSimpleTab;
//class content::SimpleBrowserMainParts;

class CMainFrame : 
	public CFrameWindowImpl<CMainFrame>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)

	//CSimpleView m_view;
	//CCommandBarCtrl m_CmdBar;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
  virtual ~CMainFrame(){};

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_USER_RETURN, OnReturn)
    MESSAGE_HANDLER(WM_USER_CREATE_TAB, OnCreateTab)
    MESSAGE_HANDLER(WM_USER_SWITCH_TAB, OnSwitchTab)
    MESSAGE_HANDLER(WM_USER_ADD_TAB, OnAddTab)
    MESSAGE_HANDLER(WM_USER_CLOSE_TAB, OnCloseTab)
    MESSAGE_HANDLER(WM_USER_UPDATE_TAB, OnUpdateTab)
    MESSAGE_HANDLER(WM_USER_SET_URL, OnSetUrl)
    MESSAGE_HANDLER(WM_USER_BACK, OnBack)
    MESSAGE_HANDLER(WM_USER_FORWARD, OnForward)
    MESSAGE_HANDLER(WM_USER_RELOAD, OnReload)
    MESSAGE_HANDLER(WM_USER_STOP, OnStop)
    MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  CMainFrame();
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnReturn(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCreateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSwitchTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnAddTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCloseTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnUpdateTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnSetUrl(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnBack(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnForward(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnReload(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnStop(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  void OnSize(UINT nType, CSize size);

  void OpenHomePage();

  void IniUI();
  void LayoutUI(int x, int y);
  //void SwitchTab(HWND hwnd);
  //void SwitchTab(HWND hwnd);

public:
  CAddressBar* m_addressbar;
  CSimpleClient* m_clientview;
  CSimpleTab* m_tab;
  content::SimpleBrowserMainParts* m_browser_main;
  content::SimpleWebContentsDelegate* m_web_contents_delegate;
};

#endif  // CONTENT_SIMPLE_UI_MAINFRM_H_