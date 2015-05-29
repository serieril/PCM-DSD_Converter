#pragma once
#include "afxwin.h"
#include "afxcmn.h"

// ProgressDlg ダイアログ

class ProgressDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ProgressDlg)

public:
	ProgressDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	virtual ~ProgressDlg();

	// ダイアログ データ
	enum { IDD = IDD_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
	void Start(TCHAR *Path);
	void Process(unsigned int percent, unsigned int position);
	CProgressCtrl m_pProgress;
	CString m_evTEXT;
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	CEdit m_ecTEXT;
	virtual void OnSetFocus();
	CEdit m_ecTimes;
	afx_msg void OnBnClickedCancelbottun();
	bool Cancelbottun = true;
};
