// GenViewerView.h : interface of the CGenViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GenViewerVIEW_H__3FAD79CB_6331_4EA1_BED5_BDD2716339A3__INCLUDED_)
#define AFX_GenViewerVIEW_H__3FAD79CB_6331_4EA1_BED5_BDD2716339A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGenViewerDoc;

#if defined(HOOPS) && defined(ACIS) 
#include "Renderer/HOOPS/HSolidView.h"
#include "Renderer/HOOPS/HSolidModel.h"
class HSelectionSet;

class HGenViewerView : public HSolidView
{
public:
	HGenViewerView(	HSolidModel *model,
		const char * 	alias = 0,	
		const char *	driver_type = 0,
		const char *	instance_name = 0,
		void *			window_handle = 0,
		void *			colormap = 0);
	virtual ~HGenViewerView();
	void	Init();
};
#endif

#ifdef VL
#include <vlGraphics/Rendering.hpp>
//#include <vlGraphics/EdgeRenderer.hpp>
#include <Renderer/vl/VLBaseView.hpp>
//#include <Renderer/vl/AxisApplet.hpp>
#include <vlMFC/MDIWindow.hpp>
#define CMFCView vlMFC::MDIWindow
#else 
#define CMFCView CView
#endif // VL


class CGenViewerView : public CMFCView
{
protected: // create from serialization only
	CGenViewerView();
	DECLARE_DYNCREATE(CGenViewerView)

// Attributes
public:
	CGenViewerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPindockView)
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGenViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void DrawLogo();
	void DrawScene();
	void DeleteContents();
	BOOL OnOpenDocument(LPCTSTR lpszPathName);

protected:
	CBitmap m_hLogo;
	int m_logoWidth;
	int m_logoHeight;
	HANDLE mThreadHandle;
	bool m_bPerspective;
	CClientDC*  m_pDC;
	bool    m_bFlush;

#if defined(HOOPS)
	HBaseOperator* m_pDefaultViewOp;
#elif defined VL
	vl::ref<VLBaseView> mVLBaseView;
	//! Utility function that adds an Actor and binds it to the given Renderable, Effect and Transform.
#endif
// Generated message map functions
protected:
	//{{AFX_MSG(CPindockView)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnViewIso();
	afx_msg void OnViewTop();
	afx_msg void OnViewBottom();
	afx_msg void OnViewFront();
	afx_msg void OnViewBack();
	afx_msg void OnViewLeft();
	afx_msg void OnViewRight();

	afx_msg void OnViewPerspective();
	afx_msg void OnUpdateViewPerspective(CCmdUI* pCmdUI);

	afx_msg void OnOrbitOpt();
	afx_msg void OnUpdateOrbitOpt(CCmdUI* pCmdUI);
	afx_msg void OnZoomOpt();
	afx_msg void OnUpdateZoomOpt(CCmdUI* pCmdUI);
	afx_msg void OnPanOpt();
	afx_msg void OnUpdatePanOpt(CCmdUI* pCmdUI);
	afx_msg void OnZoomToExtents();
	afx_msg void OnUpdateZoomToExtents(CCmdUI* pCmdUI);
	afx_msg void OnZoomToWindow();
	afx_msg void OnUpdateZoomToWindow(CCmdUI* pCmdUI);

	afx_msg void OnViewRenderWireframe();
	afx_msg void OnViewRenderGouraud();
	afx_msg void OnViewRenderHiddenLine();
	afx_msg void OnViewRenderWireframeWithSilhouette();
	afx_msg void OnViewRenderTriangulation();
	afx_msg void OnViewRenderGouraudWithEdges();
	afx_msg void OnViewRenderPhong();
	afx_msg void OnViewRenderPhongWithEdges();

	afx_msg void OnWindowSelect(); 
	afx_msg void OnUpdateWindowSelect(CCmdUI* pCmdUI);
	afx_msg void OnCreateSphere();
	afx_msg void OnUpdateCreateSphere(CCmdUI* pCmdUI);
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
#if defined(HOOPS) && defined(ACIS) 
	HSolidView* m_pHSolidView;
#endif
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
#ifdef HOOPS
	unsigned long MapFlags(unsigned long state);
	void  LocalSetOperator(HBaseOperator * NewOperator);	
#endif	
};

#ifndef _DEBUG  // debug version in pindockView.cpp
inline CGenViewerDoc* CGenViewerView::GetDocument()
   { return (CGenViewerDoc*)m_pDocument; }
#endif


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GenViewerVIEW_H__3FAD79CB_6331_4EA1_BED5_BDD2716339A3__INCLUDED_)
