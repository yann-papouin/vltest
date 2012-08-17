#ifndef SELECTION_MANIPULATIOR
#define SELECTION_MANIPULATIOR 1

#include <osgGA/TrackballManipulator>
#include <osgGA/StandardManipulator>
#include <osgViewer/View>
#include <vector>

#include "HUDCamera.h"

using namespace std;
using namespace osg;
using namespace osgGA;

class SelectionManipulator : public TrackballManipulator
{
public:
	SelectionManipulator( int flags = DEFAULT_SETTINGS);

	void SetFaceGroup(osg::ref_ptr<osg::Group> faceGroup)
	{
		m_faceGroup = faceGroup;
	}

	void SetEdgeGroup(osg::ref_ptr<osg::Group> edgeGroup)
	{
		m_edgeGroup = edgeGroup;
	}

	void SetVertexGroup(osg::ref_ptr<osg::Group> vertexGroup)
	{
		m_vertexGroup = vertexGroup;
	}

	void SetMyHUD(osg::ref_ptr<MyHUD> myhud)
	{
		m_hud = myhud;
	}

	bool OldhandleMouseMove( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	void hideEdgeAndVertex();

protected:

	virtual bool handleMouseMove( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	virtual bool performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy );

	virtual bool performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy );

	virtual bool performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy );

	virtual bool handleMouseRelease( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us );

	virtual bool handleMouseWheel( const GUIEventAdapter& ea, GUIActionAdapter& us );

	virtual bool handleMouseDrag( const GUIEventAdapter& ea, GUIActionAdapter& us );

	void SetupHighLightNode();

	void ClearHighLightNode();

	void AddToHighLightNode(osg::Node* curNode);

	vector<osg::ref_ptr<osg::Node>> m_HighlightNodeList;

	osg::ref_ptr<osg::Group>		m_faceGroup;
	osg::ref_ptr<osg::Group>		m_edgeGroup;
	osg::ref_ptr<osg::Group>		m_vertexGroup;
	osg::ref_ptr<osg::Group>		m_highlightFaceGroup;
	osg::ref_ptr<osg::Group>		m_highlightEdgeGroup;
	osg::ref_ptr<osg::Group>		m_highlightVertexGroup;

	osg::ref_ptr<MyHUD>				m_hud;

	int								m_orgFaceState;
	int								m_orgEdgeState;
	int								m_orgVertexState;

	osgViewer::View*				m_curView;
};


#endif /* SELECTION_MANIPULATIOR */
