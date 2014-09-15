#pragma once

//2014.4.6 zsy
#include "constdef.h"
#include "MacEdit.h"
#include "DlgSendMsg.h"  //2014.8.23 zsy
//2014.4.6 zsy
// CSendChoice �Ի���

class CSendChoice : public CDialogEx
{
	DECLARE_DYNAMIC(CSendChoice)

public:
	CSendChoice(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSendChoice();

// �Ի�������
	enum { IDD = IDD_PACKET_SEND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	// -1δѡ�У�0���Ͷ���Ϣ��1�����ļ���2����ͼ��3��������
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
