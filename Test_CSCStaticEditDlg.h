
// Test_CSCStaticEditDlg.h: 헤더 파일
//

#pragma once

#include "Common/CEdit/CSCStaticEdit/SCStaticEdit.h"
#include "Common/CComboBox/SCComboBox/SCComboBox.h"

// CTestCSCStaticEditDlg 대화 상자
class CTestCSCStaticEditDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestCSCStaticEditDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	// 동적 생성 컨트롤 (코드에서 Create() 호출)
	CSCStaticEdit		m_edit_dynamic;
	CSCColorTheme		m_theme = CSCColorTheme(this);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEST_CSCSTATICEDIT_DIALOG };
#endif

protected:
	// 모든 CSCStaticEdit에 컬러 테마를 일괄 적용
	void				apply_color_theme_to_edits(int theme);
	// 모든 CSCStaticEdit에 폰트를 일괄 적용
	void				apply_font_to_edits(const CString& font_name);

	LRESULT				on_message_CSCStaticEdit(WPARAM wParam, LPARAM lParam);

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CEdit m_edit;
	CSCStaticEdit m_edit_static;
	CSCStaticEdit m_edit_round;
	CSCStaticEdit m_edit_readonly;
	CSCStaticEdit m_edit_password;
	CSCStaticEdit m_edit_disabled;
	CSCComboBox m_combo_theme;
	afx_msg void OnCbnSelchangeComboTheme();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	CSCComboBox m_combo_font;
	afx_msg void OnCbnSelchangeComboFont();
};
