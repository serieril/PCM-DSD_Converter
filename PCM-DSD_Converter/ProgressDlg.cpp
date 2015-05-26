// ProgressDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "PCM-DSD_Converter.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"

// ProgressDlg ダイアログ

IMPLEMENT_DYNAMIC(ProgressDlg, CDialogEx)

ProgressDlg::ProgressDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(ProgressDlg::IDD, pParent)
, m_evTEXT(_T(""))
{

}

void ProgressDlg::OnSetFocus() {
	::DestroyCaret();
}

ProgressDlg::~ProgressDlg()
{
}
void ProgressDlg::OnCancel(){
	//DestroyWindow();
}
void ProgressDlg::OnOK(){
	//DestroyWindow();
}
void ProgressDlg::PostNcDestroy()
{
	//delete this;    //BBB追加  ダイアログの破棄
}
void ProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_pProgress);
	DDX_Text(pDX, IDC_PATHTEXT, m_evTEXT);
	DDX_Control(pDX, IDC_PATHTEXT, m_ecTEXT);
	DDX_Control(pDX, IDC_EDIT2, m_ecTimes);
}

void ProgressDlg::Start(TCHAR *Path){
	TCHAR tchstr1[512];
	TCHAR *tchstr2 = L"をDSD変換中";
	lstrcpy(tchstr1, Path);
	lstrcat(tchstr1, tchstr2);
	m_ecTEXT.SetWindowTextW(tchstr1);
}

void ProgressDlg::Process(int percent, int position){
	TCHAR *tchstr;
	m_pProgress.SetRange32(0, position);
	m_pProgress.SetPos(percent);
	if (percent == 0){
		tchstr = L"アップサンプリングの準備中";
		m_ecTimes.SetWindowTextW(tchstr);
	}
	else if (percent == position){
		tchstr = L"書き出し中";
		m_ecTimes.SetWindowTextW(tchstr);

	}
	else{
		tchstr = L"DSD変換中";
		if (percent == 1)m_ecTimes.SetWindowTextW(tchstr);
	}
}

BEGIN_MESSAGE_MAP(ProgressDlg, CDialogEx)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_CancelBottun, &ProgressDlg::OnBnClickedCancelbottun)
END_MESSAGE_MAP()


BOOL ProgressDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return true;
}


void ProgressDlg::OnBnClickedCancelbottun()
{
	Cancelbottun = false;
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
