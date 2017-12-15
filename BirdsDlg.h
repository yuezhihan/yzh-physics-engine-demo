
// BirdsDlg.h : ͷ�ļ�
//

#pragma once
#include "PhyEnv.h"
enum{BIRDX=50,BIRDY=260};
// CBirdsDlg �Ի���
class CBirdsDlg : public CDialogEx
{
// ����
public:
	double m_m1, m_k;
	
	bool m_running;
	HANDLE m_hThread;

	int m_num;
	bool m_go;
	CPoint m_posDown;

	CPhyEnv m_env;
	CBirdsDlg(CWnd* pParent = NULL);	// ��׼���캯��
	void InitEnv();
	static DWORD WINAPI CBirdsDlg::GameLoop(_In_ LPVOID lpParameter);

// �Ի�������
	enum { IDD = IDD_BIRDS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
