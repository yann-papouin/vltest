#include "stdafx.h"
#include "SelectionManipulator.h"

#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/PolytopeIntersector>
#include <osg/BlendColor>
#include <osg/Material>
#include <osg/Point>
#include <osg/Group>
#include <osg/StateAttribute>
#include <osg/PointSprite>
#include <osg/LineWidth>
#include <string>

#include "HUDCamera.h"

using namespace std;

/// Constructor.
SelectionManipulator::SelectionManipulator( int flags )
    : TrackballManipulator( flags )
{
	SetupHighLightNode();
	m_orgFaceState = m_orgEdgeState = 1;
	m_orgVertexState = 0;
	m_curView = NULL;
}

void SelectionManipulator::SetupHighLightNode()
{
	//FACE
	m_highlightFaceGroup = new osg::Group();
	osg::ref_ptr<osg::StateSet> highlightFaceState = new osg::StateSet();
	osg::ref_ptr<osg::Material> highlightFaceColor = new osg::Material();
	highlightFaceColor->setDiffuse(osg::Material::FRONT_AND_BACK,
		osg::Vec4(221/255.0f, 216/255.0f, 0, 1));
	highlightFaceState->setAttributeAndModes(highlightFaceColor, osg::StateAttribute::ON);
	m_highlightFaceGroup->setStateSet(highlightFaceState);

	//EDGE
	m_highlightEdgeGroup = new osg::Group();
	osg::ref_ptr<osg::StateSet> highlightEdgeState = new osg::StateSet();
	osg::ref_ptr<osg::Material> highlightEdgeColor = new osg::Material();
	highlightEdgeColor->setDiffuse(osg::Material::FRONT_AND_BACK,
		osg::Vec4(221/255.0f, 216/255.0f,0, 1));
	highlightEdgeState->setAttributeAndModes(highlightEdgeColor,
		osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
//	osg::ref_ptr<osg::LineWidth> hightlineWidth = new osg::LineWidth(4.0);
//	highlightEdgeState->setAttributeAndModes(hightlineWidth, osg::StateAttribute::ON);
	m_highlightEdgeGroup->setStateSet(highlightEdgeState);

	//Vertex
	m_highlightVertexGroup = new osg::Group();
	osg::ref_ptr<osg::StateSet> highlightVertexState = new osg::StateSet();
	osg::ref_ptr<osg::Material> highlightVertexColor = new osg::Material();
	highlightVertexColor->setDiffuse(osg::Material::FRONT_AND_BACK,
		osg::Vec4(1, 0, 0, 1));
	highlightVertexState->setAttributeAndModes(highlightVertexColor, osg::StateAttribute::ON);
	osg::ref_ptr<osg::Point> highpoint = new osg::Point(6);
	highlightVertexState->setAttributeAndModes(highpoint, osg::StateAttribute::ON);
	m_highlightVertexGroup->setStateSet(highlightVertexState);
}

void SelectionManipulator::ClearHighLightNode()
{
	vector<osg::ref_ptr<osg::Node>>::iterator iter;
	for (iter = m_HighlightNodeList.begin();
		iter != m_HighlightNodeList.end(); ++iter)
	{
		osg::ref_ptr<osg::Node> curNode = *iter;
		if (m_highlightFaceGroup->containsNode(curNode))
		{
			m_highlightFaceGroup->removeChild(curNode);
			osg::Group* highGroup = m_highlightFaceGroup->getParent(0)->asGroup();
			highGroup->removeChild(m_highlightFaceGroup);
			highGroup->addChild(curNode);
		}
		else if (m_highlightEdgeGroup->containsNode(curNode))
		{
			m_highlightEdgeGroup->removeChild(curNode);
			osg::Group* highGroup = m_highlightEdgeGroup->getParent(0)->asGroup();
			highGroup->removeChild(m_highlightEdgeGroup);
			highGroup->addChild(curNode);
		}
		else if (m_highlightVertexGroup->containsNode(curNode))
		{
			m_highlightVertexGroup->removeChild(curNode);
			osg::Group* highGroup = m_highlightVertexGroup->getParent(0)->asGroup();
			highGroup->removeChild(m_highlightVertexGroup);
			highGroup->addChild(curNode);
		}
	}

	m_HighlightNodeList.clear();
	if (m_hud != NULL)
	{
		m_hud->SetHUDText("");
	}
}

void SelectionManipulator::AddToHighLightNode(osg::Node* curNode)
{
	osg::Group* parentEnt = curNode->getParent(0)->asGroup();
	parentEnt->removeChild(curNode);
	string name = curNode->getName();
	string::size_type Faceidx = name.find("FACE");
	string::size_type Edgeidx = name.find("EDGE");
	if (name == "FaceGroup" || Faceidx != string::npos)
	{
		parentEnt->addChild(m_highlightFaceGroup);
		m_highlightFaceGroup->addChild(curNode);
		m_HighlightNodeList.push_back(curNode);
	}
	else if (name == "EDGE" || Edgeidx != string::npos)
	{
		parentEnt->addChild(m_highlightEdgeGroup);
		m_highlightEdgeGroup->addChild(curNode);
		m_HighlightNodeList.push_back(curNode);
	}
	else if (name.find("VERTEX") != string::npos)
	{
		parentEnt->addChild(m_highlightVertexGroup);
		m_highlightVertexGroup->addChild(curNode);
		m_HighlightNodeList.push_back(curNode);
	}

	string nodeName = curNode->getName();
	if (m_hud != NULL)
	{
		m_hud->SetHUDText(nodeName);
	}

	string typeName;
	string idValueStr;
	string::size_type idx = nodeName.find("#");
	if (idx != string::npos)
	{
		typeName = nodeName.substr(0, idx);
		idValueStr = nodeName.substr(idx+1);
	}
	int entID = (int)atoi(idValueStr.c_str());
	//ENTITY* selEntity = NULL;
	//api_get_entity_from_id(entID, selEntity);
//	const char* type_name = selEntity->type_name();
}

osgUtil::PolytopeIntersector::Intersection ComputerRightIntersection(
	const osgUtil::PolytopeIntersector::Intersections& intersections)
{
	std::string nodeName;
	osgUtil::PolytopeIntersector::Intersection firstEdgeInter;
	bool hasEdge = false;

	osgUtil::PolytopeIntersector::Intersection firstFaceInter;
	bool hasFace = false;

	osgUtil::PolytopeIntersector::Intersections::const_iterator hitr;
	for (hitr = intersections.begin();
		hitr != intersections.end(); ++hitr)
	{
		osgUtil::PolytopeIntersector::Intersection curInterSect = *hitr;
		osg::Node* curNode = curInterSect.nodePath.back();
		nodeName = curNode->getName();
		if (nodeName.find("VERTEX") != string::npos)
		{
			return curInterSect;
		}
		else if (nodeName.find("EDGE") != string::npos && !hasEdge)
		{
			firstEdgeInter = curInterSect;
			hasEdge = true;
		}
		else if (nodeName.find("FACE") != string::npos && !hasFace)
		{
			firstFaceInter = curInterSect;
			hasFace = true;
		}
	}
	if (hasEdge && hasFace)
	{
		if (firstFaceInter.distance < firstEdgeInter.distance)
		{
			return firstFaceInter;
		}
	}
	
	if (hasEdge)
	{
		return firstEdgeInter;
	}
	else if(hasFace)
	{
		return firstFaceInter;
	}
	else
	{
		return *(intersections.begin());
	}
}

// doc in parent
bool SelectionManipulator::handleMouseMove( const GUIEventAdapter& ea, GUIActionAdapter& us )
{
	//return true;
	osgViewer::View* curView = (osgViewer::View*)us.asView();
	float x = ea.getX();
	float y = ea.getY();
	float delta = 2;

	osgUtil::PolytopeIntersector::Intersections intersections;
	osgUtil::PolytopeIntersector::Intersections::iterator hitr;
	osg::ref_ptr< osgUtil::PolytopeIntersector > picker = 
		new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, x-delta, y-delta, x+delta, y+delta);
	//picker->setDimensionMask(osgUtil::PolytopeIntersector::AllDims);

	osgUtil::IntersectionVisitor iv(picker.get());
	curView->getCamera()->accept(iv);

	/*osgUtil::LineSegmentIntersector::Intersections intersections;
	osgUtil::LineSegmentIntersector::Intersections::iterator hitr;
	osg::ref_ptr< osgUtil::LineSegmentIntersector > picker = 
		new osgUtil::LineSegmentIntersector(osgUtil::Intersector::WINDOW, x, y);
	osgUtil::IntersectionVisitor iv(picker.get());
	curView->getCamera()->accept(iv);*/

	if (picker->containsIntersections())
	{
		intersections = picker->getIntersections();
		osgUtil::PolytopeIntersector::Intersection firstRes = 
			ComputerRightIntersection(intersections);
		/*osgUtil::LineSegmentIntersector::Intersection firstRes = 
			*(intersections.begin());*/
		osg::ref_ptr<osg::Node> lastNode = firstRes.nodePath.back();
		string nodeName = lastNode->getName();
		if (nodeName.find("VERTEX") == std::string::npos &&
			nodeName.find("EDGE") == std::string::npos &&
			nodeName.find("FACE") == std::string::npos)
		{
			ClearHighLightNode();
			curView->requestRedraw();
			return true;
		}
		
		if (!m_HighlightNodeList.empty() && lastNode == m_HighlightNodeList.front())
		{
			return true;
		}
		ClearHighLightNode();

		AddToHighLightNode(lastNode.get());

	}
	else
	{
		ClearHighLightNode();
	}
	// 9/8/2011 mwu : 
	curView->requestRedraw();

	return true;
}

