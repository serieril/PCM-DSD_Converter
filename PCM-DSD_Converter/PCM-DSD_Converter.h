
// PCM-DSD_Converter.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CPCMDSD_ConverterApp:
// このクラスの実装については、PCM-DSD_Converter.cpp を参照してください。
//

class CPCMDSD_ConverterApp : public CWinApp
{
public:
	CPCMDSD_ConverterApp();

	// オーバーライド
public:
	virtual BOOL InitInstance();

	// 実装

	DECLARE_MESSAGE_MAP()
};

extern CPCMDSD_ConverterApp theApp;