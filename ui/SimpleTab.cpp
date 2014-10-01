#include "content/simple/ui/stdafx.h"
#include "resource.h"
#include "SimpleTab.h"


LRESULT CSimpleTab::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
  m_hButtonBk = CreateSolidBrush(RGB(255, 0, 0));
  m_hCurrentBrush = (HBRUSH) GetCurrentObject(NULL,OBJ_BRUSH);

  CenterWindow(GetParent());
  m_index = 1000;
  Layout();
  return TRUE;
}

LRESULT CSimpleTab::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
  EndDialog(wID);
  return 0;
}

LRESULT CSimpleTab::OnAdd(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{

  // MessageBox(L"OnAdd");
  //CreateNewTab();
  ::PostMessage(m_main_frame, WM_USER_ADD_TAB, 0, 0);
  return TRUE;
}

HWND CSimpleTab::CreateNewTab()
{
  CButton* btn = new CButton;
  m_buttons.push_back(btn);
  RECT rc = {0,0, 0, 0};
  btn->Create(m_hWnd, rc, L"�½���ǩҳ", WS_CHILD | WS_VISIBLE | BS_LEFT | BS_AUTORADIOBUTTON | BS_NOTIFY | BS_PUSHLIKE, 0, ++m_index);

  Layout();
  return btn->m_hWnd;
}

void CSimpleTab::Layout()
{
  int index = 0;
  int nWidth = 200;
  int nHeight= 25;
  for (vector<CButton*>::iterator iter = m_buttons.begin(); iter != m_buttons.end(); ++iter)
  {
    CButton* btn = *iter;
    ::MoveWindow(btn->m_hWnd, index* nWidth, 2, nWidth, nHeight, TRUE);
    index++;
  }

  ::MoveWindow(GetDlgItem(IDC_ADD), index* nWidth, 2, nHeight, nHeight, TRUE);
}

LRESULT CSimpleTab::OnButton(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  if (BN_CLICKED == wNotifyCode)
  {
    m_current_tab = hWndCtl;
    ::PostMessage(m_main_frame, WM_USER_SWITCH_TAB, (WPARAM)hWndCtl, 0);
    return TRUE;
  }

  if (BN_DBLCLK == wNotifyCode)
  {
    for (vector<CButton*>::iterator iter = m_buttons.begin(); iter != m_buttons.end(); ++iter)
    {
      CButton* btn = *iter;
      if (hWndCtl == btn->m_hWnd)
      {
        btn->DestroyWindow();
        m_buttons.erase(iter);
        delete btn;
        Layout();
        ::PostMessage(m_main_frame, WM_USER_CLOSE_TAB, (WPARAM)hWndCtl, 0);
        return 0;
      }
    }
  }
  return TRUE;
}

void CSimpleTab::SetCheck(HWND hwnd)
{
  for (vector<CButton*>::iterator iter = m_buttons.begin(); iter != m_buttons.end(); ++iter)
  {
    CButton* btn = *iter;
    if (hwnd == btn->m_hWnd)
    {
      btn->SetCheck(TRUE);
    }
    else
    {
      btn->SetCheck(FALSE);
    }
  }
}