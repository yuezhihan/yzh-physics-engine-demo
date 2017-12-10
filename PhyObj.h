#pragma once
#include "Vec.h"
class CPhyEnv;
class CPhyObj
{
public:
	double m_rec; //�ָ�ϵ��(0~1)
	double m_mass; //����
	SPointF m_pos; //��Ⱦλ��
	
	SVecF m_vel; //�ٶ�
	SVecF m_acc; //�������ٶ�

	SVecF m_realAcc; //��ʵ���ٶ�(��������ϵͳ�ڵ���ײ��Ħ��)

	Image *m_pImg;
	CRgn *m_pRgn; //ͼ������
	
	CPhyObj(double mass, SPointF pos, SVecF vel, SVecF acc, Image *pImg, double rec);
	bool IsCollision(CPhyObj &ano, bool sharpChange = true, double tDelta = 0.3);
	void Collision(CPhyObj &ano);
	bool SolveCollision(CPhyEnv *env, double tDelta);

	bool CanGo(CPhyEnv *env, SVecF displace);
	void Go(CPhyEnv *env, double tDelta);
	~CPhyObj();
};

