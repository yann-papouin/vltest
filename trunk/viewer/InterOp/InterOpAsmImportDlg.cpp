#include "stdafx.h"
#include "resource.h"
#include "InterOpAsmImportDlg.h"
#include "InterOpFileIO.h"


IMPLEMENT_DYNAMIC(CInterOpAsmImportDlg, CDialog)
CInterOpAsmImportDlg::CInterOpAsmImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInterOpAsmImportDlg::IDD, pParent)
{

}

CInterOpAsmImportDlg::~CInterOpAsmImportDlg()
{
}

void CInterOpAsmImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInterOpAsmImportDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_IOP_ASM_XML, OnBnClickedRadioXML)
	ON_BN_CLICKED(IDC_RADIO_IOP_ASM_STANDARD, OnBnClickedRadioStandard)
END_MESSAGE_MAP()

int CInterOpAsmImportDlg::DoModal ( InterOpFileIO * iop_fio )
{
	m_iop_fio = iop_fio;
	return CDialog::DoModal();
}


void CInterOpAsmImportDlg::OnBnClickedRadioXML()
{
	m_iop_fio->SetConvertAsmParts(false);
}

void CInterOpAsmImportDlg::OnBnClickedRadioStandard()
{
	m_iop_fio->SetConvertAsmParts(true);
}

BOOL CInterOpAsmImportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CButton * radio_xml  = (CButton*)GetDlgItem(IDC_RADIO_IOP_ASM_XML);
	radio_xml->SetCheck(true);
	UpdateData(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}