// doc in parent
bool SelectionManipulator::OldhandleMouseMove( const GUIEventAdapter& ea, GUIActionAdapter& us )
{
	osgViewer::View* curView = (osgViewer::View*)us.asView();
	float x = ea.getX();
	float y = ea.getY();
//	float delta = 2;

	/*osgUtil::PolytopeIntersector::Intersections intersections;
	osgUtil::PolytopeIntersector::Intersections::iterator hitr;
	osg::ref_ptr< osgUtil::PolytopeIntersector > picker = 
		new osgUtil::PolytopeIntersector(osgUtil::Intersector::WINDOW, x-delta, y-delta, x+delta, y+delta);
	osgUtil::IntersectionVisitor iv(picker.get());
	curView->getCamera()->accept(iv);
	if (picker->containsIntersections())*/
	osgUtil::LineSegmentIntersector::Intersections intersections;
	if(curView->computeIntersections(x,y,intersections))
	{
		//intersections = picker->getIntersections();
		//osgUtil::PolytopeIntersector::Intersection firstRes = *(intersections.begin());
		osgUtil::LineSegmentIntersector::Intersection firstRes = 
			*(intersections.begin());
		osg::ref_ptr<osg::Node> lastNode = firstRes.nodePath.back();
		string nodeName = lastNode->getName();
		string typeName;
		string idValueStr;
		string::size_type idx = nodeName.find("#");
		if (idx != string::npos)
		{
			typeName = nodeName.substr(0, idx);
			idValueStr = nodeName.substr(idx+1);
		}
		int entID = (int)atoi(idValueStr.c_str());
		//ENTITY* selEntity = NULL;
		//api_get_entity_from_id(entID, selEntity);
//		const char* type_name = selEntity->type_name();

		osg::ref_ptr<osg::Geode> lastGeode = lastNode->asGeode();
		/*osg::StateSet* curNodeState = lastGeode->getStateSet();
		osg::Material* curMat = 
			(osg::Material*)curNodeState->getAttribute(osg::StateAttribute::MATERIAL);
		if (curMat != NULL)
		{
			curMat->setDiffuse(osg::Material::FRONT_AND_BACK,
				osg::Vec4(155/255.0f,0/255.0f,0/255.0f, 1.0));
			curNodeState->setAttributeAndModes(curMat, osg::StateAttribute::ON);
			m_HighlightNodeList.push_back(lastNode);
		}*/
		
		lastNode = lastNode->getParent(0)->getParent(0);
		
		int numDraw = lastGeode->getNumDrawables();
		for (int i = 0; i < numDraw; ++i)
		{
			lastGeode->getDrawable(i);
		}
		
		if (!m_HighlightNodeList.empty() && lastNode == m_HighlightNodeList.front())
		{
			return true;
		}
		
		vector<osg::ref_ptr<osg::Node>>::iterator iter;
		for (iter = m_HighlightNodeList.begin();
			iter != m_HighlightNodeList.end(); ++iter)
		{
			osg::ref_ptr<osg::Node> curNode = *iter;
			if (m_highlightFaceGroup->containsNode(curNode))
			{
				m_highlightFaceGroup->removeChild(curNode);
				m_faceGroup->addChild(curNode);
			}
			else if (m_highlightEdgeGroup->containsNode(curNode))
			{
				m_highlightEdgeGroup->removeChild(curNode);
				m_edgeGroup->addChild(curNode);
			}
			else if (m_highlightVertexGroup->containsNode(curNode))
			{
				m_highlightVertexGroup->removeChild(curNode);
				m_vertexGroup->addChild(curNode);
			}
		}
		m_HighlightNodeList.clear();
		
		if (m_faceGroup->containsNode(lastNode))
		{
			m_faceGroup->removeChild(lastNode);
			m_highlightFaceGroup->addChild(lastNode);
			m_HighlightNodeList.push_back(lastNode);
		}
		else if (m_edgeGroup->containsNode(lastNode))
		{
			m_edgeGroup->removeChild(lastNode);
			m_highlightEdgeGroup->addChild(lastNode);
			m_HighlightNodeList.push_back(lastNode);
		}
		else if (m_vertexGroup->containsNode(lastNode))
		{
			m_vertexGroup->removeChild(lastNode);
			m_highlightVertexGroup->addChild(lastNode);
			m_HighlightNodeList.push_back(lastNode);
		}
	}
	else
	{
		vector<osg::ref_ptr<osg::Node>>::iterator iter;
		for (iter = m_HighlightNodeList.begin();
			iter != m_HighlightNodeList.end(); ++iter)
		{
			osg::ref_ptr<osg::Node> curNode = *iter;
			if (m_highlightFaceGroup->containsNode(curNode))
			{
				m_highlightFaceGroup->removeChild(curNode);
				m_faceGroup->addChild(curNode);
			}
			else if (m_highlightEdgeGroup->containsNode(curNode))
			{
				m_highlightEdgeGroup->removeChild(curNode);
				m_edgeGroup->addChild(curNode);
			}
			else if (m_highlightVertexGroup->containsNode(curNode))
			{
				m_highlightVertexGroup->removeChild(curNode);
				m_vertexGroup->addChild(curNode);
			}
		}
		m_HighlightNodeList.clear();
	}
	return true;
}

