#pragma once

//2014.3.30 zsy
#include "constdef.h"
//2014.3.30 zsy
// CRecvPackView 视图

class CRecvPackView : public CListView
{
	DECLARE_DYNCREATE(CRecvPackView)

protected:
	CRecvPackView();           // 动态创建所使用的受保护的构造函数
	virtual ~CRecvPackView();

	//消息映射函数
	afx_msg LRESULT OnUpdateListView(WPARAM wParam,LPARAM lParam);

public:

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};


