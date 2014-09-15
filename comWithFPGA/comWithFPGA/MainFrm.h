
// MainFrm.h : CMainFrame 类的接口
//

#pragma once


//2014.3.27 zsy
#include <afxtempl.h>
#include "pcap.h"
//#include "remote-ext.h"
#include "DlgSavedata.h"
#include "afxcmn.h"
UINT ThreadPacketView(LPVOID pParam);
UINT ThreadFileSend(LPVOID pParam);
UINT ThreadMsgSend(LPVOID pParam); //2014.8.23 zsy  短消息数据发送线程
UINT ThreadVoiceSend(LPVOID pParam); //2014.8.31 zsy 声音数据发送线程
UINT ThreadPictureSend(LPVOID pParam);//2014.9.4 zsy 图片发送线程
void ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);

//2014.3.27 zsy
class CMainFrame : public CFrameWndEx
{
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
public:
// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	//CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);

	//2014.3.27 zsy
	afx_msg LRESULT OnUpdateSourMac(WPARAM wParam,LPARAM lParam);  //返回值类型不能为void，必须是LRESULT，否则编译出错
	afx_msg LRESULT OnUpdateDesMac(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateSendDesMac(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateSendFile(WPARAM wParam,LPARAM lParam); //根据选择的文件发送数据包
	afx_msg LRESULT OnUpdateSendMSG(WPARAM wParam,LPARAM lParam); //2014.8.23 zsy 根据选择的短消息发送数据包
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam,LPARAM lParam); //2014.8.26 zsy 更新进度条
	afx_msg LRESULT OnUpdateSendVoice(WPARAM wParam,LPARAM lParam); //2014.8.31 zsy 发送声音文件消息
	afx_msg LRESULT OnUpdateSendPicture(WPARAM wParam,LPARAM lParam);//2014.9.4 zsy 发送图片消息
	afx_msg LRESULT OnUpdateSendSuspend(WPARAM wParam,LPARAM lParam);//2014.9.12 zsy 发送暂停
	//2014.3.27 zsy
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CMFCStatusBar     m_wndStatusBar; //2014.8.26 zsy 修改状态栏的属性为protect,方便线程内的调用
	CSplitterWnd m_wndSplitter;
	//2014.5.9
	BOOL b_enableMenu_Receive;//是否开始接收数据，开始接收为false，未接收为true；
	//BOOL b_oneRecThread;//是否需要退出ThreadPacketView线程，false不退出。2014.8.23
	BOOL b_enableMenu_StopReceive;//停止接收菜单的控制键，false不可用，true可用
    //BOOL b_enableThread_Receive;//控制接收线程的终止 true：请终止线程；false：不执行操作
	BOOL b_enableMenu_Send;//2014.8.31 是否开始发送数据，未开始发送为ture，开始为false
	BOOL b_enableMenu_StopSend;//2014.8.31 是否停止数据发送，未停止为true，停止为false
	BOOL b_suspend_send; //2014.9.12 是否暂停发送数据，不暂停为true，暂停为false；
	//2014.5.9
	afx_msg void OnStartRecv();
	// 获得可用适配器的详细信息
	bool GetEnableAdapterInfo(void);
	afx_msg void OnStopRecv();
//	afx_msg void OnSendProp();
	afx_msg void OnStartSend();
	afx_msg void OnUpdateStartRecv(CCmdUI *pCmdUI);
	// 进度条控件
	CProgressCtrl m_progress;
	afx_msg void OnUpdateStopRecv(CCmdUI *pCmdUI);
	afx_msg void OnStopSend();
	afx_msg void OnUpdateStartSend(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStopSend(CCmdUI *pCmdUI);
};


