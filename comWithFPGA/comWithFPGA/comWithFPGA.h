
// comWithFPGA.h : comWithFPGA Ӧ�ó������ͷ�ļ�
//#include "constdef.h"
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

//2014.3.26
#include "constdef.h"
#include "pcap.h"
#include "..\..\IPHlpApi\IPHlpApi.h"
#pragma comment(lib,"iphlpapi.lib")
#include <afxtempl.h>
//2014.3.26


// CcomWithFPGAApp:
// �йش����ʵ�֣������ comWithFPGA.cpp
//

class CcomWithFPGAApp : public CWinAppEx
{
public:
	CcomWithFPGAApp();

//2014.03.26
public:
	PIP_ADAPTER_INFO pIpAdapter_user; //����������ϸ��Ϣ
	//pcap_if_t *host_device; //�������������������豸
	CArray<Packet_Info*,Packet_Info*> allPacket;  //���н��յ����ݰ���Ϣ����������½���ѡ����ᱻ����Ϊ0
	CArray<Packet_Info*,Packet_Info*> sendMsgPacket;//2014.8.23 zsy Ҫ���͵Ķ���Ϣ��������
	//CArray<HDLCParam*,HDLCParam*> sendHDLCMsg;//2014.9.19 zsy ���͵�HDLC֡��Ϣ 
	CArray<SendPacket*,SendPacket*> sendPacket;//2014.9.20 zsy ���͵���̫��֡
	CArray<SendPacket*,SendPacket*> sendErrPacket;//2014.9.21 zsy �����ط�������֡��ÿ����������ʱ��Ӧ���ȱ�֤�˶�̬�����е�Ԫ��ȫ������
	long m_Number_packet;// ���յ������ݰ�����Ŀ����������½���ѡ����ᱻ����Ϊ0
	CString str_des_macAddr;  //Ŀ��������MAC��ַ
	CString str_src_macAddr; //����������MAC��ַ
	//CString str_send_des_macAddr; //���Ͷ�Ŀ������MAC��ַ
	u_char  c_des_macAddr[6];  //���ն�Ŀ������MAC��ַ
	u_char  c_send_des_macAddr[6];//���Ͷ�Ŀ������MAC��ַ
	u_char  c_src_macAddr[6]; //��������MAC��ַ
	//BOOL isDesMac; //�Ƿ���Ŀ��������MAC��ַ���룬�еĻ����������ݰ�ʱ�򰴱�������MAC��Ŀ������MACͬʱ����
	               //����ֻ����������MAC�������ݰ�
	//BOOL m_isClear; //�Ƿ����¿�ʼ�������ݰ����˲����������֮ǰ��ʾ�����ݰ���trueʱ��ʾ���
	int m_breaktime; //��̫�����ݰ����ͼ��
	int m_packetLength; //��̫�����ݰ�����������(46�ֽ�=<length<=1500�ֽڣ���̫�����ݰ���󳤶�(7�ֽ�ǰ��ͬ����1�ֽ�֡��ʼ�������6�ֽڵ�Ŀ��MAC��6�ֽڵ�ԴMAC��2�ֽڵ�֡���ͣ�1500��������4�ֽڵ�FCS=1026�ֽ�)

	CView *pMyView;   //��ͼ������������ص������д��ݣ��ƹ��б���ͼ
	CView *pMyTreeView;  //��ͼ������������ص������д��ݣ��ƹ�����ͼ
	pcap_t *theApp_receive_pcap_handle;  //�������ݼ��Ŀ�ʼ��ֹͣ
	pcap_t *theApp_send_pcap_handle;  //�������ݷ��͵Ŀ�ʼ��ֹͣ
	BOOL b_continueSend; //2014.8.31 zsy ���Ƶ�������֡�Ƿ�������ͣ��������Զ�����֡���͡�true:��ʼ���ͣ�false��ֹͣ���� ���������̵߳Ľ���
	BOOL b_suspendSend; //2014.9.21 zsy �յ���ͣ���������ͣ���ͣ�false����ͣ���ͣ�true����������
	int i_radio;  //��ʼ��Ϊ-1��ѡ�񱣴�Ϊ0��������Ϊ1
	int i_combox; // �����ļ������ͣ����ݰ���ʽ���ߴ��ı���ʽ
	int i_openFileEx;//���ļ�������
	CString  str_fileName; //�������ݵ��ļ���
	CString  str_openfileName; //��������ʱѡ��򿪵��ļ���
	int sendSerial;//��ǰ�������

//2014.03.26
// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CcomWithFPGAApp theApp;
