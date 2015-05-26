
// PCM-DSD_ConverterDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "PCM-DSD_Converter.h"
#include "PCM-DSD_ConverterDlg.h"
#include "afxdialogex.h"
#include <fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

template<class T>
T reverse_endian(T value)
{
	char * first = reinterpret_cast<char*>(&value);
	char* last = first + sizeof(T);
	std::reverse(first, last);
	return value;
}


// CPCMDSD_ConverterDlg ダイアログ
CPCMDSD_ConverterDlg::CPCMDSD_ConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPCMDSD_ConverterDlg::IDD, pParent)
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
	ON_CBN_SELCHANGE(IDC_SamplingRate, &CPCMDSD_ConverterDlg::OnCbnSelchangeSamplingrate)
	ON_WM_HELPINFO()
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
		DragAcceptFiles();
		ListInit();
		m_dProgress.Create(ProgressDlg::IDD, this);
	}
	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CPCMDSD_ConverterDlg::ListInit(){
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
		m_lFileList.MoveWindow(0, 0, rect.Width(), rect.Height() - 30);
		m_bListDelete.MoveWindow(rect.Width() - 85, rect.Height() - 25, 80, 20);
		m_bRun.MoveWindow(rect.Width() - 170, rect.Height() - 25, 80, 20);
		m_bAllListDelete.MoveWindow(rect.Width() - 255, rect.Height() - 25, 80, 20);
		m_bAllRun.MoveWindow(rect.Width() - 340, rect.Height() - 25, 80, 20);
		m_cSamplingRate.MoveWindow(rect.Width() - 450, rect.Height() - 25, 80, 20);
		m_sSamplingRate.MoveWindow(rect.Width() - 535, rect.Height() - 21, 80, 20);
		CDialogEx::OnPaint();
	}
}

void CPCMDSD_ConverterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	// TODO: ここにメッセージ ハンドラー コードを追加します。
	Invalidate(TRUE);
}

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

bool CPCMDSD_ConverterDlg::WAV_Metadata(TCHAR *filepath, string *metadata)
{
	char filepath_tmp[512];
	char filename[512];
	char fileext[512];
	WideCharToMultiByte(CP_ACP, 0, filepath, -1, filepath_tmp, sizeof(filepath_tmp), NULL, NULL);
	metadata[3] = filepath_tmp;
	_splitpath_s(filepath_tmp, NULL, 0, NULL, 0, filename, sizeof(filename), fileext, sizeof(fileext));
	metadata[0] = filename;
	BOOL flag = true;
	string wav;
	wav = fileext;
	if (wav != ".wav" && wav != ".WAV"){
		return false;
	}

	FILE *fprwav;
	errno_t error;

	if ((error = fopen_s(&fprwav, filepath_tmp, "rb")) != 0) {
		return false;
	}

	unsigned __int32 samplingrate;
	unsigned short fmtID;
	unsigned short bitdepth;
	char tmp4[6];

	while (flag){
		if (feof(fprwav)){
			fclose(fprwav);
			return false;
		}
		fread(tmp4, 1, 1, fprwav);
		tmp4[1] = '\0';
		wav = tmp4;
		if (wav == "f"){
			fread(tmp4, 1, 3, fprwav);
			tmp4[3] = '\0';
			wav = tmp4;
			if (wav == "mt "){
				flag = false;
			}
		}
	}

	_fseeki64(fprwav, 4, SEEK_CUR);
	fread(&fmtID, 2, 1, fprwav);
	if (fmtID == 3){
		metadata[4] = "bitFloat";
	}
	else if (fmtID == 1){
		metadata[4] = "bitInt";
	}
	else{
		fclose(fprwav);
		return false;
	}

	unsigned short chnum;
	fread(&chnum, 2, 1, fprwav);
	if (chnum != 2){
		fclose(fprwav);
		return false;
	}

	fread(&samplingrate, 4, 1, fprwav);
	metadata[1] = to_string(samplingrate);
	if (samplingrate == 44100 || samplingrate == 44100 * 2 || samplingrate == 44100 * 4 || samplingrate == 44100 * 8 || samplingrate == 44100 * 16 ||
		samplingrate == 48000 || samplingrate == 48000 * 2 || samplingrate == 48000 * 4 || samplingrate == 48000 * 8 || samplingrate == 48000 * 16){
		flag = true;
	}
	else{
		fclose(fprwav);
		return false;
	}

	_fseeki64(fprwav, 6, SEEK_CUR);
	fread(&bitdepth, 2, 1, fprwav);
	metadata[2] = to_string(bitdepth) + metadata[4];
	if (bitdepth == 16 || bitdepth == 24 || bitdepth == 32 || bitdepth == 64){
		flag = true;
	}
	else{
		fclose(fprwav);
		return false;
	}
	while (flag){
		if (feof(fprwav)){
			fclose(fprwav);
			return false;
		}
		fread(tmp4, 1, 1, fprwav);
		tmp4[1] = '\0';
		wav = tmp4;
		if (wav == "d"){
			fread(tmp4, 1, 3, fprwav);
			tmp4[3] = '\0';
			wav = tmp4;
			if (wav == "ata"){
				flag = false;
			}
		}
	}
	long samplesize;
	fread(&samplesize, 4, 1, fprwav);
	metadata[4] = to_string(samplesize);

	fclose(fprwav);
	return true;
}