void SelectionManipulator::hideEdgeAndVertex()
{
	if (m_orgEdgeState && m_orgFaceState && m_edgeGroup != NULL)
	{
		m_edgeGroup->setNodeMask(0);
	}
	if (m_orgVertexState && m_vertexGroup != NULL)
	{
		m_vertexGroup->setNodeMask(0);
	}
}

bool SelectionManipulator::performMovementLeftMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	hideEdgeAndVertex();
	return osgGA::OrbitManipulator::performMovementLeftMouseButton(eventTimeDelta, dx, dy);
}

bool SelectionManipulator::performMovementMiddleMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	hideEdgeAndVertex();
	return osgGA::OrbitManipulator::performMovementMiddleMouseButton(eventTimeDelta, dx, dy);
}

bool SelectionManipulator::handleMouseRelease( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& us )
{
	osgViewer::View* curView = (osgViewer::View*)us.asView();
	bool release = osgGA::StandardManipulator::handleMouseRelease(ea, us);
	if (m_edgeGroup != NULL)
	{
		m_edgeGroup->setNodeMask(m_orgEdgeState);
	}
	if(m_vertexGroup != NULL)
	{
		m_vertexGroup->setNodeMask(m_orgVertexState);
	}
	curView->requestRedraw();
	return release;
}

