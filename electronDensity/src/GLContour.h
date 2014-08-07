// GLContour.h: interface for the CGLContour class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLCONTOUR_H__8E92F5AF_D733_47BA_9E52_F27D16391E2B__INCLUDED_)
#define AFX_GLCONTOUR_H__8E92F5AF_D733_47BA_9E52_F27D16391E2B__INCLUDED_

#include "ofMain.h"
#include "Contour.h"


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class ofApp;

class CGLContour : public CContour  
{
public:
	CGLContour();
	virtual ~CGLContour();


	// ofApp * app;

	virtual void SetFieldFcn(double (*_pFieldFcn)(ofApp*, double, double), ofApp * call_oject);

	// Drawing segment using OpenGL
	// See CContour::ExportLine for further details
	virtual inline void ExportLine(int iPlane,int x1, int y1, int x2, int y2);
};

inline void CGLContour::ExportLine(int iPlane,int x1, int y1, int x2, int y2)
{
	// cout << "GLContour::ExportLine(" << iPlane << ", " << x1 << ", " << y1 << ", " << x2 << ", " << y2 << ")" << endl;
	// glColor3f(iPlane/(float)GetNPlanes(),0,1.f-iPlane/(float)GetNPlanes());
	// glBegin(GL_LINES);
	// 	glVertex2f((GLfloat)(m_pLimits[0]+x1*m_dDx),(GLfloat)(m_pLimits[2]+y1*m_dDy));
	// 	glVertex2f((GLfloat)(m_pLimits[0]+x2*m_dDx),(GLfloat)(m_pLimits[2]+y2*m_dDy));
	// glEnd();
	// glColor3f(0,0,0);
	// glBegin(GL_POINTS);
	// 	glVertex2f((GLfloat)(m_pLimits[0]+x1*m_dDx),(GLfloat)(m_pLimits[2]+y1*m_dDy));
	// 	glVertex2f((GLfloat)(m_pLimits[0]+x2*m_dDx),(GLfloat)(m_pLimits[2]+y2*m_dDy));
	// glEnd();
	ofLine(x1,y1,x2,y2);
}

#endif // !defined(AFX_GLCONTOUR_H__8E92F5AF_D733_47BA_9E52_F27D16391E2B__INCLUDED_)
