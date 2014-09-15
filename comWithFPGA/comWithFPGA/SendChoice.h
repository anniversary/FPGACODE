#pragma once

//2014.4.6 zsy
#include "constdef.h"
#include "MacEdit.h"
#include "DlgSendMsg.h"  //2014.8.23 zsy
//2014.4.6 zsy
// CSendChoice 对话框

class CSendChoice : public CDialogEx
{
	DECLARE_DYNAMIC(CSendChoice)

public:
	CSendChoice(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSendChoice();

// 对话框数据
	enum { IDD = IDD_PACKET_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	// -1未选中，0发送短消息，1发送文件，2发送图像，3发送语音
	int m_send_choice;
	int m_breaktime;
	int m_packetLength;
	CMacEdit m_mac_edit1;
	CMacEdit m_mac_edit2;
	CMacEdit m_mac_edit3;
	CMacEdit m_mac_edit4;
	CMacEdit m_mac_edit5;
	CMacEdit m_mac_edit6;
};