bool SelectionManipulator::handleMouseWheel( const GUIEventAdapter& ea, GUIActionAdapter& us )
{
	osgGA::GUIEventAdapter::ScrollingMotion sm = ea.getScrollingMotion();
	osgViewer::View* curView = (osgViewer::View*)us.asView();

	//float x = ea.getX();
	//float y = ea.getY();

	osg::Camera* masterCam = curView->getCamera();

	double left,right,bottom,top,zNear,zFar;
	masterCam->getProjectionMatrixAsOrtho(left,right,
		bottom,top,zNear,zFar);

	// handle centering
	if( _flags & SET_CENTER_ON_WHEEL_FORWARD_MOVEMENT )
	{

		if( ((sm == GUIEventAdapter::SCROLL_DOWN && _wheelZoomFactor > 0.)) ||
			((sm == GUIEventAdapter::SCROLL_UP   && _wheelZoomFactor < 0.)) )
		{

			if( getAnimationTime() <= 0. )
			{
				// center by mouse intersection (no animation)
				setCenterByMousePointerIntersection( ea, us );
			}
			else
			{
				// start new animation only if there is no animation in progress
				if( !isAnimating() )
					startAnimationByMousePointerIntersection( ea, us );

			}

		}
	}

	switch( sm )
	{
		// mouse scroll up event
	case GUIEventAdapter::SCROLL_UP:
		{
			// perform zoom
			float scale = 1.0f + _wheelZoomFactor;
			masterCam->setProjectionMatrixAsOrtho(left*scale,right*scale,
				bottom*scale,top*scale,zNear,zFar);
			us.requestRedraw();
			us.requestContinuousUpdate( isAnimating() || _thrown );
			return true;
		}

		// mouse scroll down event
	case GUIEventAdapter::SCROLL_DOWN:
		{
			// perform zoom
			float scale = 1.0f - _wheelZoomFactor;
			masterCam->setProjectionMatrixAsOrtho(left*scale,right*scale,
				bottom*scale,top*scale,zNear,zFar);
			us.requestRedraw();
			us.requestContinuousUpdate( false );
			return true;
		}

		// unhandled mouse scrolling motion
	default:
		return false;
	}
}

bool SelectionManipulator::handleMouseDrag( const GUIEventAdapter& ea, GUIActionAdapter& us )
{
	m_curView = (osgViewer::View*)us.asView();
	addMouseEvent( ea );

	if( performMovement() )
		us.requestRedraw();

	us.requestContinuousUpdate( false );
	_thrown = false;

	return true;
}

bool SelectionManipulator::performMovementRightMouseButton( const double eventTimeDelta, const double dx, const double dy )
{
	hideEdgeAndVertex();

	// zoom model
	osg::Camera* masterCam = m_curView->getCamera();

	double left,right,bottom,top,zNear,zFar;
	masterCam->getProjectionMatrixAsOrtho(left,right,
		bottom,top,zNear,zFar);

	float scale = 1.0f + dy * getThrowScale( eventTimeDelta );
	masterCam->setProjectionMatrixAsOrtho(left*scale,right*scale,
		bottom*scale,top*scale,zNear,zFar);

	return true;
}