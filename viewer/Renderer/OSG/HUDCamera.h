#pragma once

//单件设计模式，只有一个实例

#include <osg/Camera>
#include <osg/Texture>
#include <osgText/Text>

class MyHUD :

	public osg::Camera

{

public:

	MyHUD(void);

public:

	virtual ~MyHUD(void);

	//static MyHUD *GetPtr();

	void AddHUDText(std::string);

	void SetHUDText(std::string str);

private:

	int lineNum;

	int charNumPerLine;

	osg::Vec4 color;

	//static MyHUD *myhud;

	std::list<std::string> strlist;

	osg::ref_ptr<osgText::Text> HUDText;

	osg::Vec3 position;

};

