
// PCM-DSD_ConverterDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <string>
#include"ProgressDlg.h"
#include "fftw3.h"
#include <omp.h>
#include <fstream>
#include <complex>
#define _USE_MATH_DEFINES
#include <math.h>
#include <locale.h>

#include <malloc.h>
#include <stdlib.h>

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
	CStatic m_scPrecision;
	CComboBox m_ccPrecision;
	CStatic m_scPath;
	CEdit m_ecPath;
	CButton m_bcPath;
	CComboBox m_cSamplingRate;
	CStatic m_sSamplingRate;
	CString m_evPath;
	//全て実行
	afx_msg void OnBnClickedAllrun();
	//全て削除
	afx_msg void OnBnClickedAlllistdelete();
	//実行
	afx_msg void OnBnClickedRun();
	//削除
	afx_msg void OnBnClickedListdelete();
	//参照
	afx_msg void OnBnClickedPathcheck();
	//ファイル/ディレクトリがドロップ&ドラッグ
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//閉じる
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	//サイズ変更
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//Wavファイルがドロップされた時の初動
	afx_msg void WAV_FileRead(TCHAR *FileName);
	//ディレクトリの再帰的検索
	afx_msg void DirectoryFind(TCHAR *DirectoryPath);
	//Wavファイルチェック及びメタデータ読み取り
	afx_msg bool WAV_Metadata(TCHAR *filepath, wstring *metadata);
	//PCM-DSD変換の管理
	afx_msg bool WAV_Convert(TCHAR *filepath, int number);
	//DSDIFF形式で書き込み
	afx_msg bool DSD_Write(FILE *LData, FILE *RData, FILE *WriteData, int number);
	//読み書きデータ準備
	afx_msg bool RequireWriteData(TCHAR *filepath, CString flag, wchar_t *FileMode, FILE **WriteDatadsd);
	//一時ファイル削除
	afx_msg bool TrushFile(TCHAR *filepath, CString flag);
	//Wavファイルを64bitFloat(double)化し、LR分離して一時ファイルとして書き出し
	afx_msg bool TmpWriteData(TCHAR *filepath, FILE *tmpl, FILE *tmpr, int Times);
	//PCM-DSD変換
	afx_msg bool WAV_Filter_Renew(FILE *UpSampleData, FILE *OrigData, unsigned int Times, omp_lock_t *myLock, unsigned int DSDsamplingRate);
	afx_msg bool WAV_FilterLight_Renew(FILE *UpSampleData, FILE *OrigData, unsigned int Times);
	//フリーズ防止のためにスレッド作成
	void WorkThread();
	//処理中はボタンなど無効に
	void Disable();
	//処理が終わったらボタンなど有効化
	void Enable();
	//F1ヘルプ無効化
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	//FFTプラン初期化
	void FFTInit(fftw_plan *plan, unsigned int fftsize, int Times, double *fftin, fftw_complex *ifftout);
	void iFFTInit(fftw_plan *plan, unsigned int fftsize, int Times, fftw_complex *ifftin, double *fftout);
	void PathCheck();
};
