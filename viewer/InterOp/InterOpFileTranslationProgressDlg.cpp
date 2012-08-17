#include "stdafx.h"
#include "InterOpFileTranslationProgressDlg.h"
#include "HUtilityLocaleString.h"
#include "Windows.h"

// CInterOpFileTranslationProgressDlg dialog

IMPLEMENT_DYNAMIC(CInterOpFileTranslationProgressDlg, CDialog)

CInterOpFileTranslationProgressDlg::CInterOpFileTranslationProgressDlg ( CWnd * pParent )
	: CDialog(CInterOpFileTranslationProgressDlg::IDD, pParent)
{
	Create();
}


BOOL CInterOpFileTranslationProgressDlg::OnInitDialog () {
	CenterWindow();
	return CDialog::OnInitDialog();
}

void CInterOpFileTranslationProgressDlg::DoDataExchange ( CDataExchange * pDX ) {
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDD_IOP_FILE_TRANSLATION_PROGRESS_PROGRESS, m_ProgressCtrl);
	DDX_Control(pDX, IDD_IOP_FILE_TRANSLATION_PROGRESS_OUTPUT, m_edit);
}


BEGIN_MESSAGE_MAP(CInterOpFileTranslationProgressDlg, CDialog)

END_MESSAGE_MAP()

void CInterOpFileTranslationProgressDlg::SetProgress ( float progress ) {
	SetWindowText(H_TEXT(H_FORMAT_TEXT("Loading File... (%0.2f%%)", (progress * 100))));
}

void CInterOpFileTranslationProgressDlg::SetLocalProgress ( float progress ) {
	m_ProgressCtrl.SetPos((int)(progress * 100));
}

void CInterOpFileTranslationProgressDlg::Output ( CString output ) {
	/* Magic trick to automagically scroll to the bottom... */
	m_edit.HideSelection(FALSE,FALSE);
	m_edit.ReplaceSel(output);
	m_edit.ReplaceSel(_T("\x0d\x0a"));
	m_edit.SetSel(m_edit.GetTextLength(),-1);
}

void CInterOpFileTranslationProgressDlg::Notice ( wchar_t const * msg ) {
	ShowWindow(SW_NORMAL);
	Output(CString(msg));
	UpdateWindow();
}


void CInterOpFileTranslationProgressDlg::Notice ( char const * msg ) {
	ShowWindow(SW_NORMAL);
	Output(msg);
	UpdateWindow();
}

void CInterOpFileTranslationProgressDlg::Notice ( float percent ) {
	ShowWindow(SW_NORMAL);
	SetProgress(percent);
	SetLocalProgress(percent);
	UpdateWindow();
}
