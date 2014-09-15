#pragma once

#include "constdef.h" //2014.8.23
// CDlgSendMsg 对话框

class CDlgSendMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSendMsg)

public:
	CDlgSendMsg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSendMsg();

// 对话框数据
	enum { IDD = IDD_PACKET_SEND_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEdit1();
	CEdit m_msgEdit;
	afx_msg void OnBnClickedMsgSend();
};
