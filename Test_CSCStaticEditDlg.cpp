
// Test_CSCStaticEditDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Test_CSCStaticEdit.h"
#include "Test_CSCStaticEditDlg.h"
#include "afxdialogex.h"

#include "Common/Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestCSCStaticEditDlg 대화 상자



CTestCSCStaticEditDlg::CTestCSCStaticEditDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEST_CSCSTATICEDIT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestCSCStaticEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_EDIT, m_edit_static);
	DDX_Control(pDX, IDC_EDIT, m_edit);
	DDX_Control(pDX, IDC_STATIC_EDIT_ROUND, m_edit_round);
	DDX_Control(pDX, IDC_STATIC_EDIT_READONLY, m_edit_readonly);
	DDX_Control(pDX, IDC_STATIC_EDIT_PASSWORD, m_edit_password);
	DDX_Control(pDX, IDC_STATIC_DISABLED, m_edit_disabled);
	DDX_Control(pDX, IDC_COMBO_THEME, m_combo_theme);
	DDX_Control(pDX, IDC_COMBO_FONT, m_combo_font);
}

BEGIN_MESSAGE_MAP(CTestCSCStaticEditDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTestCSCStaticEditDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTestCSCStaticEditDlg::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_THEME, &CTestCSCStaticEditDlg::OnCbnSelchangeComboTheme)
	ON_WM_WINDOWPOSCHANGED()
	ON_CBN_SELCHANGE(IDC_COMBO_FONT, &CTestCSCStaticEditDlg::OnCbnSelchangeComboFont)
	ON_REGISTERED_MESSAGE(Message_CSCStaticEdit, &CTestCSCStaticEditDlg::on_message_CSCStaticEdit)
END_MESSAGE_MAP()


// CTestCSCStaticEditDlg 메시지 처리기

BOOL CTestCSCStaticEditDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_combo_theme.set_line_height(14);
	std::deque<CString> color_theme_list;
	CSCColorTheme::get_color_theme_list(color_theme_list);
	for (int i = 0; i < color_theme_list.size(); i++)
		m_combo_theme.AddString(color_theme_list[i]);

	int cur_theme = AfxGetApp()->GetProfileInt(_T("setting"), _T("cur color theme"), 0);
	Clamp(cur_theme, 0, (int)(color_theme_list.size() - 1));
	m_combo_theme.SetCurSel(cur_theme);
	apply_color_theme_to_edits(cur_theme);
	m_theme.set_color_theme(cur_theme);

	m_combo_font.set_as_font_combo();
	m_combo_font.set_line_height(14);

	// 레지스트리에 저장된 폰트가 있으면 콤보에서 선택하고 edit 들에도 적용
	CString cur_font = AfxGetApp()->GetProfileString(_T("setting"), _T("cur font"), _T(""));
	if (!cur_font.IsEmpty())
	{
		int idx = m_combo_font.find_string(cur_font);
		if (idx >= 0)
		{
			m_combo_font.SetCurSel(idx);
			apply_font_to_edits(cur_font);
		}
	}

	m_edit.SetWindowText(_T("Original CEdit"));
	m_edit_round.set_round(8);
	//m_edit_round.set_border_color(m_theme.cr_border_inactive, m_theme.cr_border_active);
	//m_edit_round.set_border_width(2);
	m_edit_readonly.set_readonly();
	m_edit_password.set_password_mode();
	m_edit_disabled.EnableWindow(FALSE);

	// "Dynamically" 레이블 옆(= m_edit_disabled 의 한 줄 아래)에 m_edit_dynamic 동적 생성
	CRect rect_password, rect_disabled;
	m_edit_password.GetWindowRect(&rect_password);
	m_edit_disabled.GetWindowRect(&rect_disabled);
	ScreenToClient(&rect_password);
	ScreenToClient(&rect_disabled);
	int row_pitch = rect_disabled.top - rect_password.top;
	CRect rect_dynamic = rect_disabled;
	rect_dynamic.OffsetRect(0, row_pitch);
	m_edit_dynamic.Create(WS_CHILD | WS_VISIBLE | WS_TABSTOP, rect_dynamic, this, IDC_STATIC_EDIT_DYNAMIC);
	m_edit_dynamic.SetWindowText(_T("Dynamically created"));
	m_edit_dynamic.set_color_theme(cur_theme, true);

	RestoreWindowPosition(&theApp, this);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestCSCStaticEditDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestCSCStaticEditDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestCSCStaticEditDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestCSCStaticEditDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}

void CTestCSCStaticEditDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}

// 모든 CSCStaticEdit에 컬러 테마를 일괄 적용
void CTestCSCStaticEditDlg::apply_color_theme_to_edits(int theme)
{
	m_edit_static.set_color_theme(theme, true);
	m_edit_round.set_color_theme(theme, true);
	m_edit_readonly.set_color_theme(theme, true);
	m_edit_password.set_color_theme(theme, true);
	m_edit_disabled.set_color_theme(theme, true);
	m_edit_dynamic.set_color_theme(theme, true);
}

// 모든 CSCStaticEdit에 폰트를 일괄 적용
void CTestCSCStaticEditDlg::apply_font_to_edits(const CString& font_name)
{
	m_edit_static.set_font_name(font_name);
	m_edit_round.set_font_name(font_name);
	m_edit_readonly.set_font_name(font_name);
	m_edit_password.set_font_name(font_name);
	m_edit_disabled.set_font_name(font_name);
	m_edit_dynamic.set_font_name(font_name);
}

void CTestCSCStaticEditDlg::OnCbnSelchangeComboTheme()
{
	int theme = m_combo_theme.GetCurSel();
	if (theme < 0) return;

	apply_color_theme_to_edits(theme);
	AfxGetApp()->WriteProfileInt(_T("setting"), _T("cur color theme"), theme);
}

void CTestCSCStaticEditDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialogEx::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	SaveWindowPosition(&theApp, this);
}

void CTestCSCStaticEditDlg::OnCbnSelchangeComboFont()
{
	CString font_name = m_combo_font.get_font_name();
	apply_font_to_edits(font_name);
	AfxGetApp()->WriteProfileString(_T("setting"), _T("cur font"), font_name);
}

LRESULT CTestCSCStaticEditDlg::on_message_CSCStaticEdit(WPARAM wParam, LPARAM lParam)
{
	CSCStaticEditMessage* pMsg = reinterpret_cast<CSCStaticEditMessage*>(wParam);
	if (!pMsg || pMsg->pThis == nullptr)
		return 0;

	if (pMsg->pThis == &m_edit_static)
	{
		switch (pMsg->message)
		{
			case CSCStaticEdit::message_scstaticedit_enter:
				SetWindowText(_T("Static Edit: Enter key pressed"));
				break;
			case CSCStaticEdit::message_scstaticedit_escape:
				SetWindowText(_T("Static Edit: Escape key pressed"));
				break;
			case CSCStaticEdit::message_scstaticedit_text_changed:
				SetWindowText(m_edit_static.get_text());
				break;
			default:
				break;
		}
	}
}