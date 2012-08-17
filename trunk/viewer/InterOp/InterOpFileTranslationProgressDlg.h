
#pragma once

#include "Resource.h"

class CInterOpFileTranslationProgressDlg : public CDialog
{
	DECLARE_DYNAMIC(CInterOpFileTranslationProgressDlg)

public:
	CInterOpFileTranslationProgressDlg ( CWnd * pParent = NULL );   // standard constructor
	virtual ~CInterOpFileTranslationProgressDlg () {};

	enum { IDD = IDD_IOP_FILE_TRANSLATION_PROGRESS };

	afx_msg BOOL Create () { return CDialog::Create(IDD_IOP_FILE_TRANSLATION_PROGRESS); };

	void Output ( CString output );
	void Notice ( wchar_t const * msg );
	void Notice ( char const * msg );
	void Notice ( float percent );

protected:
	BOOL OnInitDialog ();
	virtual void DoDataExchange ( CDataExchange * pDX );    // DDX/DDV support
	CProgressCtrl m_ProgressCtrl;
	CRichEditCtrl m_edit;	

	void SetProgress ( float progress );
	void SetLocalProgress ( float progress );

	DECLARE_MESSAGE_MAP()
};
