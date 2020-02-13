
// PCM-DSD_ConverterDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "PCM-DSD_Converter.h"
#include "PCM-DSD_ConverterDlg.h"
#include "afxdialogex.h"
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

template<class T>
T reverse_endian(T value)
{
	char* first = reinterpret_cast<char*>(&value);
	char* last = first + sizeof(T);
	std::reverse(first, last);
	return value;
}


// CPCMDSD_ConverterDlg ダイアログ
CPCMDSD_ConverterDlg::CPCMDSD_ConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPCMDSD_ConverterDlg::IDD, pParent)
	, m_evPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPCMDSD_ConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDL_FileList, m_lFileList);
	DDX_Control(pDX, IDB_AllRun, m_bAllRun);
	DDX_Control(pDX, IDB_AllListDelete, m_bAllListDelete);
	DDX_Control(pDX, IDB_Run, m_bRun);
	DDX_Control(pDX, IDB_ListDelete, m_bListDelete);
	DDX_Control(pDX, IDC_SamplingRate, m_cSamplingRate);
	DDX_Control(pDX, IDS_STATIC, m_sSamplingRate);
	DDX_Control(pDX, IDS_STATIC2, m_scPrecision);
	DDX_Control(pDX, IDC_Precision, m_ccPrecision);
	DDX_Control(pDX, IDS_STATIC3, m_scPath);
	DDX_Control(pDX, IDC_EditPath, m_ecPath);
	DDX_Control(pDX, IDC_PathCheck, m_bcPath);
	DDX_Text(pDX, IDC_EditPath, m_evPath);
	DDX_Text(pDX, IDC_EditPath, m_evPath);
}

BEGIN_MESSAGE_MAP(CPCMDSD_ConverterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDB_AllRun, &CPCMDSD_ConverterDlg::OnBnClickedAllrun)
	ON_BN_CLICKED(IDB_AllListDelete, &CPCMDSD_ConverterDlg::OnBnClickedAlllistdelete)
	ON_BN_CLICKED(IDB_Run, &CPCMDSD_ConverterDlg::OnBnClickedRun)
	ON_BN_CLICKED(IDB_ListDelete, &CPCMDSD_ConverterDlg::OnBnClickedListdelete)
	ON_WM_DROPFILES()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_PathCheck, &CPCMDSD_ConverterDlg::OnBnClickedPathcheck)
END_MESSAGE_MAP()


// CPCMDSD_ConverterDlg メッセージ ハンドラー

BOOL CPCMDSD_ConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	{
		setlocale(LC_CTYPE, "jpn");
		m_cSamplingRate.SetCurSel(0);
		m_ccPrecision.SetCurSel(0);
		DragAcceptFiles();
		ListInit();
		PathCheck();
		fftw_init_threads();
		m_dProgress.Create(ProgressDlg::IDD, this);
	}
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

//ファイルリスト設定
void CPCMDSD_ConverterDlg::ListInit() {
	m_lFileList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_lFileList.InsertColumn(m_lFileList.GetHeaderCtrl().GetItemCount(), L"ファイル名", LVCFMT_LEFT, 150);
	m_lFileList.InsertColumn(m_lFileList.GetHeaderCtrl().GetItemCount(), L"サンプリングレート", LVCFMT_RIGHT, 100);
	m_lFileList.InsertColumn(m_lFileList.GetHeaderCtrl().GetItemCount(), L"ビット数", LVCFMT_RIGHT, 70);
	m_lFileList.InsertColumn(m_lFileList.GetHeaderCtrl().GetItemCount(), L"ファイルパス", LVCFMT_LEFT, 100);
	m_lFileList.InsertColumn(m_lFileList.GetHeaderCtrl().GetItemCount(), L"ファイルサイズ", LVCFMT_RIGHT, 75);
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

//ウィンドウサイズ変更時のアイテム追従
void CPCMDSD_ConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//②ダイアログ全体の大きさ取得
		CRect rect;
		GetClientRect(&rect);

		//③サイズ調整
		m_lFileList.MoveWindow(0, 0, rect.Width(), rect.Height() - 55);
		m_bListDelete.MoveWindow(rect.Width() - 85, rect.Height() - 50, 80, 20);
		m_bRun.MoveWindow(rect.Width() - 170, rect.Height() - 50, 80, 20);
		m_bAllListDelete.MoveWindow(rect.Width() - 255, rect.Height() - 50, 80, 20);
		m_bAllRun.MoveWindow(rect.Width() - 340, rect.Height() - 50, 80, 20);
		m_cSamplingRate.MoveWindow(rect.Width() - 450, rect.Height() - 50, 80, 20);
		m_sSamplingRate.MoveWindow(rect.Width() - 535, rect.Height() - 46, 80, 20);
		m_ccPrecision.MoveWindow(rect.Width() - 450, rect.Height() - 25, 80, 20);
		m_scPrecision.MoveWindow(rect.Width() - 480, rect.Height() - 21, 80, 20);
		m_scPath.MoveWindow(rect.Width() - 350, rect.Height() - 21, 80, 20);
		m_ecPath.MoveWindow(rect.Width() - 310, rect.Height() - 25, 240, 20);
		m_bcPath.MoveWindow(rect.Width() - 65, rect.Height() - 25, 60, 20);
		CDialogEx::OnPaint();
	}
}


void CPCMDSD_ConverterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	Invalidate(TRUE);
}

//閉じる操作等無効化
void CPCMDSD_ConverterDlg::OnClose()
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	CDialogEx::OnClose();
}

void CPCMDSD_ConverterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CPCMDSD_ConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//Wavファイルチェック及びメタデータ読み取り
bool CPCMDSD_ConverterDlg::WAV_Metadata(TCHAR* filepath, wstring* metadata)
{
	TCHAR filename[512];
	wchar_t fileext[512];
	metadata[3] = filepath;
	_tsplitpath_s(filepath, NULL, 0, NULL, 0, filename, _countof(filename), fileext, _countof(fileext));
	metadata[0] = filename;
	bool flag = true;
	wstring wwav;
	string wav;
	wwav = fileext;
	if (wwav != L".wav" && wwav != L".WAV") {
		return false;
	}

	FILE* fprwav;
	errno_t error;

	if ((error = _wfopen_s(&fprwav, filepath, L"rb")) != 0) {
		return false;
	}

	unsigned __int32 samplingrate;
	unsigned short fmtID;
	unsigned short bitdepth;
	char tmp4[6];

	while (flag) {
		if (feof(fprwav)) {
			fclose(fprwav);
			return false;
		}
		fread(tmp4, 1, 1, fprwav);
		tmp4[1] = '\0';
		wav = tmp4;
		if (wav == "f") {
			fread(tmp4, 1, 3, fprwav);
			tmp4[3] = '\0';
			wav = tmp4;
			if (wav == "mt ") {
				flag = false;
			}
		}
	}

	//fmtIDでFloatInt判別
	_fseeki64(fprwav, 4, SEEK_CUR);
	fread(&fmtID, 2, 1, fprwav);
	if (fmtID == 3) {
		metadata[4] = L"bitFloat";
	}
	else if (fmtID == 1) {
		metadata[4] = L"bitInt";
	}
	else {
		fclose(fprwav);
		return false;
	}

	unsigned short chnum;
	fread(&chnum, 2, 1, fprwav);
	if (chnum != 2) {
		fclose(fprwav);
		return false;
	}

	fread(&samplingrate, 4, 1, fprwav);
	metadata[1] = to_wstring(samplingrate);
	if (samplingrate == 44100 || samplingrate == 44100 * 2 || samplingrate == 44100 * 4 || samplingrate == 44100 * 8 || samplingrate == 44100 * 16 ||
		samplingrate == 48000 || samplingrate == 48000 * 2 || samplingrate == 48000 * 4 || samplingrate == 48000 * 8 || samplingrate == 48000 * 16) {
		flag = true;
	}
	else {
		fclose(fprwav);
		return false;
	}

	_fseeki64(fprwav, 6, SEEK_CUR);
	fread(&bitdepth, 2, 1, fprwav);
	metadata[2] = to_wstring(bitdepth) + metadata[4];
	if (bitdepth == 16 || bitdepth == 24 || bitdepth == 32 || bitdepth == 64) {
		flag = true;
	}
	else {
		fclose(fprwav);
		return false;
	}
	while (flag) {
		if (feof(fprwav)) {
			fclose(fprwav);
			return false;
		}
		fread(tmp4, 1, 1, fprwav);
		tmp4[1] = L'\0';
		wav = tmp4;
		if (wav == "d") {
			fread(tmp4, 1, 3, fprwav);
			tmp4[3] = '\0';
			wav = tmp4;
			if (wav == "ata") {
				flag = false;
			}
		}
	}
	unsigned __int32 samplesize;
	fread(&samplesize, 4, 1, fprwav);
	metadata[4] = to_wstring(samplesize);

	fclose(fprwav);
	return true;
}

