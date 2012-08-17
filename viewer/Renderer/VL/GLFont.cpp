//---------------------------------GLFont.cpp-----------------------------------------------------------------------
#include "stdafx.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <atlstr.h>
#include "GLFont.hpp"

GLFont::GLFont()
{
	m_hFont=NULL;
	InitFont(_T("����"));
	m_X = 0;
	m_Y = 0;
	m_Z = 0;
	m_dHeight = 0;
	m_fExtrusion = 0.1f;
	m_rotateTheta = 0.0;
	m_iDisplayList = 0;
	m_red =255;
	m_blue = 255;
	m_green = 255;
}

GLFont::~GLFont()
{
}

LOGFONT GLFont::GetLogFont()
{
	LOGFONT lf;
	GetObject(m_hFont,sizeof(LOGFONT),&lf);
	return lf;
}

void GLFont::DeleteFont()
{
	if(m_hFont)
	{
		DeleteObject(m_hFont);
		m_hFont=NULL;
	}
}

bool GLFont::InitFont(LOGFONT &lf)
{
	DeleteFont();
	m_hFont=CreateFontIndirect(&lf);
	if( m_hFont )return true;
	else return false;;
}

bool GLFont::InitFont(LPCWSTR fontName)
{
	DeleteFont();
	m_hFont = CreateFont(
		-16,         //< lfHeight ����߶�
		0,          //< lfWidth ������ 
		0,          //< lfEscapement �������ת�Ƕ� Angle Of Escapement 
		0,          //< lfOrientation ������ߵ���ת�Ƕ�Orientation Angle 
		FW_BOLD,        //< lfWeight ��������� 
		FALSE,         //< lfItalic �Ƿ�ʹ��б�� 
		FALSE,         //< lfUnderline �Ƿ�ʹ���»��� 
		FALSE,         //< lfStrikeOut �Ƿ�ʹ��ɾ���� 
		GB2312_CHARSET,       //< lfCharSet �����ַ��� 
		OUT_TT_PRECIS,       //< lfOutPrecision ������� 
		CLIP_DEFAULT_PRECIS,     //< lfClipPrecision �ü����� 
		ANTIALIASED_QUALITY,     //< lfQuality ������� 
		FF_DONTCARE|DEFAULT_PITCH,    //< lfPitchAndFamily Family And Pitch 
		fontName);        //< lfFaceName �������� 
	if(m_hFont)return true;
	else return false;
}

void GLFont::Draw2DText(char *string)
{
	if(strlen(string)<=0)return;

	//HDC hDC=wglGetCurrentDC();
	HDC hDC = ::CreateCompatibleDC(NULL);

	HFONT hOldFont=NULL;
	hOldFont=(HFONT)SelectObject(hDC,m_hFont);
	if(!hOldFont)
	{
		return;
	}

//	glRasterPos2f(m_X, m_Y);
	glRasterPos3d(m_X, m_Y,m_Z);

	DWORD dwChar;
	int ListNum;
	for(size_t i=0;i<strlen((char *)string);i++)
	{
		if(IsDBCSLeadByte(string[i]))
		{
			dwChar=((unsigned char)string[i])*0x100+(unsigned char)string[i+1];
			i++;
		}
		else
		{
			dwChar=string[i];
		}
		ListNum=glGenLists(1);
		wglUseFontBitmaps(hDC,dwChar,1,ListNum);
		glCallList(ListNum);
		glDeleteLists(ListNum,1);
	}

	SelectObject(hDC,hOldFont);
	::DeleteDC(hDC);
}
map<UINT,FontDesc>  GLFont::m_sglFontList;

void GLFont::Draw3DText(const char *string)
{
	if(strlen(string)<=0)
		return;

	GLYPHMETRICSFLOAT pgmf[1];
	HDC hDC=wglGetCurrentDC();

	//���õ�ǰ����
	HFONT hOldFont=NULL;
	hOldFont=(HFONT)SelectObject(hDC,m_hFont);
	if(!hOldFont)
	{
		return;
	}
	DWORD dwChar;

	int iNum=strlen((char *)string);
	m_iDisplayList=glGenLists(1);

	glPushMatrix();

	for(size_t i=0;i<strlen((char *)string);i++)
	{
		if(IsDBCSLeadByte(string[i]))//�Ƿ�Ϊ����
		{
			dwChar=((unsigned char)string[i])*0x100+(unsigned char)string[i+1];
			i++;
		}
		else
		{
			dwChar=string[i];
		}
		//��ͬ��������������������ֻ����һ�Σ��硰�����й��й������ظ����к͹�ֻ����һ�Σ��������������ٶ�
		if (m_sglFontList.find(dwChar) == m_sglFontList.end())
		{
			GLuint newID  = glGenLists(1);
			wglUseFontOutlines(hDC,dwChar,1,newID,0.0,m_fExtrusion,WGL_FONT_LINES,pgmf);
			FontDesc  font;
			font.flyfloat = pgmf[0];
			font.iList = newID;
			m_sglFontList[dwChar] = font;
		}
	}
	glPopMatrix();
	SelectObject(hDC,hOldFont);

	//���ɲ����б�
	glNewList(m_iDisplayList, GL_COMPILE);
	glPushMatrix();
	glTranslated(m_X,m_Y,m_Z);
	glScaled(m_dHeight,m_dHeight,1);
	glRotated(m_rotateTheta,0,0,1); 
	for(size_t i=0;i<strlen((char *)string);i++)
	{
		if(IsDBCSLeadByte(string[i]))
		{
			dwChar=((unsigned char)string[i])*0x100+(unsigned char)string[i+1];
			i++;
		}
		else
		{
			dwChar=string[i];
		}
		glCallList(m_sglFontList[dwChar].iList);

	}
	glPopMatrix();
	glEndList();

	//ִ�в������е�����
	glPushMatrix();
	glColor3ub((GLbyte)m_red,(GLbyte)m_green,(GLbyte)m_blue);
	glCallList(m_iDisplayList);
	glPopMatrix();
}

void GLFont::SetColor(int r, int g, int b)
{
	m_red = r;
	m_green = g;
	m_blue = b;
}


