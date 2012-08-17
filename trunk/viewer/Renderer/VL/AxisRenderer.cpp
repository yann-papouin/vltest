#include "stdafx.h"
#include <vlCore/GlobalSettings.hpp>
#include <vlGraphics/RenderQueue.hpp>
#include <vlGraphics/OpenGLContext.hpp>

#include <vlGraphics/Text.hpp>
#include <vlGraphics/FontManager.hpp>

#include <renderer/vl/AxisRenderer.hpp>
#include <Renderer/vl/GLFont.hpp>

using namespace vl;

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}



void drawCNString(const char* str) {
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	// 计算字符的个数
	// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	// 否则一个字节算一个字符
	len = 0;
	for(i=0; str[i]!='/0'; ++i)
	{
		if( IsDBCSLeadByte(str[i]) )
			++i;
		++len;
	}

	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len+1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'/0';

	// 逐个输出字符
	for(i=0; i<len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

//-----------------------------------------------------------------------------
const RenderQueue* AxisRenderer::render(const RenderQueue* render_queue, Camera* camera, real frame_clock)
{
  // skip if renderer is disabled

  if (enableMask() == 0)
    return render_queue;

  // enter/exit behavior contract

  //class InOutContract 
  //{
  //  RendererAbstract* mRenderer;
  //public:
  //  InOutContract(RendererAbstract* renderer, Camera* camera): mRenderer(renderer)
  //  {
  //    // increment the render tick.
  //    mRenderer->incrementRenderTick();

  //    // render-target activation.
  //    // note: an OpenGL context can have multiple rendering targets!
  //    mRenderer->framebuffer()->activate();

  //    // viewport setup.
  //    camera->viewport()->setClearFlags( mRenderer->clearFlags() );
  //    camera->viewport()->activate();

  //    // dispatch the renderer-started event.
  //    mRenderer->dispatchOnRendererStarted();

  //    // check user-generated errors.
  //    VL_CHECK_OGL()
  //  }

  //  ~InOutContract()
  //  {
  //    // dispatch the renderer-finished event
  //    mRenderer->dispatchOnRendererFinished();

  //    // check user-generated errors.
  //    VL_CHECK_OGL()
  //  }
  //} contract(this, camera);

  // --------------- rendering --------------- 

  //glPushAttrib(GL_ALL_ATTRIB_BITS);

  double dAspectRatio = camera->aspectRatio();

  dAspectRatio  = 1.0;
  double h = 100;
  double w = h*dAspectRatio;

  glViewport(0,0,w,h);
  glMatrixMode (GL_PROJECTION);		
 // glPushMatrix();
  glLoadIdentity ();	
  glOrtho(-1.0, 1.0, -1.0, 1.0,-1.0,1.0);

 // vl::mat4 mat = vl::mat4::getOrtho(0, w, 0, h, 0.5, 10000) * 
	//  vl::mat4::getTranslation(w / 2.0f, h / 2.0f, 0) ;

 //// VL_glMultMatrix(mat.ptr() );
 // glLoadMatrixf(mat.ptr() );

//  glPopMatrix();

  glMatrixMode(GL_MODELVIEW);
 // glPushMatrix();

  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glLoadIdentity();

  fmat4 vmat = (fmat4)camera->viewMatrix().as3x3();

//  VL_glMultMatrix( vmat.ptr() );
  glLoadMatrixf( vmat.ptr() );

  glLineWidth(1.4f); 
  float axislength = 0.5f;

  glBegin(GL_LINES);
  glColor4f(1.f,.0f,.0f,1.0f);
  glVertex2f(0.0,0.0);
  glVertex2f(axislength,0);
  glColor4f(0.f,1.0f,.0f,1.0f);
  glVertex2f(0,0);
  glVertex2f(0,axislength);
  glColor4f(0.f,0.0f,1.0f,1.0f);
  glVertex3f(0,0,0);
  glVertex3f(0,0,axislength);
  glEnd();

  //glColor4f(1.f,.0f,.0f,1.0f);
  //m_Xtext.text_out("x",axislength+0.01,0);
  //glColor4f(0.f,1.0f,.0f,1.0f);
  //m_Ytext.text_out("y",0,axislength+0.01);
  //glColor4f(0.f,0.0f,1.0f,1.0f);
  //m_Ztext.text_out("z",0,0,axislength+0.01);

 // glPopMatrix();

  //glColor3f(1.0f, 0.0f, 0.0f);
  //glRasterPos2f(0.0f, 0.0f);  
  //drawCNString("aaa");
  // framebuffer()->openglContext()->swapBuffers();


  //vl::ref<vl::Text> text = new vl::Text;
  //vl::ref<vl::Font> font = vl::defFontManager()->acquireFont("C:/Windows/Fonts/arial.ttf",5);

  //text->setFont(font.get());
  //text->setMode( vl::Text3D );
  //text->setText( L" X");
  //text->setColor(vl::red);
  //text->translate(axislength+0.01,0,0);
  //vl::ref<vl::Actor> actor = new vl::Actor(text.get());
  //vl::ref<vl::Viewport> vp = new vl::Viewport(0,0,w,h);
  //vl::ref<vl::Camera> cam = new vl::Camera;
  //cam->setViewport(vp.get());
  //actor->lod(0)->render( actor.get(), NULL, cam.get(), framebuffer()->openglContext() );


 // glClear(GL_COLOR_BUFFER_BIT);
  GLFont glFont;
  glFont.InitFont(_T("宋体"));
  glColor4f(1.0f,0.0f,0.0f,1.0f);

  glFont.m_X = axislength+0.01;
  glFont.m_Y = 0;
  glFont.m_Z = 0;
  glFont.m_dHeight = 0.2;
  //glFont.SetColor(255,0,0);
  glFont.Draw2DText("x");

  glColor4f(0.0f,1.0f,0.0f,1.0f);

  glFont.m_X = 0;
  glFont.m_Y = axislength+0.01;
  glFont.m_Z = 0;
  glFont.m_dHeight = 0.2;
 // glFont.SetColor(0,255,0);
  glFont.Draw2DText("y");

  glColor4f(0.0f,0.0f,1.0f,1.0f);

  glFont.m_X = 0;
  glFont.m_Y = 0;
  glFont.m_Z =  axislength+0.01;
  glFont.m_dHeight = 0.2;
  //glFont.SetColor(0,0,255);
  glFont.Draw2DText("z");

  glFlush();

  return render_queue;
}