//DSDIFF形式で書き込み
bool CPCMDSD_ConverterDlg::DSD_Write(FILE* LData, FILE* RData, FILE* WriteData, int number) {
	int OrigSamplingRate = _ttoi(m_lFileList.GetItemText(number, 1));
	int BaseSamplingRate;
	if (OrigSamplingRate % 44100 == 0) {
		BaseSamplingRate = 44100;
	}
	else {
		BaseSamplingRate = 48000;
	}

	unsigned __int64 OrigBit = _ttoi(m_lFileList.GetItemText(number, 2).Left(2));
	unsigned __int64 OrigDataSize = _ttoi64(m_lFileList.GetItemText(number, 4)) / ((OrigBit / 8) * 2);
	_fseeki64(LData, 0, SEEK_SET);
	_fseeki64(RData, 0, SEEK_SET);
	int DSD_SamplingRate;

	switch (m_cSamplingRate.GetCurSel()) {
	case 0:
		DSD_SamplingRate = BaseSamplingRate * 16;
		break;
	case 1:
		DSD_SamplingRate = BaseSamplingRate * 32;
		break;
	case 2:
		DSD_SamplingRate = BaseSamplingRate * 64;
		break;
	case 3:
		DSD_SamplingRate = BaseSamplingRate * 128;
		break;
	case 4:
		DSD_SamplingRate = BaseSamplingRate * 256;
		break;
	case 5:
		DSD_SamplingRate = BaseSamplingRate * 512;
		break;
	case 6:
		DSD_SamplingRate = BaseSamplingRate * 1024;
		break;
	case 7:
		DSD_SamplingRate = BaseSamplingRate * 2048;
		break;
	}

	unsigned __int64 DSD_SampleSize = OrigDataSize * (DSD_SamplingRate / OrigSamplingRate);
	unsigned __int64 DSD_DataSize = DSD_SampleSize / 4;
	_fseeki64(LData, 0, SEEK_END);
	_fseeki64(RData, 0, SEEK_END);
	_fseeki64(LData, _ftelli64(LData) - DSD_SampleSize, SEEK_SET);
	_fseeki64(RData, _ftelli64(RData) - DSD_SampleSize, SEEK_SET);


	fwrite("FRM8", 4, 1, WriteData);//FRM8
	unsigned __int64 binary = 0;
	unsigned short ushort = 0;
	unsigned char uchar = 0;
	unsigned __int64 ulong = 0;
	unsigned __int64 tell = 0;
	binary = reverse_endian(DSD_DataSize + 152);
	fwrite(&binary, 8, 1, WriteData);//chunksize
	fwrite("DSD ", 4, 1, WriteData);//DSD

	fwrite("FVER", 4, 1, WriteData);//FVER
	binary = 0;
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	binary = reverse_endian(4);
	fwrite(&binary, 4, 1, WriteData);//Chunksize

	//Version
	binary = 1;
	fwrite(&binary, 1, 1, WriteData);
	binary = 5;
	fwrite(&binary, 1, 1, WriteData);
	binary = 0;
	fwrite(&binary, 1, 1, WriteData);
	binary = 0;
	fwrite(&binary, 1, 1, WriteData);

	fwrite("PROP", 4, 1, WriteData);//PROP
	binary = 0;
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	binary = reverse_endian(108);
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	fwrite("SND ", 4, 1, WriteData);//SND

	fwrite("FS  ", 4, 1, WriteData);//FS
	binary = 0;
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	binary = reverse_endian(4);
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	unsigned __int32 binary1;
	binary1 = reverse_endian(DSD_SamplingRate);
	fwrite(&binary1, 4, 1, WriteData);//SamplingRate

	fwrite("CHNL", 4, 1, WriteData);//CHNL
	binary = 0;
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	binary = reverse_endian(10);
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	binary = 0;//number of channel
	fwrite(&binary, 1, 1, WriteData);
	binary = 2;
	fwrite(&binary, 1, 1, WriteData);
	fwrite("SLFT", 4, 1, WriteData);//SLFT
	fwrite("SRGT", 4, 1, WriteData);//SRGT

	fwrite("CMPR", 4, 1, WriteData);//CMPR
	binary = 0;
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	binary = reverse_endian(20);
	fwrite(&binary, 4, 1, WriteData);//Chunksize

	fwrite("DSD ", 4, 1, WriteData);//DSD
	binary = 14;
	fwrite(&binary, 1, 1, WriteData);
	fwrite("not compressed ", 15, 1, WriteData);//not compressed

	fwrite("ABSS", 4, 1, WriteData);//ABSS
	binary = 0;
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	binary = reverse_endian(8);
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	fwrite(&ushort, 2, 1, WriteData);//Hours
	fwrite(&uchar, 1, 1, WriteData);//Minites
	fwrite(&uchar, 1, 1, WriteData);//Seconds
	fwrite(&ulong, 4, 1, WriteData);//samples

	fwrite("LSCO", 4, 1, WriteData);//LSCO
	binary = 0;
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	binary = reverse_endian(2);
	fwrite(&binary, 4, 1, WriteData);//Chunksize
	fwrite(&ushort, 2, 1, WriteData);//IsConfig

	fwrite("DSD ", 4, 1, WriteData);//DSD
	binary = reverse_endian(DSD_DataSize);
	fwrite(&binary, 8, 1, WriteData);//Chunksize
	unsigned __int64 i = 0;
	int buffersize = 16384 * 2 * 8;
	unsigned char* onebit = new unsigned char[buffersize / 4];
	unsigned char* tmpdataL = new unsigned char[buffersize];
	unsigned char* tmpdataR = new unsigned char[buffersize];
	unsigned char tmpL = 0; unsigned char tmpR = 0;
	int n = 0;
	int p = 0;
	int t = 0;
	unsigned __int64 k = 0;
	//WAV_FilterはLRごとUnsignedCharで書き出しているので、それを8サンプル1バイトにまとめてから
	//データ領域として書き出す
	for (i = 0; i < DSD_SampleSize / buffersize; i++) {
		p = 0;
		fread(tmpdataL, 1, buffersize, LData);
		fread(tmpdataR, 1, buffersize, RData);
		for (k = 0; k < buffersize / 4; k++) {
			onebit[k] = tmpdataL[p] << 7;
			onebit[k] += tmpdataL[p + 1] << 6;
			onebit[k] += tmpdataL[p + 2] << 5;
			onebit[k] += tmpdataL[p + 3] << 4;
			onebit[k] += tmpdataL[p + 4] << 3;
			onebit[k] += tmpdataL[p + 5] << 2;
			onebit[k] += tmpdataL[p + 6] << 1;
			onebit[k] += tmpdataL[p + 7] << 0;
			k++;
			onebit[k] = tmpdataR[p] << 7;
			onebit[k] += tmpdataR[p + 1] << 6;
			onebit[k] += tmpdataR[p + 2] << 5;
			onebit[k] += tmpdataR[p + 3] << 4;
			onebit[k] += tmpdataR[p + 4] << 3;
			onebit[k] += tmpdataR[p + 5] << 2;
			onebit[k] += tmpdataR[p + 6] << 1;
			onebit[k] += tmpdataR[p + 7] << 0;
			p += 8;
		}
		fwrite(onebit, 1, buffersize / 4, WriteData);//DSD_Data
	}

	for (k = i * buffersize / 4; k < DSD_DataSize; k += 2) {
		fread(tmpdataL, 1, 8, LData);
		fread(tmpdataR, 1, 8, RData);
		for (n = 0; n < 8; n++) {
			if (tmpdataL[n] == 1) {
				tmpL += unsigned char(pow(2, 7 - n));
			}
			if (tmpdataR[n] == 1) {
				tmpR += unsigned char(pow(2, 7 - n));
			}
		}
		fwrite(&tmpL, 1, 1, WriteData);
		fwrite(&tmpR, 1, 1, WriteData);
		tmpL = 0;
		tmpR = 0;
	}
	tell = _ftelli64(WriteData);

	delete[] onebit;
	delete[] tmpdataL;
	delete[] tmpdataR;
	return true;
}

