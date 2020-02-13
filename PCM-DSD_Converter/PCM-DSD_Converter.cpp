
// PCM-DSD_Converter.cpp : アプリケーションのクラス動作を定義します。
//

#include "stdafx.h"
#include "PCM-DSD_Converter.h"
#include "PCM-DSD_ConverterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPCMDSD_ConverterApp

BEGIN_MESSAGE_MAP(CPCMDSD_ConverterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CPCMDSD_ConverterApp コンストラクション

CPCMDSD_ConverterApp::CPCMDSD_ConverterApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CPCMDSD_ConverterApp オブジェクトです。

CPCMDSD_ConverterApp theApp;


// CPCMDSD_ConverterApp 初期化

BOOL CPCMDSD_ConverterApp::InitInstance()
{
	CWinApp::InitInstance();
	

	// ダイアログにシェル ツリー ビューまたはシェル リスト ビュー コントロールが
	// 含まれている場合にシェル マネージャーを作成します。
	//CShellManager *pShellManager = new CShellManager;

	// MFC コントロールでテーマを有効にするために、"Windows ネイティブ" のビジュアル マネージャーをアクティブ化
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 標準初期化
	// これらの機能を使わずに最終的な実行可能ファイルの
	// サイズを縮小したい場合は、以下から不要な初期化
	// ルーチンを削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: 会社名または組織名などの適切な文字列に
	// この文字列を変更してください。
	SetRegistryKey(_T("PCMDSD.COM"));

	CPCMDSD_ConverterDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	//if (nResponse == IDOK)
	//{
	//	// TODO: ダイアログが <OK> で消された時のコードを
	//	//  記述してください。
	//}
	//else if (nResponse == IDCANCEL)
	//{
	//	// TODO: ダイアログが <キャンセル> で消された時のコードを
	//	//  記述してください。
	//}
	//else if (nResponse == -1)
	//{
	//	TRACE(traceAppMsg, 0, "警告: ダイアログの作成に失敗しました。アプリケーションは予期せずに終了します。\n");
	//	TRACE(traceAppMsg, 0, "警告: ダイアログで MFC コントロールを使用している場合、#define _AFX_NO_MFC_CONTROLS_IN_DIALOGS を指定できません。\n");
	//}

	//// 上で作成されたシェル マネージャーを削除します。
	//if (pShellManager != nullptr)
	//{
	//	delete pShellManager;
	//}

	// ダイアログは閉じられました。アプリケーションのメッセージ ポンプを開始しないで
	//  アプリケーションを終了するために FALSE を返してください。
	return FALSE;
}