// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

//#include "content/simple/ui/stdafx.h"
#include "MainFrm.h"
#include "AddressBar.h"
#include "SimpleClient.h"
#include "SimpleTab.h"


CMainFrame::CMainFrame()
{
  m_addressbar = NULL;
  m_clientview = NULL;
  m_tab = NULL;
  //m_web_contents_delegate = NULL;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if(CFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
		return TRUE;

  return FALSE;
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	// HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	// m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	// m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	  SetMenu(NULL);

	// HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	// CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	// AddSimpleReBarBand(hWndCmdBar);
	// AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

	// CreateSimpleStatusBar();

  IniUI();

	//m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

  

	// UIAddToolBar(hWndToolBar);
	//UISetCheck(ID_VIEW_TOOLBAR, 1);
	//UISetCheck(ID_VIEW_STATUS_BAR, 1);

  OpenHomePage();

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 1;
}


void CMainFrame::IniUI()
{
  RECT rcClient;
  GetClientRect(&rcClient);
  RECT rcAddress = rcClient;
  RECT rcTab = rcClient;
  RECT rcView = rcClient;

  m_addressbar = new CAddressBar;
  m_addressbar->m_main_frame = m_hWnd;
  m_addressbar->Create(m_hWnd);

  m_tab = new CSimpleTab;
  m_tab->m_main_frame = m_hWnd;
  m_tab->Create(m_hWnd);

  m_clientview = new CSimpleClient;
  m_clientview->Create(m_hWnd);

  LayoutUI(rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

  m_addressbar->ShowWindow(SW_SHOW);
  m_tab->ShowWindow(SW_SHOW);
  m_clientview->ShowWindow(SW_SHOW);
}

void CMainFrame::OnSize(UINT nType, CSize size)
{
  LayoutUI(size.cx, size.cy);
}


void CMainFrame::LayoutUI(int x, int y)
{
  if (NULL != m_addressbar)
  {
    RECT rcAddressBar = { 0 };

    rcAddressBar.left = 0;
    rcAddressBar.top = 0;
    rcAddressBar.right = x;
    rcAddressBar.bottom = rcAddressBar.left + 30;
    m_addressbar->MoveWindow(&rcAddressBar);

    RECT rcTab = {0};
    rcTab.left = 0;
    rcTab.top = rcAddressBar.bottom;
    rcTab.right = x;
    rcTab.bottom = rcTab.top + 30;
    m_tab->MoveWindow(&rcTab);

    RECT rcClientView = {0};
    rcClientView.left = 0;
    rcClientView.top = rcTab.bottom;
    rcClientView.right = x;
    rcClientView.bottom = y;
    m_clientview->MoveWindow(&rcClientView);

    //if (m_web_contents_delegate)
    //{
    //  m_web_contents_delegate->ResizeView(rcClientView);
    //}
  }
}

void CMainFrame::OpenHomePage()
{
  //m_web_contents_delegate = new content::SimpleWebContentsDelegate();
  //m_web_contents_delegate->Initialize();
  //m_web_contents_delegate->CreateNewTab((content::BrowserContext*)m_browser_main->browser_context_.get(),
  //  GURL("http://www.baidu.com/"), NULL, MSG_ROUTING_NONE, gfx::Size());
  //m_web_contents_delegate->SetHWND(m_hWnd, m_clientview->m_hWnd);
  //m_web_contents_delegate->window_ = m_clientview->m_hWnd;
  //m_web_contents_delegate->SetBrowserContext((content::BrowserContext*)m_browser_main->browser_context_.get());
  //m_web_contents_delegate->Initialize((content::BrowserContext*)m_browser_main->browser_context_.get(), GURL("http://www.baidu.com/"), NULL,MSG_ROUTING_NONE, gfx::Size());

}

LRESULT CMainFrame::OnReturn(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  //CString url = m_addressbar->GetUrl();
  //if (-1 == url.Find(L"http://"))
  //{
  //  url = L"http://" + url;
  //}
  //
  //string16 sUrl= url;
  //m_web_contents_delegate->LoadURL(GURL(sUrl));
  return 0;
}

LRESULT CMainFrame::OnCreateTab(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  //HWND hwnd = m_tab->CreateNewTab();
  //m_web_contents_delegate->MakePair(hwnd, lParam);
  //m_tab->SetCheck(hwnd);

  //// 设置地址栏
  //m_addressbar->SetUrl(m_web_contents_delegate->GetURL().c_str());

  //CButton button;
  //button.Attach(hwnd);
  //// 设置tab标题
  //CString title = m_web_contents_delegate->GetTitle().c_str();
  //button.SetWindowText(title);
 
  return 0;
}

LRESULT CMainFrame::OnSwitchTab(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  //m_web_contents_delegate->SwitchTab((HWND)wParam);
 
  // 设置地址栏
  //m_addressbar->SetUrl(m_web_contents_delegate->GetURL().c_str());

  // 设置tab选中
  return 0;
}

LRESULT CMainFrame::OnAddTab(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  //m_web_contents_delegate->AddTab();
  //m_addressbar->SetUrl(L"");
  return 0;
}

LRESULT CMainFrame::OnCloseTab(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

  //m_web_contents_delegate->CloseTab(wParam);
  return 0;
}

LRESULT CMainFrame::OnUpdateTab(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  //HWND hwnd = (HWND)lParam;
  //content::TAB_INFO ti = m_web_contents_delegate->GetTabInfo(hwnd);
  //CButton button;
  //button.Attach(hwnd);
  //// 设置tab标题
  //button.SetWindowText(ti.title.c_str());
  return 0;
}

LRESULT CMainFrame::OnSetUrl(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  // 设置地址栏
  //m_addressbar->SetUrl(m_web_contents_delegate->GetURL().c_str());
  return 0;
}

LRESULT CMainFrame::OnBack(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  //m_web_contents_delegate->Back();
  return 0;
}

LRESULT CMainFrame::OnForward(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  //m_web_contents_delegate->Forward();
  return 0;
}

LRESULT CMainFrame::OnReload(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  //m_web_contents_delegate->Reload();
  return 0;
}

LRESULT CMainFrame::OnStop(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  //m_web_contents_delegate->Stop();
  return 0;
}