bool TrushFile(TCHAR *filepath, CString flag){
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

	wchar_t cdir[512];
	GetCurrentDirectory(512, cdir);

	CString DeletePath = cdir; DeletePath += DirectoryName; DeletePath += _T("\\"); DeletePath += FileName + flag;
	if (!DeleteFileW(DeletePath)){
		return false;
	}
	return true;
}

bool CPCMDSD_ConverterDlg::RequireWriteData(TCHAR *filepath, CString flag, wchar_t *FileMode, FILE **WriteDatadsd){
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

	wchar_t cdir[512];
	GetCurrentDirectory(512, cdir);

	CFileFind find;
	CString WritePath = cdir; WritePath += DirectoryName;
	if (!PathFileExists(WritePath)){
		CreateDirectory(WritePath, NULL);
	}
	WritePath += _T("\\"); WritePath += FileName; WritePath += flag;
	errno_t error_w;

	TCHAR errorMessage3[512];
	_tcscpy_s(errorMessage3, WritePath);
	TCHAR *errorMessage4 = L"に書き込めませんでした";
	lstrcat(errorMessage3, errorMessage4);

	if (!wcscmp(FileMode, L"wb") && find.FindFile(WritePath)){
		TCHAR errorMessage5[512];
		_tcscpy_s(errorMessage5, WritePath);
		TCHAR *errorMessage6 = L"は既に存在します。上書きしますか？";
		lstrcat(errorMessage5, errorMessage6);
		int i = MessageBox(errorMessage5, L"上書き確認", MB_YESNO);
		if (i == IDNO){
			return false;
		}
		else{
			if ((error_w = _wfopen_s(WriteDatadsd, WritePath, FileMode)) != 0) {
				MessageBox(errorMessage3, L"DSDIFFファイル書き込み失敗", MB_OK);
				return false;
			}
		}
	}
	else{
		if ((error_w = _wfopen_s(WriteDatadsd, WritePath, FileMode)) != 0) {
			MessageBox(errorMessage3, L"DSDIFFファイル書き込み失敗", MB_OK);
			return false;
		}
	}
	setvbuf(*WriteDatadsd, NULL, _IOFBF, 512 * 1024);

	return true;
}

