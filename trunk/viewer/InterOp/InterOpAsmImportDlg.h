
#ifndef _INTEROPASMIMPORTDLG_H
#include "resource.h"

class InterOpFileIO;

class CInterOpAsmImportDlg : public CDialog
{
	DECLARE_DYNAMIC(CInterOpAsmImportDlg)

public:
	CInterOpAsmImportDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInterOpAsmImportDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_IOP_ASM_IMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	InterOpFileIO *m_iop_fio;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioXML();
	afx_msg void OnBnClickedRadioStandard();
	afx_msg int DoModal ( InterOpFileIO * iop_fio );
};

#define _INTEROPASMIMPORTDLG_H
#endif
