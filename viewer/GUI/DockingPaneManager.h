#if !defined(AFX_DOCKINGPANEMANAGER_H__34C1E8DA_AC92_4721_BC15_A904662E0A5A__INCLUDED_)
#define AFX_DOCKINGPANEMANAGER_H__34C1E8DA_AC92_4721_BC15_A904662E0A5A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDockingPaneTabbedContainer : public CXTPDockingPaneTabbedContainer
{
public:
	CDockingPaneTabbedContainer(CXTPDockingPaneLayout* pLayout);

	void OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton);
	BOOL IsCaptionButtonVisible(CXTPDockingPaneCaptionButton* pButton);
};

class CDockingPaneMiniWnd : public CXTPDockingPaneMiniWnd
{
public:
	CDockingPaneMiniWnd(CXTPDockingPaneLayout* pLayout);
	void OnCaptionButtonClick(CXTPDockingPaneCaptionButton* pButton);

	void OnNcRButtonDown(UINT nHitTest, CPoint point);

	DECLARE_MESSAGE_MAP()
};



class CDockingPanePaintManager : public CXTPDockingPaneOffice2003Theme
{
public:
	CDockingPanePaintManager();
protected:

	void RoundRect(CDC* pDC, CRect rc, COLORREF clr);
	void DrawSplitter(CDC& dc, CXTPDockingPaneSplitterWnd* pSplitter);

	virtual void DrawCaptionButton(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, COLORREF clrText);
	virtual void DrawCaptionButtonEntry(CDC* pDC, CXTPDockingPaneCaptionButton* pButton, CPoint pt, COLORREF clrButton);
};


class CDockingPaneManager : public CXTPDockingPaneManager
{
public:
	CDockingPaneManager();
	virtual ~CDockingPaneManager();

protected:
	virtual CXTPDockingPaneBase* OnCreatePane(XTPDockingPaneType type, CXTPDockingPaneLayout* pLayout);

};

#endif // !defined(AFX_DOCKINGPANEMANAGER_H__34C1E8DA_AC92_4721_BC15_A904662E0A5A__INCLUDED_)