//一時ファイル削除
bool CPCMDSD_ConverterDlg::TrushFile(TCHAR* filepath, CString flag) {
	wchar_t DirectoryName[512];
	wchar_t FileName[512];
	wchar_t FileExt[512];
	_wsplitpath_s(filepath, NULL, 0, DirectoryName, sizeof(DirectoryName) / sizeof(DirectoryName[0]),
		FileName, sizeof(FileName) / sizeof(FileName[0]), FileExt, sizeof(FileExt) / sizeof(FileExt[0]));

	size_t num;
	char DirectoryNameTmp[512];
	wcstombs_s(&num, DirectoryNameTmp, 512, DirectoryName, 512);
	string DirectorySingleName = DirectoryNameTmp;
	DirectorySingleName.erase(DirectorySingleName.size() - 1, 1);

	int n = (int)DirectorySingleName.rfind("\\", DirectorySingleName.size());
	DirectorySingleName.erase(0, DirectorySingleName.rfind("\\", n));
	mbstowcs_s(&num, DirectoryName, 512, DirectorySingleName.c_str(), 512);

	CString DeletePath;
	if (m_evPath == L"") {
		wchar_t cdir[512];
		GetCurrentDirectory(512, cdir);
		DeletePath = cdir;
		DeletePath += DirectoryName;
	}
	else {
		DeletePath = m_evPath;
	}
	DeletePath += _T("\\"); DeletePath += FileName + flag;
	if (!DeleteFileW(DeletePath)) {
		return false;
	}
	return true;
}

//読み書きデータ準備
bool CPCMDSD_ConverterDlg::RequireWriteData(TCHAR* filepath, CString flag, wchar_t* FileMode, FILE** WriteDatadsd) {
	wchar_t DirectoryName[512];
	wchar_t FileName[512];
	wchar_t FileExt[24];
	_wsplitpath_s(filepath, NULL, 0, DirectoryName, sizeof(DirectoryName) / sizeof(DirectoryName[0]),
		FileName, sizeof(FileName) / sizeof(FileName[0]), FileExt, sizeof(FileExt) / sizeof(FileExt[0]));

	size_t num;
	char DirectoryNameTmp[512];
	wcstombs_s(&num, DirectoryNameTmp, 512, DirectoryName, 512);
	string DirectorySingleName = DirectoryNameTmp;
	DirectorySingleName.erase(DirectorySingleName.size() - 1, 1);

	int n = (int)DirectorySingleName.rfind("\\", DirectorySingleName.size());
	DirectorySingleName.erase(0, DirectorySingleName.rfind("\\", n));
	mbstowcs_s(&num, DirectoryName, 512, DirectorySingleName.c_str(), 512);

	CString WritePath;
	if (m_evPath == L"") {
		wchar_t cdir[512];
		GetCurrentDirectory(512, cdir);
		WritePath = cdir;
		WritePath += DirectoryName;
	}
	else {
		WritePath = m_evPath;
	}

	CFileFind find;
	if (!PathFileExists(WritePath)) {
		CreateDirectory(WritePath, NULL);
	}
	WritePath += _T("\\"); WritePath += FileName; WritePath += flag;
	errno_t error_w;

	TCHAR errorMessage3[1024];
	_tcscpy_s(errorMessage3, WritePath);
	TCHAR* errorMessage4 = L"に書き込めませんでした";
	lstrcat(errorMessage3, errorMessage4);

	if (!wcscmp(FileMode, L"wb") && find.FindFile(WritePath)) {
		TCHAR errorMessage5[1024];
		_tcscpy_s(errorMessage5, WritePath);
		TCHAR* errorMessage6 = L"は既に存在します。上書きしますか？";
		lstrcat(errorMessage5, errorMessage6);
		unsigned int i;
		try {
			i = MessageBox(errorMessage5, L"上書き確認", MB_YESNO);
		}
		catch (...) { MessageBox(L"エラー", L"エラー", MB_OK); }
		int clickno = 0;
		try {
			clickno = IDNO;
		}
		catch (...) { MessageBox(L"エラー", L"エラー", MB_OK); }
		if (i == clickno) {
			return false;
		}
		else {
			if ((error_w = _wfopen_s(WriteDatadsd, WritePath, FileMode)) != 0) {
				MessageBox(errorMessage3, L"DSDIFFファイル書き込み失敗", MB_OK);
				return false;
			}
		}
	}
	else {
		if ((error_w = _wfopen_s(WriteDatadsd, WritePath, FileMode)) != 0) {
			MessageBox(errorMessage3, L"DSDIFFファイル書き込み失敗", MB_OK);
			return false;
		}
	}
	setvbuf(*WriteDatadsd, NULL, _IOFBF, 512 * 1024);

	return true;
}

