#include "stdafx.h"
#include "PhyObj.h"
#include "PhyEnv.h"

CPhyObj::CPhyObj(double mass, SPointF pos, SVecF vel, SVecF acc, Image *pImg, double rec)
{
	m_rec = rec;
	m_mass = mass;
	m_vel = vel;
	m_acc = acc;
	m_pos = pos;
	m_pImg = pImg;
	m_realAcc = m_acc;
	m_pRgn = new CRgn();

	CRgn &m_rgn = *m_pRgn;
	PixelFormat pf = m_pImg->GetPixelFormat();
	int width = m_pImg->GetWidth();
	int height = m_pImg->GetHeight();

	if (GetPixelFormatSize(pf) == 32) {
		BitmapData Bitdata;
		Rect rect(0, 0, width, height);
		((Bitmap*)m_pImg)->LockBits(&rect, ImageLockModeRead, pf, &Bitdata);
		char *p = (char*)Bitdata.Scan0;
		m_rgn.CreateRectRgn(0, 0, 0, 0);
		for (int i = 0; i<height; ++i) {
			for (int j = 0; j<width; ++j) {
				int j2;
				for (j2 = j; j2 < width; ++j2) {
					if (*(p + 4 * width * i + 4 * j2 + 3) == 0)
						break;
				}
				if (j2>j) {
					CRgn trgn;
					trgn.CreateRectRgn(j, i, j2, i + 1);
					CombineRgn(m_rgn, trgn, m_rgn, RGN_OR);
				}
			}
		}
		((Bitmap*)m_pImg)->UnlockBits(&Bitdata);
	}
	else m_rgn.CreateRectRgn(0, 0, width, height);
}

CPhyObj::~CPhyObj()
{
	
}

bool CPhyObj::CanGo(CPhyEnv *env, SVecF displace)
{
	int n = env->m_objs.size();
	for (int i = 0; i < n; ++i) {
		//if(env->m_objs[i])
	}
	//1. �����е�ǰδ�Ӵ�������Ĳ���U
	//2. �����S
	//3. U��S
	return true;
}

void CPhyObj::Go(CPhyEnv *env, double tDelta)
{
	//if (!CanGo()) {}
	m_vel = m_vel + m_realAcc * tDelta;
	SVecF displace = m_vel * tDelta + m_realAcc * 0.5 * tDelta * tDelta;
	//if (this == &(env->m_objs[1]))
	//	TRACE("%lf %lf\n",displace.x,displace.y);
	m_pos = m_pos + displace;
	
	//BUG1: ��������ײ��һ�������ȸı��˶�״̬���������������巴��������ײ��������������������������
	//BUG2: �м��ٶ�ʱ����ײ�ж����󣬵����������ݣ���������������������������������������������������
	//��ײ���ٶȼ�ʹ��ͬ������ٶȲ�ͬ��������Ȼ�����ݣ���
	//��һ������������Ҳ�������ͬ�ļ��ٶȡ���������A�ļ��ٶ�Ҳ���ܸı䡣��
	//��ײʱ���趨��ͬ��˲ʱ���ٶȣ���
	//�������ͬʱ��������δ�������
	//ÿ��update�����鼯 �ϲ��н��������壬Ȼ����� ʵ�ʼ��ٶȣ�����ֵ��Ӱ��ԭ���ٶ�
	//Ħ�����޷���������ϵͳ�������������໥����
	//BUG3: �������淽����󣡣�����������������������������������������������������������������������

	//���⣺
	//��ײ�����ģ���λ��(��һ��λ�ƹ���Ҫ��С��ֹ��ǽ����һ��λ�ƹ�С��Ҫ�Ŵ��Է���ⲻ��λ��)
	//���α䡱�����Զ��ָ����ü��ٶȻָ���������ǿ����Ϊ����λ�ã��������ϴ�go֮ǰ���ж�����
	//�ٶȹ���A֡��A+1ֱ֡�Ӵ������壩���ٶȹ�С�������������ײ���ʧЧ�����ᵼ�¡���ǽ������
	//Ħ�����Ĵ��� 
	//�������淽��
}

bool CPhyObj::SolveCollision(CPhyEnv *env, double tDelta)
{
	bool isOK = true;
	for (int i = 0, _m = env->m_objs.size(); i < _m; ++i) if (this != &(env->m_objs[i])) {
		if (IsCollision(env->m_objs[i], true, tDelta)) {
			Collision(env->m_objs[i]);
			isOK = false;
		}
	}
	return isOK;
}

bool CPhyObj::IsCollision(CPhyObj &ano, bool shapeChange, double tDelta)
{
	if (&ano == this) return false;
	CRgn rgn1, rgn2;
	rgn1.CreateRectRgn(0, 0, 0, 0);
	rgn2.CreateRectRgn(0, 0, 0, 0);
	rgn1.CopyRgn(m_pRgn);
	rgn2.CopyRgn(ano.m_pRgn);
	rgn1.OffsetRgn((int)(m_pos.x),(int)(m_pos.y));
	rgn2.OffsetRgn((int)(ano.m_pos.x), (int)(ano.m_pos.y));

	CRgn rgnOvlap;
	rgnOvlap.CreateRectRgn(0, 0, 0, 0);
	int res = rgnOvlap.CombineRgn(&rgn1, &rgn2, RGN_AND);
	if (res == COMPLEXREGION || res == SIMPLEREGION) { //��ǰ֡�нӴ�
		if (!shapeChange) {
			return true;
		}
		//����˶������ж�����һ֡�Ӵ����
		CRect rect;
		rgnOvlap.GetRgnBox(&rect);
		
		SVecF displace1 = m_vel * tDelta + m_realAcc * 0.5 * tDelta * tDelta;
		SVecF displace2 = ano.m_vel * tDelta + ano.m_realAcc * 0.5 * tDelta * tDelta;
		rgn1.OffsetRgn(-(int)(m_pos.x) + (int)(m_pos.x + displace1.x), -(int)(m_pos.y) + (int)(m_pos.y + displace1.y));
		rgn2.OffsetRgn(-(int)(ano.m_pos.x) + (int)(ano.m_pos.x + displace2.x), -(int)(ano.m_pos.y) + (int)(ano.m_pos.y + displace2.y));
		

		//TRACE("%lf %lf\n", displace2.x, displace2.y);

		CRgn rgnOvlap_;
		rgnOvlap_.CreateRectRgn(0, 0, 0, 0);
		rgnOvlap_.CombineRgn(&rgn1, &rgn2, RGN_AND);
		CRect rect_;
		rgnOvlap_.GetRgnBox(&rect_);
		//TRACE("rect:%d %d %d rect_:%d %d %d\n", rect.Width(), rect.Height(), rect.Height() * rect.Width(),
		//	rect_.Width(), rect_.Height(), rect_.Height() * rect_.Width());
		return rect_.Height() * rect_.Width() > rect.Height() * rect.Width();
	} else return false;
}

void CPhyObj::Collision(CPhyObj &ano)
{
	double m1 = m_mass, m2 = ano.m_mass;
	SVecF v1 = m_vel, v2 = ano.m_vel;
	double e = sqrt(m_rec * ano.m_rec);
	m_vel = (v1 * m1 + v2 * m2 + (v2 - v1) * e * m2)/(m1 + m2);
	ano.m_vel = (v1 * m1 + v2 * m2 + (v1 - v2) * e * m1) / (m1 + m2);
}
