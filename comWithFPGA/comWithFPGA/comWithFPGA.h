
// comWithFPGA.h : comWithFPGA 应用程序的主头文件
//#include "constdef.h"
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

//2014.3.26
#include "constdef.h"
#include "pcap.h"
#include "..\..\IPHlpApi\IPHlpApi.h"
#pragma comment(lib,"iphlpapi.lib")
#include <afxtempl.h>
//2014.3.26


// CcomWithFPGAApp:
// 有关此类的实现，请参阅 comWithFPGA.cpp
//

class CcomWithFPGAApp : public CWinAppEx
{
public:
	CcomWithFPGAApp();

//2014.03.26
public:
	PIP_ADAPTER_INFO pIpAdapter_user; //本机主机详细信息
	//pcap_if_t *host_device; //本地主机可用适配器设备
	CArray<Packet_Info*,Packet_Info*> allPacket;  //所有接收到数据包信息，如果有重新接收选项则会被重置为0
	CArray<Packet_Info*,Packet_Info*> sendMsgPacket;//2014.8.23 zsy 要发送的短消息数据内容
	//CArray<HDLCParam*,HDLCParam*> sendHDLCMsg;//2014.9.19 zsy 发送的HDLC帧消息 
	CArray<SendPacket*,SendPacket*> sendPacket;//2014.9.20 zsy 发送的以太网帧
	CArray<SendPacket*,SendPacket*> sendErrPacket;//2014.9.21 zsy 请求重发的数据帧，每次正常发送时都应该先保证此动态数组中的元素全被发送
	long m_Number_packet;// 接收到的数据包的数目。如果有重新接收选项则会被重置为0
	CString str_des_macAddr;  //目的主机的MAC地址
	CString str_src_macAddr; //本地主机的MAC地址
	//CString str_send_des_macAddr; //发送端目的主机MAC地址
	u_char  c_des_macAddr[6];  //接收端目的主机MAC地址
	u_char  c_send_des_macAddr[6];//发送端目的主机MAC地址
	u_char  c_src_macAddr[6]; //本地主机MAC地址
	//BOOL isDesMac; //是否有目的主机的MAC地址输入，有的话，接收数据包时则按本地主机MAC和目的主机MAC同时过滤
	               //否则，只按本地主机MAC过滤数据包
	//BOOL m_isClear; //是否重新开始接受数据包，此操作将会清空之前显示的数据包。true时表示清空
	int m_breaktime; //以太网数据包发送间隔
	int m_packetLength; //以太网数据包数据区长度(46字节=<length<=1500字节，以太网数据包最大长度(7字节前导同步吗＋1字节帧开始定界符＋6字节的目的MAC＋6字节的源MAC＋2字节的帧类型＋1500数据区＋4字节的FCS=1026字节)

	CView *pMyView;   //视图变量，用来向回调函数中传递，掌管列表视图
	CView *pMyTreeView;  //视图变量，用来向回调函数中传递，掌管树视图
	pcap_t *theApp_receive_pcap_handle;  //控制数据监测的开始和停止
	pcap_t *theApp_send_pcap_handle;  //控制数据发送的开始和停止
	BOOL b_continueSend; //2014.8.31 zsy 控制单个数据帧是否继续发送，适用于自定义组帧发送。true:开始发送，false：停止发送 ；可用于线程的结束
	BOOL b_suspendSend; //2014.9.21 zsy 收到暂停发送命令，暂停发送；false：暂停发送，true：继续发送
	int i_radio;  //初始化为-1，选择保存为0，不保存为1
	int i_combox; // 保存文件的类型，数据包格式或者纯文本格式
	int i_openFileEx;//打开文件的类型
	CString  str_fileName; //保存数据的文件名
	CString  str_openfileName; //发送数据时选择打开的文件名
	int sendSerial;//当前发送序号

//2014.03.26
// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CcomWithFPGAApp theApp;