////Wavファイルを64bitFloat(double)化し、LR分離して一時ファイルとして書き出し
bool CPCMDSD_ConverterDlg::TmpWriteData(TCHAR* filepath, FILE* tmpl, FILE* tmpr, int Times) {
	FILE* wavread;
	errno_t error;
	char filepath_tmp[512];
	bool flag = true;
	bool floatint = true;
	string wav;
	WideCharToMultiByte(CP_ACP, 0, filepath, -1, filepath_tmp, sizeof(filepath_tmp), NULL, NULL);

	if ((error = _wfopen_s(&wavread, filepath, L"rb")) != 0) {
		return false;
	}
	setvbuf(wavread, NULL, _IOFBF, 512 * 1024);

	char tmp4[6];

	while (flag) {
		if (feof(wavread)) {
			fclose(wavread);
			return false;
		}
		fread(tmp4, 1, 1, wavread);
		tmp4[1] = '\0';
		wav = tmp4;
		if (wav == "f") {
			fread(tmp4, 1, 3, wavread);
			tmp4[3] = '\0';
			wav = tmp4;
			if (wav == "mt ") {
				flag = false;
			}
		}
	}
	//fmtIDでFloatInt判別
	short fmtID;
	_fseeki64(wavread, 4, SEEK_CUR);
	fread(&fmtID, 2, 1, wavread);
	if (fmtID == 3) {
		floatint = false;
	}
	else {
		floatint = true;
	}

	unsigned short bitdepth;
	_fseeki64(wavread, 12, SEEK_CUR);
	fread(&bitdepth, 2, 1, wavread);
	flag = true;

	while (flag) {
		if (feof(wavread)) {
			fclose(wavread);
			return false;
		}
		fread(tmp4, 1, 1, wavread);
		tmp4[1] = '\0';
		wav = tmp4;
		if (wav == "d") {
			fread(tmp4, 1, 3, wavread);
			tmp4[3] = '\0';
			wav = tmp4;
			if (wav == "ata") {
				flag = false;
			}
		}
	}
	unsigned __int32 samplesize;
	fread(&samplesize, 4, 1, wavread);

	//データがDSD変換時にきれいに割り切れるように、頭にゼロフィルする
	ifstream ifs(".\\FIRFilter.dat");
	int section_1 = 0;
	string str;
	if (ifs.fail())
	{
		return false;
	}
	getline(ifs, str);
	section_1 = atoi(str.c_str());
	double* firfilter_table = new double[section_1];//
	__int64 i = 0;
	while (getline(ifs, str))
	{
		firfilter_table[i] = atof(str.c_str());
		i++;
	}

	ifs.close();
	__int64 buffer_int = 0;
	double buffer_double = 0;
	float buffer_float;
	double bit = pow(2, bitdepth - 1);
	unsigned __int64 writelength = samplesize / ((bitdepth / 8) * 2);
	__int64 fillsize = (section_1 + 1) * Times - (writelength % ((section_1 + 1) * Times));
	for (int i = 0; i < fillsize; i++) {
		fwrite(&buffer_double, 8, 1, tmpl);
		fwrite(&buffer_double, 8, 1, tmpr);
	}

	//各種フォーマットの値をdouble型変数に-1,1に正規化して入れる
	if (floatint) {
		for (int i = 0; i < writelength; i++) {
			if (!m_dProgress.Cancelbottun) return false;
			fread(&buffer_int, bitdepth / 8, 1, wavread);
			buffer_int = buffer_int << (64 - bitdepth);
			buffer_int = buffer_int >> (64 - bitdepth);
			buffer_double = buffer_int / bit;
			fwrite(&buffer_double, 8, 1, tmpl);

			fread(&buffer_int, bitdepth / 8, 1, wavread);
			buffer_int = buffer_int << (64 - bitdepth);
			buffer_int = buffer_int >> (64 - bitdepth);
			buffer_double = buffer_int / bit;
			fwrite(&buffer_double, 8, 1, tmpr);
		}
	}
	else if (bitdepth == 32) {
		if (!m_dProgress.Cancelbottun) return false;
		for (int i = 0; i < writelength; i++) {
			fread(&buffer_float, bitdepth / 8, 1, wavread);
			buffer_double = buffer_float;
			fwrite(&buffer_double, 8, 1, tmpl);
			fread(&buffer_float, bitdepth / 8, 1, wavread);
			buffer_double = buffer_float;
			fwrite(&buffer_double, 8, 1, tmpr);
		}
	}
	else {
		for (int i = 0; i < writelength; i++) {
			if (!m_dProgress.Cancelbottun) return false;
			fread(&buffer_double, bitdepth / 8, 1, wavread);
			fwrite(&buffer_double, 8, 1, tmpl);
			fread(&buffer_double, bitdepth / 8, 1, wavread);
			fwrite(&buffer_double, 8, 1, tmpr);
		}
	}


	_fseeki64(tmpl, 0, SEEK_SET);
	_fseeki64(tmpr, 0, SEEK_SET);
	fclose(wavread);
	delete[] firfilter_table;
	return true;
}

//FFTプラン初期化
//FFT FIRで処理は削減しているものの、アップサンプリングの最後の方ではさすがにFFTサイズが大きく、処理が重い
//FFTW_Wisdomや、CPU拡張命令を試したが、目に見える改善はせず。要対策。
void CPCMDSD_ConverterDlg::FFTInit(fftw_plan* plan, unsigned int fftsize, int Times, double* fftin, fftw_complex* fftout) {
	fftw_plan_with_nthreads(omp_get_num_procs() / 2);
	*plan = fftw_plan_dft_r2c_1d(int(fftsize / Times), fftin, fftout, FFTW_ESTIMATE);
}
void CPCMDSD_ConverterDlg::iFFTInit(fftw_plan* plan, unsigned int fftsize, int Times, fftw_complex* ifftin, double* ifftout) {
	fftw_plan_with_nthreads(omp_get_num_procs() / 2);
	*plan = fftw_plan_dft_c2r_1d(int(fftsize / Times), ifftin, ifftout, FFTW_ESTIMATE);
}

