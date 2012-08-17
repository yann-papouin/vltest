
#ifndef _INTEROPIMPORTDLG_H
#include "resource.h"

class InterOpFileIO;

class CInterOpImportDlg : public CDialog
{
	DECLARE_DYNAMIC(CInterOpImportDlg)

public:
	CInterOpImportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInterOpImportDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IOP_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	InterOpFileIO *m_iop_fio;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioProE();
	afx_msg void OnBnClickedRadioUG();
	afx_msg int DoModal ( InterOpFileIO * iop_fio );
};

#define _INTEROPIMPORTDLG_H
#endif
