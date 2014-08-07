// GLContour.cpp: implementation of the CGLContour class.
//
//////////////////////////////////////////////////////////////////////

// #include "stdafx.h"
#include "GLContour.h"
#include "ofApp.h"

// #ifdef _DEBUG
// #undef THIS_FILE
// static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
// #endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLContour::CGLContour()
: CContour()
{

}

CGLContour::~CGLContour()
{

}


void CGLContour::SetFieldFcn(double (*_pFieldFcn)(ofApp*, double, double), ofApp * call_object) 
{	
	// ofApp* app = static_cast<ofApp*>(call_object);
	app = call_object;
	m_pFieldFcn=_pFieldFcn;
};