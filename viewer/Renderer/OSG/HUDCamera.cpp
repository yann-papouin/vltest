#include "stdafx.h"
#include "HUDCamera.h"

//MyHUD *MyHUD::myhud=NULL;

MyHUD::MyHUD(void):color(1.0f,1.0,0.8f,0.2f)

,lineNum(5)

,charNumPerLine(20)

,position(20.0f,20.0f,0.0f)

{

	//���������Ǳ����

	this->setDataVariance(osg::Geometry::DYNAMIC);

	//����һ�������������������ͶӰ�����ģ����ͼ����

	//�Լ�������ʾHUD���ӽڵ�

	osg::Camera* camera = this;



	//�õ���Ļ�ֱ���

	unsigned int width, height;

	osg::GraphicsContext::WindowingSystemInterface* wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (!wsi) 

	{

		osg::notify(osg::NOTICE)<<"Error, no WindowSystemInterface available, cannot create windows."<<std::endl;

		return ;

	}

	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);



	//����ͶӰ����

	camera->setProjectionMatrix(osg::Matrix::ortho2D(0,width,0,height));



	// �趨��ͼ����

	camera->setReferenceFrame(osg::Transform::ABSOLUTE_RF);

	camera->setViewMatrix(osg::Matrix::identity());



	//ֻ�����Ȼ���

	camera->setClearMask(GL_DEPTH_BUFFER_BIT);



	//�����������ͼ����Ⱦ˳�����˳������������ͼ���ڵ������������

	//������Ⱦ������Ӧ����PRE_RENDER������HUD������ΪPOST_RENDER

	camera->setRenderOrder(osg::Camera::POST_RENDER);



	//����������������ݹ������¼�

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



	////������ɫ�Ļ�ϣ�����͸���԰��������

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

	//�õ��ı�

	std::string HUDstr;

	std::list<std::string>::iterator it;

	for(it=strlist.begin();it!=strlist.end();it++)

		HUDstr=HUDstr+*it;

	HUDText->setText(HUDstr);

}

