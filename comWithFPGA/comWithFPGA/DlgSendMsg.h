#pragma once

#include "constdef.h" //2014.8.23
// CDlgSendMsg �Ի���

class CDlgSendMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSendMsg)

public:
	CDlgSendMsg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSendMsg();

// �Ի�������
	enum { IDD = IDD_PACKET_SEND_MESSAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnEnChangeEdit1();
	CEdit m_msgEdit;
	afx_msg void OnBnClickedMsgSend();
};
