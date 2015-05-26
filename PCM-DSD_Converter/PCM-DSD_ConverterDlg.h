
// PCM-DSD_ConverterDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <string>
#include"ProgressDlg.h"
#include <fftw3.h>
#include <omp.h>
using namespace std;

// CPCMDSD_ConverterDlg ダイアログ
class CPCMDSD_ConverterDlg : public CDialogEx
{
	// コンストラクション
public:
	CPCMDSD_ConverterDlg(CWnd* pParent = NULL);	// 標準コンストラクター
	string flag_Bottun = "";
	// ダイアログ データ
	enum { IDD = IDD_PCMDSD_CONVERTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート

private:
	ProgressDlg m_dProgress;
	static UINT __cdecl WorkThread(LPVOID pParam);
	// 実装
protected:
	HICON m_hIcon;
	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	virtual void ListInit();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCancel();
	CMFCListCtrl m_lFileList;
	CButton m_bAllRun;
	CButton m_bAllListDelete;
	CButton m_bRun;
	CButton m_bListDelete;
	afx_msg void OnBnClickedAllrun();
	afx_msg void OnBnClickedAlllistdelete();
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedListdelete();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CComboBox m_cSamplingRate;
	afx_msg void OnCbnSelchangeSamplingrate();
	CStatic m_sSamplingRate;
	afx_msg void WAV_FileRead(TCHAR *FileName);
	afx_msg void DirectoryFind(TCHAR *DirectoryPath);
	afx_msg bool WAV_Metadata(TCHAR *filepath, string *metadata);
	afx_msg bool WAV_Convert(TCHAR *filepath, int number);
	afx_msg bool DSD_Write(FILE *LData, FILE *RData, FILE *WriteData, int number);
	afx_msg bool Creat_TmpFile(TCHAR *filepath);
	afx_msg bool RequireWriteData(TCHAR *filepath, CString flag, wchar_t *FileMode, FILE **WriteDatadsd);
	afx_msg bool TmpWriteData(TCHAR *filepath, FILE *tmpl, FILE *tmpr, int Times);
	afx_msg bool WAV_Filter(FILE *UpSampleData, FILE *OrigData, int Times, omp_lock_t *myLock);
	afx_msg bool PCMtoDSD(FILE *UpSampleData, FILE *OrigData, int dsdtimes);
	void WorkThread();
	void Disable();
	void Enable();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	void FFTInit(fftw_plan *plan, __int64 fftsize, int Times, double *fftin, fftw_complex *ifftout);
	void iFFTInit(fftw_plan *plan, __int64 fftsize, int Times, fftw_complex *ifftin, double *fftout);
};
