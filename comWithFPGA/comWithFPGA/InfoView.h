#pragma once

#include "constdef.h"
// CInfoView 视图

class CInfoView : public CTreeView
{
	DECLARE_DYNCREATE(CInfoView)

protected:
	CInfoView();           // 动态创建所使用的受保护的构造函数
	virtual ~CInfoView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	afx_msg  LRESULT OnUpdatePacketContent(WPARAM wParam,LPARAM lParam); //显示数据包的内容
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};


