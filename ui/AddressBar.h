
#ifndef CONTENT_SIMPLE_UI_ADDRESSBAR_H_
#define CONTENT_SIMPLE_UI_ADDRESSBAR_H_

class CAddressBar : public CDialogImpl<CAddressBar>
{
public:
  enum { IDD = IDD_ADDRESS_BAR };

  BEGIN_MSG_MAP(CAddressBar)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
    COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
    COMMAND_ID_HANDLER(IDC_BACK, OnBack)
    COMMAND_ID_HANDLER(IDC_FORWARD, OnForward)
    COMMAND_ID_HANDLER(IDC_RELOAD, OnReload)
    COMMAND_ID_HANDLER(IDC_STOP, OnStop)
  ALT_MSG_MAP(1)
    //MSG_WM_CHAR(OnEdit)
    MESSAGE_HANDLER(WM_CHAR, OnEdit)
  END_MSG_MAP()

  // Handler prototypes (uncomment arguments if needed):
  //	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  //	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
  //	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

  CAddressBar();

  LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
  LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnBack(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnForward(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnReload(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  LRESULT OnStop(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
  void SetUrl(CString url);
  CString GetUrl();
  LRESULT OnEdit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  CContainedWindow m_edit;
  HWND m_main_frame;
};

#endif  // CONTENT_SIMPLE_UI_ADDRESSBAR_H_
