#include "stdafx.h"
#include "resource.h"
#include "InterOpImportDlg.h"
#include "InterOpFileIO.h"


IMPLEMENT_DYNAMIC(CInterOpImportDlg, CDialog)
CInterOpImportDlg::CInterOpImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInterOpImportDlg::IDD, pParent)
{

}

CInterOpImportDlg::~CInterOpImportDlg()
{
}

void CInterOpImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInterOpImportDlg, CDialog)
	ON_BN_CLICKED(IDC_RADIO_IOP_PROE, OnBnClickedRadioProE)
	ON_BN_CLICKED(IDC_RADIO_IOP_UG, OnBnClickedRadioUG)
END_MESSAGE_MAP()

int CInterOpImportDlg::DoModal ( InterOpFileIO * iop_fio )
{
	m_iop_fio = iop_fio;
	return CDialog::DoModal();
}


void CInterOpImportDlg::OnBnClickedRadioProE()
{
	m_iop_fio->SetPrtIsUG(false);
}

void CInterOpImportDlg::OnBnClickedRadioUG()
{
	m_iop_fio->SetPrtIsUG(true);
}

BOOL CInterOpImportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CButton * radio_proe  = (CButton*)GetDlgItem(IDC_RADIO_IOP_PROE);
	radio_proe->SetCheck(true);
	UpdateData(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}