//改良版FIRフィルタ版PCM-DSD変換
bool CPCMDSD_ConverterDlg::WAV_Filter_Renew(FILE* UpSampleData, FILE* OrigData, unsigned int Times, omp_lock_t* myLock, unsigned int DSDsamplingRate) {
	omp_set_lock(myLock);
	//FIRフィルタ係数読み込み
	//タップ数は2^N-1
	ifstream ifs(".\\FIRFilter.dat");
	unsigned int section_1 = 0;
	string str;
	unsigned __int64 samplesize;
	if (ifs.fail())
	{
		return false;
	}
	getline(ifs, str);
	section_1 = atoi(str.c_str());
	double* firfilter_table = new double[section_1];
	unsigned __int32 i = 0;
	while (getline(ifs, str))
	{
		firfilter_table[i] = atof(str.c_str());
		i++;
	}

	ifs.close();

	//ノイズシェーピング係数読み込み
	ifstream ifsNS(".\\NoiseShapingCoeff.dat");
	if (ifsNS.fail())
	{
		exit(EXIT_FAILURE);
	}
	i = 0; int s = 0;
	getline(ifsNS, str);
	unsigned int order = atoi(str.c_str());

	double** NS = new double* [2];
	NS[0] = new double[order];
	NS[1] = new double[order];
	while (getline(ifsNS, str))
	{
		if (str != "0") {
			if (i == 0)
				NS[i][s] = atof(str.c_str());
			else {
				NS[i][order - s - 1] = atof(str.c_str());
			}
			s++;
		}
		else {
			s = 0;
			i++;
		}
	}
	ifsNS.close();
	for (i = 0; i < order; i++) {
		NS[1][i] = NS[1][i];
		NS[0][i] = NS[0][i] - NS[1][i];
	}

	_fseeki64(OrigData, 0, SEEK_END);
	samplesize = _ftelli64(OrigData);
	_fseeki64(OrigData, 0, SEEK_SET);
	samplesize = samplesize / 8;

	//FFT Overlap add Methodを用いたFIRフィルタ畳みこみ演算
	//x(L),h(N),FFT(M)としたとき、M>=L+N-1になる必要があるので
	//最終アップサンプリング時にM=2*L=2*(N+1)となるように定義
	const unsigned int logtimes = unsigned int(log(Times) / log(2));
	const unsigned int fftsize = (section_1 + 1) * Times;
	const unsigned int datasize = fftsize / 2;
	unsigned int* nowfftsize = new  unsigned int[logtimes];
	unsigned int* zerosize = new  unsigned int[logtimes];
	unsigned int* puddingsize = new  unsigned int[logtimes];
	unsigned int* realfftsize = new unsigned int[logtimes];
	unsigned int* addsize = new  unsigned int[logtimes];
	double** prebuffer = new double* [logtimes];
	double gain = 1;

	double* buffer = new double[fftsize];
	unsigned char* out = new unsigned char[datasize];
	for (i = 0; i < datasize; i++) {
		out[i] = 0;
	}

	double* deltabuffer = new double[order + 1];
	for (i = 0; i < order; i++) {
		deltabuffer[i] = 0;
	}

	double x_in = 0;
	double error_y = 0;
	double deltagain = 0.5;

	// FIR FFT用変数
	double** fftin = (double**)fftw_malloc(sizeof(double) * logtimes);
	fftw_complex** fftout = (fftw_complex**)fftw_malloc(sizeof(fftw_complex) * logtimes);
	fftw_complex** ifftin = (fftw_complex**)fftw_malloc(sizeof(fftw_complex) * logtimes);
	double** ifftout = (double**)fftw_malloc(sizeof(double) * logtimes);
	fftw_complex** firfilter_table_fft = (fftw_complex**)fftw_malloc(sizeof(fftw_complex) * logtimes);

	fftw_plan* FFT = (fftw_plan*)fftw_malloc(sizeof(fftw_plan) * (logtimes));
	fftw_plan* iFFT = (fftw_plan*)fftw_malloc(sizeof(fftw_plan) * (logtimes));

	unsigned int p = 0;
	unsigned int k = 0;
	unsigned int t = 0;
	unsigned int q = 0;
	for (i = 1; i < Times; i = i * 2) {
		nowfftsize[p] = fftsize / (Times / (i * 2));
		realfftsize[p] = nowfftsize[p] / 2 + 1;
		zerosize[p] = nowfftsize[p] / 4;
		puddingsize[p] = nowfftsize[p] - zerosize[p] * 2;
		gain = gain * (2.0 / nowfftsize[p]);
		addsize[p] = zerosize[p] * 2;

		prebuffer[p] = new double[fftsize];
		firfilter_table_fft[logtimes - p - 1] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * unsigned int(fftsize / i));
		fftin[logtimes - p - 1] = (double*)fftw_malloc(sizeof(double) * unsigned int(fftsize / i));
		fftout[logtimes - p - 1] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * unsigned int(fftsize / i));
		ifftin[logtimes - p - 1] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * unsigned int((fftsize / i + 1) / 2 + 1));
		ifftout[logtimes - p - 1] = (double*)fftw_malloc(sizeof(double) * unsigned int(fftsize / i));

		for (k = 0; k < fftsize / i; k++) {
			fftin[logtimes - p - 1][k] = 0;
			ifftout[logtimes - p - 1][k] = 0;
			fftout[logtimes - p - 1][k][0] = 0;
			fftout[logtimes - p - 1][k][1] = 0;
			ifftin[logtimes - p - 1][k / 2][0] = 0;
			ifftin[logtimes - p - 1][k / 2][1] = 0;
		}
		for (k = 0; k < fftsize; k++) {
			prebuffer[p][k] = 0;
		}
		p++;
	}
	p = 0;
	//!!!FFTW3ライブラリに同時アクセスすると落ちる
	for (i = 1; i < Times; i = i * 2) {
		FFTInit(&FFT[logtimes - p - 1], fftsize, int(i), fftin[logtimes - p - 1], fftout[logtimes - p - 1]);
		iFFTInit(&iFFT[logtimes - p - 1], fftsize, int(i), ifftin[logtimes - p - 1], ifftout[logtimes - p - 1]);
		p++;
	}
	///H(n)準備
	for (k = 0; k < logtimes; k++) {
		for (i = 0; i < section_1; i++) {
			fftin[k][i] = firfilter_table[i];
		}
		for (i = section_1; i < fftsize / pow(2, k); i++) {
			fftin[logtimes - k - 1][i] = 0;
		}
	}
	for (i = 0; i < logtimes; i++) {
		fftw_execute(FFT[logtimes - i - 1]);
		for (p = 0; p < fftsize / pow(2, i + 1) + 1; p++) {
			firfilter_table_fft[logtimes - i - 1][p][0] = fftout[logtimes - i - 1][p][0];
			firfilter_table_fft[logtimes - i - 1][p][1] = fftout[logtimes - i - 1][p][1];
		}
	}
	unsigned __int64 SplitNum = unsigned __int64((samplesize / datasize) * Times);
	deltagain = gain * deltagain;
	omp_unset_lock(myLock);
	for (i = 0; i < SplitNum; i++) {
		m_dProgress.Process(i + 1, unsigned __int32(SplitNum));//子ダイアログのプログレスバーに値を投げる
		fread(buffer, 8, datasize / Times, OrigData);
		for (t = 0; t < logtimes; t++) {
			q = 0;
			for (p = 0; p < zerosize[t]; p++) {
				fftin[t][q] = buffer[p];
				q++;
				fftin[t][q] = 0;
				q++;
			}
			memset(fftin[t] + q, 0, 8 * (nowfftsize[t] - q));
			fftw_execute(FFT[t]);
			for (p = 0; p < realfftsize[t]; p++) {
				ifftin[t][p][0] = fftout[t][p][0] * firfilter_table_fft[t][p][0] - fftout[t][p][1] * firfilter_table_fft[t][p][1];
				ifftin[t][p][1] = fftout[t][p][0] * firfilter_table_fft[t][p][1] + firfilter_table_fft[t][p][0] * fftout[t][p][1];
			}
			fftw_execute(iFFT[t]);
			for (p = 0; p < puddingsize[t]; p++) {
				buffer[p] = prebuffer[t][p] + ifftout[t][p];
			}
			q = 0;
			for (p = puddingsize[t]; p < nowfftsize[t]; p++) {
				buffer[p] = prebuffer[t][q] = ifftout[t][p];
				q++;
			}
		}
		//1bit化
		//Direct Form II型で⊿Σ変調
		for (q = 0; q < datasize; q++) {

			x_in = buffer[q] * deltagain;

			for (t = 0; t < order; t++) {
				x_in += NS[0][t] * deltabuffer[t];
			}

			if (x_in >= 0.0) {
				out[q] = 1;
				error_y = -1.0;
			}
			else {
				out[q] = 0;
				error_y = 1.0;
			}
			for (t = order; t > 0; t--) {
				deltabuffer[t] = deltabuffer[t - 1];
			}

			deltabuffer[0] = x_in + error_y;

			for (t = 0; t < order; t++) {
				deltabuffer[0] += NS[1][t] * deltabuffer[t + 1];
			}
		}
		fwrite(out, 1, datasize, UpSampleData);
		if (!m_dProgress.Cancelbottun) return false;//子ダイアログで中止ボタンが押された
	}


	//お掃除
	for (i = 0; i < logtimes; i++) {
		fftw_destroy_plan(FFT[i]);
		fftw_destroy_plan(iFFT[i]);
		fftw_free(ifftin[i]);
		fftw_free(ifftout[i]);
		fftw_free(fftin[i]);
		fftw_free(fftout[i]);
		fftw_free(firfilter_table_fft[i]);
		delete[] prebuffer[i];
	}
	fftw_free(iFFT);
	fftw_free(FFT);
	fftw_free(ifftin);
	fftw_free(ifftout);
	fftw_free(fftin);
	fftw_free(fftout);
	fftw_free(firfilter_table_fft);
	delete[] NS[0];
	delete[] NS[1];
	delete[] NS;
	delete[] nowfftsize;
	delete[] zerosize;
	delete[] puddingsize;
	delete[] realfftsize;
	delete[] addsize;
	delete[] out;
	delete[] prebuffer;
	delete[] buffer;
	delete[] deltabuffer;
	delete[] firfilter_table;
	return true;
}

