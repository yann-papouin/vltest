#include "stdafx.h"
#include "HUDCamera.h"

//MyHUD *MyHUD::myhud=NULL;

MyHUD::MyHUD(void):color(1.0f,1.0,0.8f,0.2f)

,lineNum(5)

,charNumPerLine(20)

,position(20.0f,20.0f,0.0f)

{

	//设置数据是变更的

	this->setDataVariance(osg::Geometry::DYNAMIC);

	//创建一个摄像机，用它来设置投影矩阵和模型视图矩阵

	//以及用来显示HUD的子节点

	osg::Camera* camera = this;



	//得到屏幕分辨率

	unsigned int width, height;

	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (!wsi) 

	{

		osg::notify(osg::NOTICE)<<"Error, no WindowSystemInterface available, cannot create windows."<<std::endl;

		return ;

	}

	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);



	//建立投影矩阵

	camera->setProjectionMatrix(osg::Matrix::ortho2D(0,width,0,height));



	// 设定视图矩阵

	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);

	camera->setViewMatrix(osg::Matrix::identity());



	//只清除深度缓存

	camera->setClearMask(GL_DEPTH_BUFFER_BIT);



	//设置摄像机子图的渲染顺序，这个顺序相对于这个子图所在的所有摄像机。

	//对于渲染到纹理，应设置PRE_RENDER，对于HUD，设置为POST_RENDER

	camera->setRenderOrder(osg::Camera::POST_RENDER);



	//不处理主摄像机传递过来的事件

	camera->setAllowEventFocus(false);



	osg::Geode* geode = new osg::Geode();



	std::string timesFont("fudd.ttf");



	// turn lighting off for the text and disable depth test to ensure its always ontop.

	osg::StateSet* stateset = geode->getOrCreateStateSet();

	stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);





	HUDText=new  osgText::Text;

	geode->addDrawable( HUDText.get() );

	HUDText->setFont(timesFont);

	HUDText->setPosition(position);

	HUDText->setText("");

	HUDText->setDataVariance(osg::Geometry::DYNAMIC);



	//osg::BoundingBox bb;

	//for(unsigned int i=0;i<geode->getNumDrawables();++i)

	//{

	//	bb.expandBy(geode->getDrawable(i)->getBound());

	//}



	//osg::Geometry* geom = new osg::Geometry;



	//osg::Vec3Array* vertices = new osg::Vec3Array;

	//float depth = bb.zMin()-0.1;

	//vertices->push_back(osg::Vec3(bb.xMin(),bb.yMax(),depth));

	//vertices->push_back(osg::Vec3(bb.xMin(),bb.yMin(),depth));

	//vertices->push_back(osg::Vec3(bb.xMax(),bb.yMin(),depth));

	//vertices->push_back(osg::Vec3(bb.xMax(),bb.yMax(),depth));

	//geom->setVertexArray(vertices);



	//osg::Vec3Array* normals = new osg::Vec3Array;

	//normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));

	//geom->setNormalArray(normals);

	//geom->setNormalBinding(osg::Geometry::BIND_OVERALL);



	//osg::Vec4Array* colors = new osg::Vec4Array;

	//colors->push_back(osg::Vec4(1.0f,0.0,0.0f,1.0f));

	//geom->setColorArray(colors);

	//geom->setColorBinding(osg::Geometry::BIND_OVERALL);



	//geom->addPrimitiveSet(new osg::DrawArrays(GL_QUADS,0,4));



	////设置颜色的混合，启动透明以按深度排序

	//stateset->setMode(GL_BLEND,osg::StateAttribute::ON);

	//stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);



	//geode->addDrawable(geom);

	camera->addChild(geode);

}



MyHUD::~MyHUD(void)

{

}

//MyHUD * MyHUD::GetPtr()
//
//{
//
//	if (NULL==myhud)
//
//	{
//
//		myhud=new MyHUD;
//
//	}
//
//	return myhud;
//
//}

void MyHUD::SetHUDText(std::string str)
{
	HUDText->setText(str);
}

void MyHUD::AddHUDText(std::string str)

{

	while (str.size()>(size_t)charNumPerLine)

	{

		strlist.push_back(std::string(str.begin(),str.begin()+charNumPerLine));

		str.erase(str.begin(),str.begin()+charNumPerLine);

	}

	if(str.size()>0)

		strlist.push_back(str);

	while(strlist.size()>(size_t)lineNum)

		strlist.pop_front();

	//得到文本

	std::string HUDstr;

	std::list<std::string>::iterator it;

	for(it=strlist.begin();it!=strlist.end();it++)

		HUDstr=HUDstr+*it;

	HUDText->setText(HUDstr);

}

