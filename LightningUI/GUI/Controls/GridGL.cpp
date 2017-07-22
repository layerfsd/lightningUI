#include "GridGL.h"

CGridGL::CGridGL(void)
{
	m_iLowX=-10;
	m_iLowY=-10;
	m_iHighX=10;
	m_iHighY=10;
	m_iCurX=0;
	m_iCurY=0;
	//m_ptexBase=NULL;
	m_bEnable = TRUE;	
	m_bValueChanged=FALSE;
	m_bHasMoveNotify = TRUE;
	m_iMargin = 0;

	m_bClickOnBtns = FALSE;
}

CGridGL::~CGridGL(void)
{
}

BOOL CGridGL::Initialize(UINT id,CControlGL *pParent,CSkinManagerGL *pSkinManager,CTextGL *pTextGL)
{
// 	SIZE si;
	m_pSkinManager=pSkinManager;
	m_ID=id;
	m_pTextGL=pTextGL;
	SetParent(pParent);

	//
		m_iconUp.Initialize(0,this,pSkinManager->GetTexture(TEXID_081));
		m_iconDown.Initialize(0,this,pSkinManager->GetTexture(TEXID_080));
		m_iconRight.Initialize(0,this,pSkinManager->GetTexture(TEXID_083));
		m_iconLeft.Initialize(0,this,pSkinManager->GetTexture(TEXID_082));

		//controls:
		m_btUp.Initialize(0,this,pSkinManager->GetTexture(TEXID_085),NULL,
			pSkinManager->GetTexture(TEXID_086),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btUp.SetIcon(&m_iconUp);

		m_btDown.Initialize(0,this,pSkinManager->GetTexture(TEXID_085),NULL,
			pSkinManager->GetTexture(TEXID_086),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btDown.SetIcon(&m_iconDown);

		m_btLeft.Initialize(0,this,pSkinManager->GetTexture(TEXID_085),NULL,
			pSkinManager->GetTexture(TEXID_086),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btLeft.SetIcon(&m_iconLeft);

		m_btRight.Initialize(0,this,pSkinManager->GetTexture(TEXID_085),NULL,
			pSkinManager->GetTexture(TEXID_086),NULL,
			NULL,NULL,
			NULL,NULL);
		m_btRight.SetIcon(&m_iconRight);

		m_tagWave.Initialize(0,this,pSkinManager->GetTexture(TEXID_613));

		m_tagLineHor.Initialize(0,this,pSkinManager->GetTexture(TEXID_612));
		m_tagLineVer.Initialize(0,this,pSkinManager->GetTexture(TEXID_612));

		VIEW_STATE vs=m_tagLineVer.GetCurViewState();
		vs.fRotateZ=90;
		m_tagLineVer.SetCurViewState(&vs);

	return TRUE;
}

void CGridGL::Render(void)
{
	if(!BeginRender())return;


	if(m_bEnableRender && IsVisible() )
	{
		//draw border:
/*		static CTexture *ptex=m_pSkinManager->GetTexture(TEXID_553);
		if(ptex)
		{
			COLORGL clr={m_fCurLuminance,m_fCurLuminance,m_fCurLuminance,m_fCurAlpha};
			DrawRect(ptex,0,0,460,400,14,&clr,TRUE);
		}
*/

// 		glTranslatef(0,0,1);

		/*
		//draw base:
		glVertexPointer(3, GL_FLOAT, 0, m_pVertex);
		//glTexCoordPointer(2, GL_FLOAT, 0, m_pTexCoord);
		glEnableClientState(GL_VERTEX_ARRAY);
		//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glEnable(GL_TEXTURE_2D);
		glColor4f(m_crCurColor.r,m_crCurColor.g,m_crCurColor.b,m_fCurAlpha);//(0.1f,0.36f,0.5f,0.6f);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);//GL_REPLACE
		if(m_ptexBase)
		{
			glBindTexture(GL_TEXTURE_2D, m_ptexBase->GetID());
			UpdateTexCoord(m_ptexBase);
		}
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		*/
	
		/*
		GLfloat py=(GLfloat)(m_iCurY-m_iLowY)*(GLfloat)(m_siCurSize.cy-10)/(GLfloat)(m_iHighY-m_iLowX) - (GLfloat)m_siCurSize.cy/2 +5;
		GLfloat px=(GLfloat)(m_iCurX-m_iLowX)*(GLfloat)(m_siCurSize.cx-10)/(GLfloat)(m_iHighX-m_iLowX) - (GLfloat)m_siCurSize.cx/2 +5;//margin 10
		//draw line:
		GLfloat line[]={	
			-(GLfloat)m_siCurSize.cx/2-40+m_iMargin,	py,	0.0f,
			(GLfloat)m_siCurSize.cx/2+40-m_iMargin,	py,	0.0f,
			px,(GLfloat)m_siCurSize.cy/2+40-m_iMargin,	0.0f,
			px,-(GLfloat)m_siCurSize.cy/2-40+m_iMargin,	0.0f,

			-(GLfloat)m_siCurSize.cx/2-40+m_iMargin,	py,	0.0f,
			(GLfloat)m_siCurSize.cx/2+40-m_iMargin,	py,	0.0f,
			px,(GLfloat)m_siCurSize.cy/2+40-m_iMargin,	0.0f,
			px,-(GLfloat)m_siCurSize.cy/2-40+m_iMargin,	0.0f
		};
// 		glTranslatef(0,0,1);
		glDisable(GL_TEXTURE_2D);
		glVertexPointer(3, GL_FLOAT, 0, line);
		glLineWidth(1.0);
// 		glColor4f(0.0f,0.0f,0.0f,1.0);
// 		glDrawArrays(GL_LINES, 0, 2);
// 		glDrawArrays(GL_LINES, 2, 2);
		glColor4f(1.0f,1.0f,1.0f,m_fCurAlpha);
// 		glLineWidth(1.0);
		glDrawArrays(GL_LINES, 4, 2);
		glDrawArrays(GL_LINES, 6, 2);
// 		glTranslatef(0,0,2);
		*/

		m_tagLineVer.SetAlpha(m_fCurAlpha);
		m_tagLineVer.Render();

		m_tagLineHor.SetAlpha(m_fCurAlpha);
		m_tagLineHor.Render();

		//draw controls:
		m_btUp.SetAlpha(m_fCurAlpha);
		m_btDown.SetAlpha(m_fCurAlpha);
		m_btLeft.SetAlpha(m_fCurAlpha);
		m_btRight.SetAlpha(m_fCurAlpha);

		m_btUp.Render();
		m_btDown.Render();
		m_btLeft.Render();
		m_btRight.Render();

// 		static GLfloat scale1=0.1f;
// 		m_tagWave.SetPos(px,py);
// 		m_tagWave.SetScale(scale1,scale1,scale1);
// 		m_tagWave.Render();
// 		scale1+=0.02;
// 		if(scale1>1.0f)scale1=0.1f;
// 
// 		static GLfloat scale2=0.6f;
// 		m_tagWave.SetPos(px,py);
// 		m_tagWave.SetScale(scale2,scale2,scale2);
// 		m_tagWave.Render();
// 		scale2+=0.02;
// 		if(scale2>1.0f)scale2=0.1f;

		m_tagWave.SetAlpha(m_fCurAlpha);
		m_tagWave.Render();

		//draw value:
		/*if(m_pTextGL)
		{
			SIZE si;
			COLORGL cr={1.0,1.0,1.0,1.0};
			wchar_t str[16];
			m_pTextGL->SetParent(this);
			swprintf_s(str,16,L"%d",m_iCurY);
			m_pTextGL->SetString(str);
			//m_pTextGL->SetScale(0.85,1.0,1.0,FALSE);
			m_pTextGL->SetCharScale(1.0f,1.0f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(GLfloat(-si.cx/2),GLfloat(m_siCurSize.cy/2));
			m_pTextGL->SetColor(&cr,FALSE);
			m_pTextGL->SetAlpha(m_fCurAlpha,FALSE);
			m_pTextGL->SetLuminance(m_fCurLuminance);
			m_pTextGL->Render();

			swprintf_s(str,16,L"%d",m_iCurX);
			m_pTextGL->SetString(str);
			//m_pTextGL->SetScale(0.85,1.0,1.0,FALSE);
			m_pTextGL->SetCharScale(1.0f,1.0f);
			m_pTextGL->GetExtension(&si);
			m_pTextGL->SetPos(GLfloat(-m_siCurSize.cx/2),GLfloat(si.cy/2));
			m_pTextGL->SetColor(&cr,FALSE);
			m_pTextGL->Render();
		}*/

	}
	EndRender();
}

void CGridGL::Enable(BOOL bEnable)
{
	m_bEnable = bEnable;
	CControlGL::Enable(bEnable);
}

BOOL CGridGL::IsPtnOnObject(POINT *pt)
{
	return CObjectGL::IsPtnOnObject(pt) 
		|| m_btDown.IsPtnOnObject(pt)
		 || m_btUp.IsPtnOnObject(pt)
		  || m_btLeft.IsPtnOnObject(pt)
		   || m_btRight.IsPtnOnObject(pt);
}
void CGridGL::OnTouchDown(POINT *pt)
{
	if(!m_bEnable)
		return;

	m_bClickOnBtns = FALSE;

	m_btUp.OnTouchDown(pt);
	m_btDown.OnTouchDown(pt);
	m_btLeft.OnTouchDown(pt);
	m_btRight.OnTouchDown(pt);

	if(CObjectGL::IsPtnOnObject(pt))//client only
	{
		m_bActived = TRUE;
	}

	//
	if(m_btDown.IsPtnOnObject(pt)||m_btUp.IsPtnOnObject(pt)||m_btRight.IsPtnOnObject(pt)||m_btLeft.IsPtnOnObject(pt))
	{
		m_bClickOnBtns = TRUE;
	}
}
void CGridGL::OnTouchUp(POINT *pt)
{
	if(!m_bEnable)
		return;

	m_btUp.OnTouchUp(pt);
	m_btDown.OnTouchUp(pt);
	m_btLeft.OnTouchUp(pt);
	m_btRight.OnTouchUp(pt);


	if(m_btRight.IsPtnOnObject(pt))
	{
		int iset=m_iCurX+1;
		if(iset>m_iHighX)
			iset=m_iHighX;
		SetValue(iset,m_iCurY);
		m_bActived=TRUE;
	}
	else if(m_btLeft.IsPtnOnObject(pt))
	{
		int iset=m_iCurX-1;
		if(iset<m_iLowX)
			iset=m_iLowX;
		SetValue(iset,m_iCurY);
		m_bActived=TRUE;
	}
	else if(m_btUp.IsPtnOnObject(pt))
	{
		int iset=m_iCurY+1;
		if(iset>m_iHighY)
			iset=m_iHighY;
		SetValue(m_iCurX,iset);
		m_bActived=TRUE;
	}
	else if(m_btDown.IsPtnOnObject(pt))
	{
		int iset=m_iCurY-1;
		if(iset<m_iLowY)
			iset=m_iLowY;
		SetValue(m_iCurX,iset);
		m_bActived=TRUE;
	}

// 	if(m_bActived)
// 	{
// 		if(m_lpNotifyFunc)
// 		{
// 			short x=short(m_iCurX);
// 			short y=short(m_iCurY);
// 
// 			m_lpNotifyFunc(m_ID,NOTIFY_VALUE_CHANGE,(((WORD)x)<<16)+((WORD)y),NULL,m_lparam);
// 		}
// 	}

	m_bActived = FALSE;
}
void CGridGL::OnTouchMove(POINT *pt)
{
	if(!m_bEnable)
		return;

	if (m_bActived && !m_bClickOnBtns/*IsPtnOnObject(pt)*/ /*&& !IsOnAdjustBtn(pt)*/)
	{
		SetValue(m_iLowX+(GLfloat)(pt->x-GetScreenRect().left-m_iMargin)*(GLfloat)(m_iHighX-m_iLowX)/(GLfloat)(m_siCurSize.cx-m_iMargin*2),
			m_iLowY+(GLfloat)(-pt->y+GetScreenRect().bottom-m_iMargin)*(GLfloat)(m_iHighY-m_iLowX)/(GLfloat)(m_siCurSize.cy-m_iMargin*2),TRUE);
	}
}
void CGridGL::OnGesture(GESTUREINFO *pgi)
{

}
void CGridGL::SetPos(GLfloat x,GLfloat y,GLfloat z,BOOL bAni)
{
	CObjectGL::SetPos(x,y,z,bAni);

	m_btUp.SetPos(0.0f,GLfloat(m_siCurSize.cy/2-m_btUp.GetSize().cy/2-5));

	m_btDown.SetPos(0.0f,GLfloat(-m_siCurSize.cy/2+m_btDown.GetSize().cy/2+5));

	m_btLeft.SetPos(GLfloat(-m_siCurSize.cx/2+m_btLeft.GetSize().cx/2+5),0.0f);

	m_btRight.SetPos(GLfloat(m_siCurSize.cx/2-m_btRight.GetSize().cx/2-5),0.0f);

	m_tagLineHor.SetScale((m_siCurSize.cx-m_iMargin*2+60)/64.0f,1.0f);
	m_tagLineVer.SetScale((m_siCurSize.cy-m_iMargin*2+60)/64.0f,1.0f);
}

BOOL CGridGL::IsReady(void)
{
	return CControlGL::IsReady() &&
		m_btUp.IsReady() &&
		m_btDown.IsReady() &&
		m_btLeft.IsReady() &&
		m_btRight.IsReady() ;
}
void CGridGL::SetValueX(int x)
{
	SetValue(x,m_iCurY,FALSE);
}
void CGridGL::SetValueY(int y)
{
	SetValue(m_iCurX,y,FALSE);
}
void CGridGL::SetMargin(int iMargin)
{
	m_iMargin = iMargin;
}
void CGridGL::SetValue(int x,int y,BOOL bNotify)
{
	int last_x=m_iCurX;
	int last_y=m_iCurY;

	m_iCurX=x;
	if(m_iCurX>m_iHighX)
		m_iCurX=m_iHighX;
	else if(m_iCurX<m_iLowX)
		m_iCurX=m_iLowX;

	m_iCurY=y;
	if(m_iCurY>m_iHighY)
		m_iCurY=m_iHighY;
	else if(m_iCurY<m_iLowY)
		m_iCurY=m_iLowY;

	m_bValueChanged=(m_iCurX!=last_x)||(m_iCurY!=last_y);

	if(m_bValueChanged)
	{
		GLfloat py=(GLfloat)(m_iCurY-m_iLowY)*(GLfloat)(m_siCurSize.cy-m_iMargin*2)/(GLfloat)(m_iHighY-m_iLowX) - (GLfloat)m_siCurSize.cy/2 +m_iMargin;
		GLfloat px=(GLfloat)(m_iCurX-m_iLowX)*(GLfloat)(m_siCurSize.cx-m_iMargin*2)/(GLfloat)(m_iHighX-m_iLowX) - (GLfloat)m_siCurSize.cx/2 +m_iMargin;

		m_tagWave.SetPos(px,py);

		m_tagLineHor.SetPos(0,py);
		m_tagLineVer.SetPos(px,0);

		if(m_lpNotifyFunc && bNotify)
		{
			short x=short(m_iCurX);
			short y=short(m_iCurY);

			m_lpNotifyFunc(m_ID,NOTIFY_VALUE_CHANGE,(((WORD)x)<<16)+((WORD)y),NULL,m_lparam);
		}
	}

}