//改良版IIRフィルタ版軽量PCM-DSD変換
bool CPCMDSD_ConverterDlg::WAV_FilterLight_Renew(FILE* UpSampleData, FILE* OrigData, unsigned int Times) {
	unsigned __int64 samplesize;
	_fseeki64(OrigData, 0, SEEK_END);
	samplesize = _ftelli64(OrigData);
	_fseeki64(OrigData, 0, SEEK_SET);
	samplesize = samplesize / 8;
	unsigned int SplitNum = 4096;
	const unsigned int logtimes = unsigned int(log(Times) / log(2));
	const unsigned int datasize = unsigned int(samplesize / SplitNum);
	const unsigned int Updatasize = datasize * Times;
	double* buffer = new double[Updatasize];
	double* databuffer = new double[Updatasize];

	unsigned char* out = new unsigned char[Updatasize];

	double A[12] = { 0 };
	double B[12] = { 0 };
	double deltabuffer[13] = { 0 };
	double x_in = 0;
	double error_y = 0;

	double deltagain = Times / 2;
	//IIRフィルタ係数読み込み
	unsigned int  section_1 = 0;
	unsigned int i = 0;
	ifstream ifs(".\\IIRFilter.dat");
	string str;
	unsigned int  s = 0;
	if (ifs.fail())
	{
		exit(EXIT_FAILURE);
	}
	getline(ifs, str);
	section_1 = atoi(str.c_str());
	getline(ifs, str);
	double** iirfilter_table = new double* [section_1];
	for (i = 0; i < section_1; i++) {
		iirfilter_table[i] = new double[6];
	}
	i = 0;
	while (getline(ifs, str))
	{
		if (str != "") {
			iirfilter_table[i][s] = atof(str.c_str());
			s++;
		}
		else {
			s = 0;
			i++;
		}
	}
	ifs.close();

	//ノイズシェーピング係数読み込み
	ifstream ifsNS(".\\NoiseShapingCoeff.dat");
	if (ifsNS.fail())
	{
		exit(EXIT_FAILURE);
	}
	i = 0; s = 0;
	getline(ifsNS, str);
	unsigned int order = atoi(str.c_str());
	double** NS = new double* [2];
	NS[0] = new double[order];
	NS[1] = new double[order];
	while (getline(ifsNS, str))
	{
		if (str != "0") {
			if (i == 0)
				NS[i][s] = atof(str.c_str());
			else {
				NS[i][order - s - 1] = atof(str.c_str());
			}
			s++;
		}
		else {
			s = 0;
			i++;
		}
	}
	ifsNS.close();
	for (i = 0; i < order; i++) {
		NS[1][i] = NS[1][i];
		NS[0][i] = NS[0][i] - NS[1][i];
	}

	double* nowdatasize = new double[logtimes];
	double*** qe_1 = new double** [logtimes];
	for (i = 0; i < logtimes; i++) {
		nowdatasize[i] = datasize * pow(2, i);
		qe_1[i] = new double* [section_1];
		for (s = 0; s < section_1; s++) {
			qe_1[i][s] = new double[3];
			qe_1[i][s][0] = 0; qe_1[i][s][1] = 0; qe_1[i][s][2] = 0;
		}
	}

	double tmp_iir = 0;
	unsigned int  x = 0;
	unsigned int a = 0; unsigned int p = 0; unsigned int q = 0; unsigned int t = 0; s = 0;

	for (i = 0; i < SplitNum; i++) {
		m_dProgress.Process(i + 1, SplitNum);//子ダイアログのプログレスバーに値を投げる
		fread(databuffer, 8, datasize, OrigData);
		s = 0;
		for (t = 0; t < logtimes; t++) {
			p = 0;
			for (q = 0; q < nowdatasize[t]; q++) {
				buffer[p] = databuffer[q];
				p++;
				buffer[p] = 0;
				p++;
			}
			for (q = 0; q < nowdatasize[t] * 2; q++) {

				tmp_iir = buffer[q];
				for (x = 0; x < section_1; x++) {
					qe_1[s][x][0] = iirfilter_table[x][0] * tmp_iir - iirfilter_table[x][1] * qe_1[s][x][1] - iirfilter_table[x][2] * qe_1[s][x][2];
					tmp_iir = iirfilter_table[x][3] * qe_1[s][x][0] + iirfilter_table[x][4] * qe_1[s][x][1] + iirfilter_table[x][5] * qe_1[s][x][2];
					qe_1[s][x][2] = qe_1[s][x][1];
					qe_1[s][x][1] = qe_1[s][x][0];
				}
				buffer[q] = tmp_iir;
				databuffer[q] = tmp_iir;
			}
			s++;
		}
		//1bit化
		for (q = 0; q < Updatasize; q++) {
			x_in = buffer[q] * deltagain;
			for (t = 0; t < order; t++) {
				x_in += NS[0][t] * deltabuffer[t];
			}

			if (x_in >= 0.0) {
				out[q] = 1;
				error_y = -1.0;
			}
			else {
				out[q] = 0;
				error_y = 1.0;
			}

			for (t = order; t > 0; t--) {
				deltabuffer[t] = deltabuffer[t - 1];
			}

			deltabuffer[0] = x_in + error_y;
			for (t = 0; t < order; t++) {
				deltabuffer[0] += NS[1][t] * deltabuffer[t + 1];
			}
		}
		fwrite(out, 1, Updatasize, UpSampleData);
		if (!m_dProgress.Cancelbottun) return false;//子ダイアログで中止ボタンが押された
	}

	//お掃除
	for (t = 0; t < logtimes; t++) {
		delete[] qe_1[t];
	}
	delete[] NS[0];
	delete[] NS[1];
	delete[] NS;
	delete[] qe_1;
	delete[] iirfilter_table;
	delete[] out;
	delete[] databuffer;
	delete[] buffer;
	delete[] nowdatasize;
	return true;
}

//フリーズ防止のためにスレッド作成
UINT __cdecl CPCMDSD_ConverterDlg::WorkThread(LPVOID pParam) {
	CPCMDSD_ConverterDlg* pDlg = (CPCMDSD_ConverterDlg*)pParam;
	int DSD_Times = 0;
	switch (pDlg->m_cSamplingRate.GetCurSel()) {
	case 0:
		DSD_Times = 4;
		break;
	case 1:
		DSD_Times = 5;
		break;
	case 2:
		DSD_Times = 6;
		break;
	case 3:
		DSD_Times = 7;
		break;
	case 4:
		DSD_Times = 8;
		break;
	case 5:
		DSD_Times = 9;
		break;
	case 6:
		DSD_Times = 10;
		break;
	case 7:
		DSD_Times = 11;
		break;
	}
	pDlg->Disable();
	if (pDlg->flag_Bottun == "All") {
		TCHAR runfile_tmp[512];
		int row_now = pDlg->m_lFileList.GetItemCount();
		int n = 0;
		int i = 0;
		for (i = 0; i < row_now; i++) {
			_tcscpy_s(runfile_tmp, pDlg->m_lFileList.GetItemText(n, 3));
			if (pDlg->WAV_Convert(runfile_tmp, n)) {
				pDlg->m_lFileList.DeleteItem(n);
			}
			else {
				int Times = 0;
				int samplingrate = _ttoi(pDlg->m_lFileList.GetItemText(n, 1));
				if (0 == samplingrate % 44100) {
					Times = DSD_Times - (samplingrate / 44100) + 1;
				}
				else {
					Times = DSD_Times - (samplingrate / 48000) + 1;
				}
				pDlg->TrushFile(runfile_tmp, _T("_tmpL0"));
				pDlg->TrushFile(runfile_tmp, _T("_tmpR0"));
				pDlg->TrushFile(runfile_tmp, _T("_tmpLDSD"));
				pDlg->TrushFile(runfile_tmp, _T("_tmpRDSD"));

				n++;
			}
		}
		if (i != 0) {
			pDlg->Enable();
			TCHAR errorMessage1[128];
			wsprintf(errorMessage1, TEXT("%d"), i - n);
			TCHAR* errorMessage2 = L"件の処理を実行しました";
			lstrcat(errorMessage1, errorMessage2);
			pDlg->MessageBox(errorMessage1, L"処理完了", MB_OK);
		}
	}
	else {
		int i = 0;
		int iRow;
		TCHAR runfile_tmp[512];
		POSITION pos = pDlg->m_lFileList.GetFirstSelectedItemPosition();
		while (pos) {
			iRow = pDlg->m_lFileList.GetNextSelectedItem(pos);
			_tcscpy_s(runfile_tmp, pDlg->m_lFileList.GetItemText(iRow - i, 3));
			if (pDlg->WAV_Convert(runfile_tmp, iRow - i)) {
				pDlg->m_lFileList.DeleteItem(iRow - i);
				i++;
			}
			else {
				pDlg->TrushFile(runfile_tmp, _T("_tmpL"));
				pDlg->TrushFile(runfile_tmp, _T("_tmpR"));
				pDlg->TrushFile(runfile_tmp, _T("_tmpLDSD"));
				pDlg->TrushFile(runfile_tmp, _T("_tmpRDSD"));
			}
		}
		if (i != 0) {
			pDlg->Enable();
			TCHAR errorMessage1[128];
			wsprintf(errorMessage1, TEXT("%d"), i);
			TCHAR* errorMessage2 = L"件の処理を実行しました";
			lstrcat(errorMessage1, errorMessage2);
			pDlg->MessageBox(errorMessage1, L"処理完了", MB_OK);
		}
	}
	return true;
}

