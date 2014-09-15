#pragma once

#include "constdef.h"
// CInfoView ��ͼ

class CInfoView : public CTreeView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	virtual ~CInfoView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	afx_msg  LRESULT OnUpdatePacketContent(WPARAM wParam,LPARAM lParam); //��ʾ���ݰ�������
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


