//#include <random>
// Birds.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CBirdsApp: 
// �йش����ʵ�֣������ Birds.cpp
//

class CBirdsApp : public CWinApp
{
public:
	CBirdsApp();
	ULONG_PTR m_gdiplusToken;
	std::default_random_engine m_randEng;

	BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image *&pImg);
	double Rand(double L, double R) {
		std::uniform_real_distribution<double> rander(L,std::nextafter(R,DBL_MAX));
		return rander(m_randEng);
	}
	int Rand(int L,int R) {
		std::uniform_int_distribution<int> rander(L,R);
		return rander(m_randEng);
	}
	LONGLONG GetTime() {
		LARGE_INTEGER liPerfFreq;
		QueryPerformanceFrequency(&liPerfFreq);
		LARGE_INTEGER liPerfNow;
		QueryPerformanceCounter(&liPerfNow);
		return liPerfNow.QuadPart * 1000 / liPerfFreq.QuadPart;
	}
// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CBirdsApp theApp;