//PCM-DSD変換の管理
bool CPCMDSD_ConverterDlg::WAV_Convert(TCHAR* filepath, int number) {
	m_dProgress.Start(filepath);
	m_dProgress.Process(0, 100);
	//精度取得
	int Precision = m_ccPrecision.GetCurSel();
	//DSDのサンプリングレートにするには何倍すればいいのか計算
	unsigned int DSD_Times;
	switch (m_cSamplingRate.GetCurSel()) {
	case 0:
		DSD_Times = (unsigned int)pow(2, 4);
		break;
	case 1:
		DSD_Times = (unsigned int)pow(2, 5);
		break;
	case 2:
		DSD_Times = (unsigned int)pow(2, 6);
		break;
	case 3:
		DSD_Times = (unsigned int)pow(2, 7);
		break;
	case 4:
		DSD_Times = (unsigned int)pow(2, 8);
		break;
	case 5:
		DSD_Times = (unsigned int)pow(2, 9);
		break;
	case 6:
		DSD_Times = (unsigned int)pow(2, 10);
		break;
	case 7:
		DSD_Times = (unsigned int)pow(2, 11);
		break;
	}
	unsigned int Times = 0;
	unsigned int DSDSamplingRate = 0;
	int samplingrate = _ttoi(m_lFileList.GetItemText(number, 1));
	if (0 == samplingrate % 44100) {
		Times = DSD_Times / (samplingrate / 44100);
		DSDSamplingRate = samplingrate * Times;
	}
	else {
		Times = DSD_Times / (samplingrate / 48000);
		DSDSamplingRate = samplingrate * Times;
	}
	bool flag = true;
	bool flagl = true;
	bool flagr = true;
	FILE* tmpl;
	FILE* tmpr;
	//LR分離&64bit化
	if (!RequireWriteData(filepath, _T("_tmpL0"), L"wb", &tmpl)) {
		flagl = false;
		flag = false;
	}
	if (!RequireWriteData(filepath, _T("_tmpR0"), L"wb", &tmpr)) {
		flagr = false;
		flag = false;
	}
	if (flag)if (!TmpWriteData(filepath, tmpl, tmpr, Times)) {
		flag = false;
	}
	if (flagl) {
		fclose(tmpl);
	}
	if (flagr) {
		fclose(tmpr);
	}
	omp_lock_t myLock;
	omp_init_lock(&myLock);
#pragma omp parallel
#pragma omp sections
	{
#pragma omp section
		{
			//Lch処理
			if (flag) {
				bool flagUpl = true;
				bool flagOrigl = true;
				FILE* tmpl;
				FILE* UpsampleDataL;
				omp_set_lock(&myLock);
				if (!RequireWriteData(filepath, _T("_tmpL0"), L"rb", &tmpl)) {
					flagOrigl = false;
					flag = false;
				}
				if (!RequireWriteData(filepath, _T("_tmpLDSD"), L"wb", &UpsampleDataL)) {
					flagUpl = false;
					flag = false;
				}
				omp_unset_lock(&myLock);
				if (Precision == 0) {
					if (flag)if (!WAV_Filter_Renew(UpsampleDataL, tmpl, Times, &myLock, DSDSamplingRate)) {
						flag = false;
					}
				}
				else {
					if (flag)if (!WAV_FilterLight_Renew(UpsampleDataL, tmpl, Times)) {
						flag = false;
					}
				}
				if (flagUpl) {
					fclose(UpsampleDataL);
				}
				if (flagOrigl) {
					fclose(tmpl);
				}
			}
		}
#pragma omp section  
		{
			//Rch処理
			if (flag) {
				bool flagUpr = true;
				bool flagOrigr = true;
				FILE* tmpr;
				FILE* UpsampleDataR;
				omp_set_lock(&myLock);
				if (!RequireWriteData(filepath, _T("_tmpR0"), L"rb", &tmpr)) {
					flagOrigr = false;
					flag = false;
				}
				if (!RequireWriteData(filepath, _T("_tmpRDSD"), L"wb", &UpsampleDataR)) {
					flagUpr = false;
					flag = false;
				}
				omp_unset_lock(&myLock);
				if (Precision == 0) {
					if (flag)if (!WAV_Filter_Renew(UpsampleDataR, tmpr, Times, &myLock, DSDSamplingRate)) {
						flag = false;
					}
				}
				else {
					if (flag)if (!WAV_FilterLight_Renew(UpsampleDataR, tmpr, Times)) {
						flag = false;
					}
				}
				if (flagUpr) {
					fclose(UpsampleDataR);
				}
				if (flagOrigr) {
					fclose(tmpr);
				}
			}
		}
	}
	omp_destroy_lock(&myLock);
	//LRマージ
	if (flag) {
		FILE* tmpDSD;
		CString DSDName;
		bool flagOrigl = true;
		bool flagOrigr = true;
		bool flagDSD = true;
		DSDName.Format(L"%d", (int)DSD_Times);
		if (!RequireWriteData(filepath, _T("_tmpLDSD"), L"rb", &tmpl)) {
			TrushFile(filepath, _T("_tmpLDSD"));
			flagOrigl = false;
			flag = false;
		}
		if (!RequireWriteData(filepath, _T("_tmpRDSD"), L"rb", &tmpr)) {
			TrushFile(filepath, _T("_tmpLDSD"));
			TrushFile(filepath, _T("_tmpRDSD"));
			flagOrigr = false;
			flag = false;
		}
		if (!RequireWriteData(filepath, _T("_DSD") + DSDName + _T(".dff"), L"wb", &tmpDSD)) {
			TrushFile(filepath, _T("_tmpLDSD"));
			TrushFile(filepath, _T("_tmpRDSD"));
			flagDSD = false;
			flag = false;
		}
		if (flag)if (!DSD_Write(tmpl, tmpr, tmpDSD, number)) {
			TrushFile(filepath, _T("_tmpLDSD"));
			TrushFile(filepath, _T("_tmpRDSD"));
		}
		if (flagOrigl) {
			fclose(tmpl);
		}
		if (flagOrigr) {
			fclose(tmpr);
		}
		if (flagDSD) {
			fclose(tmpDSD);
		}
	}

	TrushFile(filepath, _T("_tmpL0"));
	TrushFile(filepath, _T("_tmpR0"));
	TrushFile(filepath, _T("_tmpLDSD"));
	TrushFile(filepath, _T("_tmpRDSD"));

	if (!flag) {
		return false;
	}
	return true;
}