bool CPCMDSD_ConverterDlg::TmpWriteData(TCHAR *filepath, FILE *tmpl, FILE *tmpr, int Times){
	FILE *wavread;
	errno_t error;
	char filepath_tmp[512];
	bool flag = true;
	bool floatint = true;
	string wav;
	WideCharToMultiByte(CP_ACP, 0, filepath, -1, filepath_tmp, sizeof(filepath_tmp), NULL, NULL);

	if ((error = fopen_s(&wavread, filepath_tmp, "rb")) != 0) {
		return false;
	}
	setvbuf(wavread, NULL, _IOFBF, 512 * 1024);

	char tmp4[6];

	while (flag){
		if (feof(wavread)){
			fclose(wavread);
			return false;
		}
		fread(tmp4, 1, 1, wavread);
		tmp4[1] = '\0';
		wav = tmp4;
		if (wav == "f"){
			fread(tmp4, 1, 3, wavread);
			tmp4[3] = '\0';
			wav = tmp4;
			if (wav == "mt "){
				flag = false;
			}
		}
	}
	short fmtID;
	_fseeki64(wavread, 4, SEEK_CUR);
	fread(&fmtID, 2, 1, wavread);
	if (fmtID == 3){
		floatint = false;
	}
	else{
		floatint = true;
	}

	unsigned short bitdepth;
	_fseeki64(wavread, 12, SEEK_CUR);
	fread(&bitdepth, 2, 1, wavread);
	flag = true;

	while (flag){
		if (feof(wavread)){
			fclose(wavread);
			return false;
		}
		fread(tmp4, 1, 1, wavread);
		tmp4[1] = '\0';
		wav = tmp4;
		if (wav == "d"){
			fread(tmp4, 1, 3, wavread);
			tmp4[3] = '\0';
			wav = tmp4;
			if (wav == "ata"){
				flag = false;
			}
		}
	}
	long samplesize;
	fread(&samplesize, 4, 1, wavread);

	ifstream ifs(".\\FIRFilter.dat");
	int section_1 = 0;
	string str;
	if (ifs.fail())
	{
		return false;
	}
	getline(ifs, str);
	section_1 = atoi(str.c_str());
	double *firfilter_table = new double[section_1];//
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
	__int64 writelength = samplesize / ((bitdepth / 8) * 2);
	__int64 fillsize = (section_1 + 1)*Times - (writelength % ((section_1 + 1)*Times));
	for (int i = 0; i < fillsize; i++){
		fwrite(&buffer_double, 8, 1, tmpl);
		fwrite(&buffer_double, 8, 1, tmpr);
	}
	if (floatint){
		for (int i = 0; i < writelength; i++){
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
	else if (bitdepth == 32){
		if (!m_dProgress.Cancelbottun) return false;
		for (int i = 0; i < writelength; i++){
			fread(&buffer_float, bitdepth / 8, 1, wavread);
			buffer_double = buffer_float;
			fwrite(&buffer_double, 8, 1, tmpl);
			fread(&buffer_float, bitdepth / 8, 1, wavread);
			buffer_double = buffer_float;
			fwrite(&buffer_double, 8, 1, tmpr);
		}
	}
	else{
		for (int i = 0; i < writelength; i++){
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
	return true;
}

void CPCMDSD_ConverterDlg::FFTInit(fftw_plan *plan, __int64 fftsize, int Times, double *fftin, fftw_complex *ifftout){
	fftw_plan_with_nthreads(omp_get_max_threads() / 2);
	*plan = fftw_plan_dft_r2c_1d(int(fftsize / Times), fftin, ifftout, FFTW_ESTIMATE);
}

void CPCMDSD_ConverterDlg::iFFTInit(fftw_plan *plan, __int64 fftsize, int Times, fftw_complex *ifftin, double *fftout){
	fftw_plan_with_nthreads(omp_get_max_threads() / 2);
	*plan = fftw_plan_dft_c2r_1d(int(fftsize / Times), ifftin, fftout, FFTW_ESTIMATE);
}

bool CPCMDSD_ConverterDlg::WAV_Filter(FILE *UpSampleData, FILE *OrigData, int Times, omp_lock_t *myLock){
	//FIRフィルタ係数読み込み
	ifstream ifs(".\\FIRFilter.dat");
	int section_1 = 0;
	string str;
	__int64 samplesize;
	if (ifs.fail())
	{
		return false;
	}
	getline(ifs, str);
	section_1 = atoi(str.c_str());
	double *firfilter_table = new double[section_1];
	__int64 i = 0;
	while (getline(ifs, str))
	{
		firfilter_table[i] = atof(str.c_str());
		i++;
	}

	ifs.close();
	_fseeki64(OrigData, 0, SEEK_END);
	samplesize = _ftelli64(OrigData);
	_fseeki64(OrigData, 0, SEEK_SET);
	samplesize = samplesize / 8;


	const int logtimes = int(log(Times) / log(2));
	const int fftsize = (section_1 + 1) * Times;
	const int datasize = fftsize / 2;
	int *nowfftsize = new int[logtimes];
	int *zerosize = new int[logtimes];
	int *puddingsize = new int[logtimes];
	int *realfftsize = new int[logtimes];
	double *gain = new double[logtimes];
	double *buffer = new double[fftsize];
	double *databuffer = new double[datasize];
	double **prebuffer = new double*[logtimes];

	unsigned char *out = new unsigned char[unsigned int(datasize)];
	double shaper_coeffs_8th_b[8] = { 8.036523104430531e-01, -5.294484548544922e+00, 1.497412386332955e+01, -2.356658330575455e+01, 2.228874261804205e+01, -1.266723038877453e+01, 4.005297650249176e+00, -5.435177297167231e-01 };
	double shaper_coeffs_8th_a[8] = { -7.193145776600000e+00, 2.268630685861116e+01, -4.097785898127190e+01, 4.636939574322227e+01, -3.366324022655938e+01, 1.531356101838154e+01, -3.991500436793876e+00, 4.564822702832769e-01 };

	double quantizer_in = 0;
	double next = 0;
	double deltabuffer[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	double err = 0;
	double deltagain = 0.5;

	// FIR FFT用変数
	fftw_set_timelimit(5);
	double **fftin = (double**)fftw_malloc(sizeof(double) * logtimes);
	fftw_complex **fftout = (fftw_complex**)fftw_malloc(sizeof(fftw_complex) * logtimes);
	fftw_complex **ifftin = (fftw_complex**)fftw_malloc(sizeof(fftw_complex) * logtimes);
	double **ifftout = (double**)fftw_malloc(sizeof(double) * logtimes);
	fftw_complex **firfilter_table_fft = (fftw_complex**)fftw_malloc(sizeof(fftw_complex) * logtimes);

	fftw_plan *FFT = (fftw_plan*)fftw_malloc(sizeof(fftw_plan) * (logtimes));
	fftw_plan *iFFT = (fftw_plan*)fftw_malloc(sizeof(fftw_plan) * (logtimes));
	
	int p = 0;
	int k = 0;
	int t = 0;
	int q = 0;
	for (i = 1; i < Times; i = i * 2){
		nowfftsize[p] = int(fftsize / (Times / (i * 2)));
		realfftsize[p] = nowfftsize[p] / 2 + 1;
		gain[p] = (double)2.0 / nowfftsize[p];
		zerosize[p] = nowfftsize[p] / 2;
		puddingsize[p] = nowfftsize[p] - zerosize[p];

		prebuffer[p] = new double[fftsize];

		firfilter_table_fft[logtimes - p - 1] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * int(fftsize / i));
		fftin[logtimes - p - 1] = (double*)fftw_malloc(sizeof(double) * int(fftsize / i));
		fftout[logtimes - p - 1] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * int(fftsize / i));
		ifftin[logtimes - p - 1] = (fftw_complex*)fftw_malloc(sizeof(fftw_complex) * int((fftsize / i + 1) / 2 + 1));
		ifftout[logtimes - p - 1] = (double*)fftw_malloc(sizeof(double) * int(fftsize / i));

		for (k = 0; k < fftsize / i; k++){
			fftin[logtimes - p - 1][k] = 0;
			ifftout[logtimes - p - 1][k] = 0;
			fftout[logtimes - p - 1][k][0] = 0;
			fftout[logtimes - p - 1][k][1] = 0;
			ifftin[logtimes - p - 1][k / 2][0] = 0;
			ifftin[logtimes - p - 1][k / 2][1] = 0;
		}
		for (k = 0; k < fftsize; k++){
			prebuffer[p][k] = 0;
		}
		p++;
	}
	p = 0;
	//!!!FFTW3ライブラリに同時アクセスすると落ちる
	omp_set_lock(myLock);
	for (i = 1; i < Times; i = i * 2){
		FFTInit(&FFT[logtimes - p - 1], fftsize, int(i), fftin[logtimes - p - 1], fftout[logtimes - p - 1]);
		iFFTInit(&iFFT[logtimes - p - 1], fftsize, int(i), ifftin[logtimes - p - 1], ifftout[logtimes - p - 1]);
		p++;
	}
	omp_unset_lock(myLock);
	///H(n)準備
	for (k = 0; k < logtimes; k++){
		for (i = 0; i < section_1; i++){
			fftin[k][i] = firfilter_table[i];
		}
		for (i = section_1; i < fftsize / pow(2, k); i++){
			fftin[logtimes - k - 1][i] = 0;
		}
	}
	for (i = 0; i < logtimes; i++){
		fftw_execute(FFT[logtimes - i - 1]);
		for (p = 0; p < fftsize / pow(2, i + 1) + 1; p++){
			firfilter_table_fft[logtimes - i - 1][p][0] = fftout[logtimes - i - 1][p][0];
			firfilter_table_fft[logtimes - i - 1][p][1] = fftout[logtimes - i - 1][p][1];
		}
	}

	__int64 SplitNum = (samplesize / datasize)*Times;
	//Convolution&UpSampling
	for (i = 0; i < SplitNum; i++){
		m_dProgress.Process(int(i + 1), int(SplitNum));
		fread(buffer, 8, datasize / Times, OrigData);
		for (t = 0; t < logtimes; t++){
			for (p = 0; p < zerosize[t]; p++){
				fftin[t][p] = buffer[p / 2];
				p++;
				fftin[t][p] = 0;
			}
			for (p = zerosize[t]; p < nowfftsize[t]; p++){
				fftin[t][p] = 0;
			}
			fftw_execute(FFT[t]);
			for (p = 0; p < realfftsize[t]; p++){
				ifftin[t][p][0] = fftout[t][p][0] * firfilter_table_fft[t][p][0] - fftout[t][p][1] * firfilter_table_fft[t][p][1];
				ifftin[t][p][1] = fftout[t][p][0] * firfilter_table_fft[t][p][1] + firfilter_table_fft[t][p][0] * fftout[t][p][1];
			}
			fftw_execute(iFFT[t]);
			for (p = 0; p < nowfftsize[t]; p++){
				buffer[p] = ifftout[t][p] * gain[t];
			}
			for (p = 0; p < puddingsize[t]; p++){
				buffer[p] = prebuffer[t][p] + buffer[p];
				prebuffer[t][p] = buffer[zerosize[t] + p];
			}
		}
		//1bit化
		for (q = 0; q < datasize; q++){
			quantizer_in = buffer[q] * deltagain +
				deltabuffer[0] * shaper_coeffs_8th_b[0] +
				deltabuffer[1] * shaper_coeffs_8th_b[1] +
				deltabuffer[2] * shaper_coeffs_8th_b[2] +
				deltabuffer[3] * shaper_coeffs_8th_b[3] +
				deltabuffer[4] * shaper_coeffs_8th_b[4] +
				deltabuffer[5] * shaper_coeffs_8th_b[5] +
				deltabuffer[6] * shaper_coeffs_8th_b[6] +
				deltabuffer[7] * shaper_coeffs_8th_b[7];
			next =
				deltabuffer[0] * shaper_coeffs_8th_a[0] +
				deltabuffer[1] * shaper_coeffs_8th_a[1] +
				deltabuffer[2] * shaper_coeffs_8th_a[2] +
				deltabuffer[3] * shaper_coeffs_8th_a[3] +
				deltabuffer[4] * shaper_coeffs_8th_a[4] +
				deltabuffer[5] * shaper_coeffs_8th_a[5] +
				deltabuffer[6] * shaper_coeffs_8th_a[6] +
				deltabuffer[7] * shaper_coeffs_8th_a[7];

			if (quantizer_in < 0) {
				err = quantizer_in + 1.0;
				out[q] = 0;
			}
			else {
				err = quantizer_in - 1.0;
				out[q] = 1;
			}

			deltabuffer[7] = deltabuffer[6];
			deltabuffer[6] = deltabuffer[5];
			deltabuffer[5] = deltabuffer[4];
			deltabuffer[4] = deltabuffer[3];
			deltabuffer[3] = deltabuffer[2];
			deltabuffer[2] = deltabuffer[1];
			deltabuffer[1] = deltabuffer[0];
			deltabuffer[0] = err - next;
		}
		fwrite(out, 1, datasize, UpSampleData);
		if (!m_dProgress.Cancelbottun) return false;
	}

	//お掃除
	for (i = 0; i < logtimes; i++){
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
	delete[] nowfftsize;
	delete[] zerosize;
	delete[] puddingsize;
	delete[] realfftsize;
	delete[] gain;
	delete[] out;
	delete[] databuffer;
	delete[] prebuffer;
	delete[] buffer;
	delete[] firfilter_table;
	return true;
}

UINT __cdecl CPCMDSD_ConverterDlg::WorkThread(LPVOID pParam){
	CPCMDSD_ConverterDlg* pDlg = (CPCMDSD_ConverterDlg*)pParam;
	int DSD_Times;
	switch (pDlg->m_cSamplingRate.GetCurSel()) {
	case 0:
		DSD_Times = 6;
		break;
	case 1:
		DSD_Times = 7;
		break;
	case 2:
		DSD_Times = 8;
		break;
	case 3:
		DSD_Times = 9;
		break;
	case 4:
		DSD_Times = 10;
		break;
	case 5:
		DSD_Times = 11;
		break;
	}
	pDlg->Disable();
	if (pDlg->flag_Bottun == "All"){
		TCHAR runfile_tmp[512];
		int row_now = pDlg->m_lFileList.GetItemCount();
		int n = 0;
		int i = 0;
		for (i = 0; i < row_now; i++){
			_tcscpy_s(runfile_tmp, pDlg->m_lFileList.GetItemText(n, 3));
			if (pDlg->WAV_Convert(runfile_tmp, n)){
				pDlg->m_lFileList.DeleteItem(n);
			}
			else{
				int Times = 0;
				int samplingrate = _ttoi(pDlg->m_lFileList.GetItemText(n, 1));
				if (0 == samplingrate % 44100){
					Times = DSD_Times - (samplingrate / 44100) + 1;
				}
				else{
					Times = DSD_Times - (samplingrate / 48000) + 1;
				}
				for (int i = 0; i < Times; i++){
					CString TimesStr;
					TimesStr.Format(L"%d", Times);
					TrushFile(runfile_tmp, _T("_tmpL") + TimesStr);
					TrushFile(runfile_tmp, _T("_tmpR") + TimesStr);
				}
				n++;
			}
		}
		if (i != 0){
			pDlg->Enable();
			TCHAR errorMessage1[128];
			wsprintf(errorMessage1, TEXT("%d"), i - n);
			TCHAR *errorMessage2 = L"件の処理を実行しました";
			lstrcat(errorMessage1, errorMessage2);
			pDlg->MessageBox(errorMessage1, L"処理完了", MB_OK);
		}
	}
	else{
		int i = 0;
		int iRow;
		TCHAR runfile_tmp[512];
		POSITION pos = pDlg->m_lFileList.GetFirstSelectedItemPosition();
		while (pos){
			iRow = pDlg->m_lFileList.GetNextSelectedItem(pos);
			_tcscpy_s(runfile_tmp, pDlg->m_lFileList.GetItemText(iRow - i, 3));
			if (pDlg->WAV_Convert(runfile_tmp, iRow - i)){
				pDlg->m_lFileList.DeleteItem(iRow - i);
				i++;
			}
			else{
				int Times = 0;
				int samplingrate = _ttoi(pDlg->m_lFileList.GetItemText(iRow - i, 1));
				if (0 == samplingrate % 44100){
					Times = DSD_Times - (samplingrate / 44100) + 1;
				}
				else{
					Times = DSD_Times - (samplingrate / 48000) + 1;
				}
				for (int i = 0; i < Times; i++){
					CString TimesStr;
					TimesStr.Format(L"%d", Times);
					TrushFile(runfile_tmp, _T("_tmpL") + TimesStr);
					TrushFile(runfile_tmp, _T("_tmpR") + TimesStr);
				}
			}
		}
		if (i != 0){
			pDlg->Enable();
			TCHAR errorMessage1[128];
			wsprintf(errorMessage1, TEXT("%d"), i);
			TCHAR *errorMessage2 = L"件の処理を実行しました";
			lstrcat(errorMessage1, errorMessage2);
			pDlg->MessageBox(errorMessage1, L"処理完了", MB_OK);
		}
	}
	return TRUE;
}

bool CPCMDSD_ConverterDlg::WAV_Convert(TCHAR *filepath, int number){
	m_dProgress.Start(filepath);
	m_dProgress.Process(0, 100);
	int DSD_Times;
	switch (m_cSamplingRate.GetCurSel()) {
	case 0:
		DSD_Times = (int)pow(2, 6);
		break;
	case 1:
		DSD_Times = (int)pow(2, 7);
		break;
	case 2:
		DSD_Times = (int)pow(2, 8);
		break;
	case 3:
		DSD_Times = (int)pow(2, 9);
		break;
	case 4:
		DSD_Times = (int)pow(2, 10);
		break;
	case 5:
		DSD_Times = (int)pow(2, 11);
		break;
	}
	int Times = 0;
	int samplingrate = _ttoi(m_lFileList.GetItemText(number, 1));
	if (0 == samplingrate % 44100){
		Times = DSD_Times / (samplingrate / 44100);
	}
	else{
		Times = DSD_Times / (samplingrate / 48000);
	}
	bool flag = true;
	bool flagl = true;
	bool flagr = true;
	FILE *tmpl;
	FILE *tmpr;
	//LR分離&64bit化
	if (!RequireWriteData(filepath, _T("_tmpL0"), L"wb", &tmpl)){
		flagl = false;
		flag = false;
	}
	if (!RequireWriteData(filepath, _T("_tmpR0"), L"wb", &tmpr)){
		flagr = false;
		flag = false;
	}
	if (flag)if (!TmpWriteData(filepath, tmpl, tmpr, Times)){
		flag = false;
	}
	if (flagl){
		fclose(tmpl);
	}
	if (flagr){
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
		if (flag){
			bool flagUpl = true;
			bool flagOrigl = true;
			FILE *tmpl;
			FILE *UpsampleDataL;
			if (!RequireWriteData(filepath, _T("_tmpL0"), L"rb", &tmpl)){
				flagOrigl = false;
				flag = false;
			}
			if (!RequireWriteData(filepath, _T("_tmpLDSD"), L"wb", &UpsampleDataL)){
				flagUpl = false;
				flag = false;
			}
			if (flag)if (!WAV_Filter(UpsampleDataL, tmpl, Times, &myLock)){
				flag = false;
			}
			if (flagUpl){
				fclose(UpsampleDataL);
			}
			if (flagOrigl){
				fclose(tmpl);
			}
		}
	}
#pragma omp section  
	{
		//Rch処理
		if (flag){
			bool flagUpr = true;
			bool flagOrigr = true;
			FILE *tmpr;
			FILE *UpsampleDataR;
			if (!RequireWriteData(filepath, _T("_tmpR0"), L"rb", &tmpr)){
				flagOrigr = false;
				flag = false;
			}
			if (!RequireWriteData(filepath, _T("_tmpRDSD"), L"wb", &UpsampleDataR)){
				flagUpr = false;
				flag = false;
			}
			if (flag)if (!WAV_Filter(UpsampleDataR, tmpr, Times, &myLock)){
				flag = false;
			}
			if (flagUpr){
				fclose(UpsampleDataR);
			}
			if (flagOrigr){
				fclose(tmpr);
			}
		}
	}
	}
	//LRマージ
	if (flag){
		FILE *tmpDSD;
		CString DSDName;
		bool flagOrigl = true;
		bool flagOrigr = true;
		bool flagDSD = true;
		DSDName.Format(L"%d", (int)DSD_Times);
		if (!RequireWriteData(filepath, _T("_tmpLDSD"), L"rb", &tmpl)){
			TrushFile(filepath, _T("_tmpLDSD"));
			flagOrigl = false;
			flag = false;
		}
		if (!RequireWriteData(filepath, _T("_tmpRDSD"), L"rb", &tmpr)){
			TrushFile(filepath, _T("_tmpLDSD"));
			TrushFile(filepath, _T("_tmpRDSD"));
			flagOrigr = false;
			flag = false;
		}
		if (!RequireWriteData(filepath, _T("_DSD") + DSDName + _T(".dff"), L"wb", &tmpDSD)){
			TrushFile(filepath, _T("_tmpLDSD"));
			TrushFile(filepath, _T("_tmpRDSD"));
			flagDSD = false;
			flag = false;
		}
		if (flag)if (!DSD_Write(tmpl, tmpr, tmpDSD, number)){
			TrushFile(filepath, _T("_tmpLDSD"));
			TrushFile(filepath, _T("_tmpRDSD"));
		}
		if (flagOrigl){
			fclose(tmpl);
		}
		if (flagOrigr){
			fclose(tmpr);
		}
		if (flagDSD){
			fclose(tmpDSD);
		}
	}

	TrushFile(filepath, _T("_tmpL0"));
	TrushFile(filepath, _T("_tmpR0"));
	TrushFile(filepath, _T("_tmpLDSD"));
	TrushFile(filepath, _T("_tmpRDSD"));

	if (!flag){
		return false;
	}
	return true;
}

void CPCMDSD_ConverterDlg::WAV_FileRead(TCHAR *FileName){
	string *metadata = new string[5];
	bool flag = true;

	if (WAV_Metadata(FileName, metadata)){
		int row_now = m_lFileList.GetItemCount();
		for (int i = 0; i < row_now; i++){
			if (metadata[3].c_str() == m_lFileList.GetItemText(i, 3)){
				flag = false;
			}
		}
		if (flag){
			m_lFileList.InsertItem(row_now, FileName);
			for (int n = 0; n < 5; n++){
				wchar_t wBuffer[512];
				char cBuffer[512];
				strcpy_s(cBuffer, metadata[n].c_str());
				MultiByteToWideChar(CP_ACP, NULL, cBuffer, -1, wBuffer, sizeof(wBuffer));
				m_lFileList.SetItemText(row_now, n, wBuffer);
			}
		}
	}
	else{
		TCHAR *errorMessage1 = FileName;
		TCHAR *errorMessage2 = L"はwavファイルではありません";
		lstrcat(errorMessage1, errorMessage2);
		MessageBox(errorMessage1, L"wavファイル読み込み失敗", MB_OK);
	}
	delete[] metadata;
}

void CPCMDSD_ConverterDlg::DirectoryFind(TCHAR *DirectoryPath){
	CFileFind find;
	TCHAR FilePathTemp[512];
	CString dirPath = DirectoryPath;
	dirPath += _T("\\*"); // ファイルの条件
	if (find.FindFile(dirPath)){
		BOOL flag;
		do{
			flag = find.FindNextFile();
			_tcscpy_s(FilePathTemp, find.GetFilePath());
			// *.DSDIFFファイルが存在したときの処理
			if (!find.IsDots()){
				if (find.IsDirectory()){
					DirectoryFind(FilePathTemp);
				}
				else{
					CString Filepath = find.GetFilePath();
					wchar_t fileext[512];
					char fileext_tmp[512];
					size_t num;
					_wsplitpath_s(Filepath, NULL, 0, NULL, 0, NULL, 0, fileext, sizeof(fileext) / sizeof(fileext[0]));
					wcstombs_s(&num, fileext_tmp, 512, fileext, 512);
					string wav = fileext_tmp;
					if ((wav == ".wav") || (wav == ".WAV")){
						WAV_FileRead(FilePathTemp);
					}
				}
			}
		} while (flag); // これなら対象ファイル全てが処理出来る
	}
}

void CPCMDSD_ConverterDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	TCHAR FileName[512];
	int NameSize = sizeof(FileName);
	int FileNumber;
	CString str;
	int i;

	FileNumber = DragQueryFile(hDropInfo, 0xffffffff, FileName, NameSize);
	for (i = 0; i < FileNumber; i++){
		DragQueryFile(hDropInfo, i, FileName, NameSize);
		if (PathIsDirectory(FileName)){
			DirectoryFind(FileName);
		}
		else{
			WAV_FileRead(FileName);
		}
	}
	CDialogEx::OnDropFiles(hDropInfo);
}

void CPCMDSD_ConverterDlg::Disable(){
	m_dProgress.EnableWindow(TRUE);
	m_dProgress.ShowWindow(TRUE);
	CButton *button0 = (CButton*)GetDlgItem(IDB_AllRun);
	CButton *button1 = (CButton*)GetDlgItem(IDB_AllListDelete);
	CButton *button2 = (CButton*)GetDlgItem(IDB_Run);
	CButton *button3 = (CButton*)GetDlgItem(IDB_ListDelete);
	CWnd *box = (CWnd*)GetDlgItem(IDC_SamplingRate);

	button0->EnableWindow(FALSE);
	button1->EnableWindow(FALSE);
	button2->EnableWindow(FALSE);
	button3->EnableWindow(FALSE);
	box->EnableWindow(FALSE);
	DragAcceptFiles(FALSE);
}

void CPCMDSD_ConverterDlg::Enable(){
	m_dProgress.EnableWindow(FALSE);
	m_dProgress.ShowWindow(FALSE);
	m_dProgress.Cancelbottun = true;
	CButton *button0 = (CButton*)GetDlgItem(IDB_AllRun);
	CButton *button1 = (CButton*)GetDlgItem(IDB_AllListDelete);
	CButton *button2 = (CButton*)GetDlgItem(IDB_Run);
	CButton *button3 = (CButton*)GetDlgItem(IDB_ListDelete);
	CWnd *box = (CWnd*)GetDlgItem(IDC_SamplingRate);

	button0->EnableWindow(TRUE);
	button1->EnableWindow(TRUE);
	button2->EnableWindow(TRUE);
	button3->EnableWindow(TRUE);
	box->EnableWindow(TRUE);
	DragAcceptFiles(TRUE);
}

void CPCMDSD_ConverterDlg::OnBnClickedAllrun()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	flag_Bottun = "All";
	if (m_lFileList.GetItemCount() != 0){
		AfxBeginThread(WorkThread, this);
	}
}

void CPCMDSD_ConverterDlg::OnBnClickedRun()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	flag_Bottun == "One";
	if (m_lFileList.GetFirstSelectedItemPosition() != 0){
		AfxBeginThread(WorkThread, this);
	}
}

void CPCMDSD_ConverterDlg::OnBnClickedAlllistdelete()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_lFileList.DeleteAllItems();
}

void CPCMDSD_ConverterDlg::OnBnClickedListdelete()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	int iRow = m_lFileList.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	while (iRow != -1){
		m_lFileList.DeleteItem(iRow);
		iRow = m_lFileList.GetNextItem(iRow - 1, LVNI_ALL | LVNI_SELECTED);
	}
}

void CPCMDSD_ConverterDlg::OnCbnSelchangeSamplingrate()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}

void CPCMDSD_ConverterDlg::OnCancel(){
	m_dProgress.DestroyWindow();
	CDialogEx::OnCancel();
}

BOOL CPCMDSD_ConverterDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return true;
}