//Wavファイルがドロップされた時の初動
void CPCMDSD_ConverterDlg::WAV_FileRead(TCHAR* FileName) {
	wstring* metadata = new wstring[5];
	bool flag = true;

	if (WAV_Metadata(FileName, metadata)) {
		int row_now = m_lFileList.GetItemCount();
		for (int i = 0; i < row_now; i++) {
			if (metadata[3].c_str() == m_lFileList.GetItemText(i, 3)) {
				flag = false;
			}
		}
		if (flag) {
			m_lFileList.InsertItem(row_now, FileName);
			for (int n = 0; n < 5; n++) {
				wchar_t wBuffer[512];
				wcscpy_s(wBuffer, 512, metadata[n].c_str());
				m_lFileList.SetItemText(row_now, n, wBuffer);
			}
		}
	}
	else {
		TCHAR* errorMessage1 = FileName;
		TCHAR* errorMessage2 = L"はwavファイルではありません";
		lstrcat(errorMessage1, errorMessage2);
		MessageBox(errorMessage1, L"wavファイル読み込み失敗", MB_OK);
	}
	delete[] metadata;
}

//ディレクトリの再帰的検索
void CPCMDSD_ConverterDlg::DirectoryFind(TCHAR* DirectoryPath) {
	CFileFind find;
	TCHAR FilePathTemp[512];
	CString dirPath = DirectoryPath;
	dirPath += _T("\\*"); // ファイルの条件
	if (find.FindFile(dirPath)) {
		bool flag;
		do {
			flag = find.FindNextFile();
			_tcscpy_s(FilePathTemp, find.GetFilePath());
			// *.DSDIFFファイルが存在したときの処理
			if (!find.IsDots()) {
				if (find.IsDirectory()) {
					DirectoryFind(FilePathTemp);
				}
				else {
					CString Filepath = find.GetFilePath();
					wchar_t fileext[512];
					char fileext_tmp[512];
					size_t num;
					_wsplitpath_s(Filepath, NULL, 0, NULL, 0, NULL, 0, fileext, sizeof(fileext) / sizeof(fileext[0]));
					wcstombs_s(&num, fileext_tmp, 512, fileext, 512);
					string wav = fileext_tmp;
					if ((wav == ".wav") || (wav == ".WAV")) {
						WAV_FileRead(FilePathTemp);
					}
				}
			}
		} while (flag); // これなら対象ファイル全てが処理出来る
	}
}

//ファイル/ディレクトリがドロップ&ドラッグ
void CPCMDSD_ConverterDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	TCHAR FileName[512];
	int NameSize = sizeof(FileName);
	int FileNumber;
	CString str;
	int i;

	FileNumber = DragQueryFile(hDropInfo, 0xffffffff, FileName, NameSize);
	for (i = 0; i < FileNumber; i++) {
		DragQueryFile(hDropInfo, i, FileName, NameSize);
		if (PathIsDirectory(FileName)) {
			DirectoryFind(FileName);
		}
		else {
			WAV_FileRead(FileName);
		}
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

//処理中はボタンなど無効に
void CPCMDSD_ConverterDlg::Disable() {
	m_dProgress.EnableWindow(TRUE);
	m_dProgress.ShowWindow(TRUE);

	m_bAllRun.EnableWindow(FALSE);
	m_bAllListDelete.EnableWindow(FALSE);
	m_bRun.EnableWindow(FALSE);
	m_bListDelete.EnableWindow(FALSE);
	m_bcPath.EnableWindow(FALSE);

	m_cSamplingRate.EnableWindow(FALSE);
	m_ccPrecision.EnableWindow(FALSE);

	DragAcceptFiles(FALSE);
}

//処理が終わったらボタンなど有効化
void CPCMDSD_ConverterDlg::Enable() {
	m_dProgress.EnableWindow(FALSE);
	m_dProgress.ShowWindow(FALSE);
	m_dProgress.Cancelbottun = true;

	m_bAllRun.EnableWindow(TRUE);
	m_bAllListDelete.EnableWindow(TRUE);
	m_bRun.EnableWindow(TRUE);
	m_bListDelete.EnableWindow(TRUE);
	m_bcPath.EnableWindow(TRUE);

	m_cSamplingRate.EnableWindow(TRUE);
	m_ccPrecision.EnableWindow(TRUE);

	DragAcceptFiles(TRUE);
}

//全てを実行
void CPCMDSD_ConverterDlg::OnBnClickedAllrun()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	flag_Bottun = "All";
	if (m_lFileList.GetItemCount() != 0) {
		AfxBeginThread(WorkThread, this);
	}
}

//実行
void CPCMDSD_ConverterDlg::OnBnClickedRun()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	flag_Bottun == "One";
	if (m_lFileList.GetFirstSelectedItemPosition() != 0) {
		AfxBeginThread(WorkThread, this);
	}
}

//全てを削除
void CPCMDSD_ConverterDlg::OnBnClickedAlllistdelete()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_lFileList.DeleteAllItems();
}

//削除
void CPCMDSD_ConverterDlg::OnBnClickedListdelete()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int iRow = m_lFileList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	while (iRow != -1) {
		m_lFileList.DeleteItem(iRow);
		iRow = m_lFileList.GetNextItem(iRow - 1, LVNI_ALL | LVNI_SELECTED);
	}
}

//参照,ここから丸パクリ http://www.jade.dti.ne.jp/~arino/sample6.htm
void CPCMDSD_ConverterDlg::OnBnClickedPathcheck()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	BROWSEINFO bInfo;
	LPITEMIDLIST pIDList;
	TCHAR szDisplayName[MAX_PATH];

	// BROWSEINFO構造体に値を設定
	bInfo.hwndOwner = AfxGetMainWnd()->m_hWnd;		// ダイアログの親ウインドウのハンドル
	bInfo.pidlRoot = NULL;						// ルートフォルダを示すITEMIDLISTのポインタ (NULLの場合デスクトップフォルダが使われます）
	bInfo.pszDisplayName = szDisplayName;				// 選択されたフォルダ名を受け取るバッファのポインタ
	bInfo.lpszTitle = _T("フォルダの選択");		// ツリービューの上部に表示される文字列 
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;		// 表示されるフォルダの種類を示すフラグ
	bInfo.lpfn = NULL;						// BrowseCallbackProc関数のポインタ
	bInfo.lParam = (LPARAM)0;					// コールバック関数に渡す値

	// フォルダ選択ダイアログを表示
	pIDList = ::SHBrowseForFolder(&bInfo);
	if (pIDList == NULL) {

		// 戻り値がNULLの場合、フォルダが選択されずにダイアログが閉じられたことを意味します。

	}
	else {
		// ItemIDListをパス名に変換します
		if (!::SHGetPathFromIDList(pIDList, szDisplayName)) {
		}
		m_evPath = szDisplayName;
		// 最後にpIDListのポイントしているメモリを開放します
		::CoTaskMemFree(pIDList);
		UpdateData(FALSE);
	}
}

void CPCMDSD_ConverterDlg::PathCheck() {

	FILE* fprpath;
	errno_t err;
	char* ctx;
	char Path[513] = { 0 };
	if ((err = _wfopen_s(&fprpath, L"./directory.dat", L"r")) != 0) {
		return;
	}
	else {
		fgets(Path, 512, fprpath);
		fclose(fprpath);
		strtok_s(Path, "\n\0", &ctx);
		m_evPath = Path;
		UpdateData(FALSE);
	}
	return;
}

//閉じる動作をオーバーライド
void CPCMDSD_ConverterDlg::OnCancel() {
	_wsetlocale(LC_ALL, L"Japanese");
	wstring SPath = m_evPath;
	wofstream ofs("./directory.dat", ios::out | ios::trunc);
	ofs.imbue(std::locale("Japanese", LC_COLLATE));
	ofs.imbue(std::locale("Japanese", LC_CTYPE));

	if (ofs.fail())
	{
		m_dProgress.DestroyWindow();
		CDialogEx::OnCancel();
	}
	ofs << SPath.c_str() << std::endl;
	ofs.close();
	m_dProgress.DestroyWindow();
	CDialogEx::OnCancel();
}

//F1ヘルプ無効化
BOOL CPCMDSD_ConverterDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return TRUE;
}
