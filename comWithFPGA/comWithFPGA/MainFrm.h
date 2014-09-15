
// MainFrm.h : CMainFrame ��Ľӿ�
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
UINT ThreadMsgSend(LPVOID pParam); //2014.8.23 zsy  ����Ϣ���ݷ����߳�
UINT ThreadVoiceSend(LPVOID pParam); //2014.8.31 zsy �������ݷ����߳�
UINT ThreadPictureSend(LPVOID pParam);//2014.9.4 zsy ͼƬ�����߳�
void ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);

//2014.3.27 zsy
class CMainFrame : public CFrameWndEx
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:
// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	//CMFCStatusBar     m_wndStatusBar;
	CMFCToolBarImages m_UserImages;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);

	//2014.3.27 zsy
	afx_msg LRESULT OnUpdateSourMac(WPARAM wParam,LPARAM lParam);  //����ֵ���Ͳ���Ϊvoid��������LRESULT������������
	afx_msg LRESULT OnUpdateDesMac(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateSendDesMac(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUpdateSendFile(WPARAM wParam,LPARAM lParam); //����ѡ����ļ��������ݰ�
	afx_msg LRESULT OnUpdateSendMSG(WPARAM wParam,LPARAM lParam); //2014.8.23 zsy ����ѡ��Ķ���Ϣ�������ݰ�
	afx_msg LRESULT OnUpdateProgress(WPARAM wParam,LPARAM lParam); //2014.8.26 zsy ���½�����
	afx_msg LRESULT OnUpdateSendVoice(WPARAM wParam,LPARAM lParam); //2014.8.31 zsy ���������ļ���Ϣ
	afx_msg LRESULT OnUpdateSendPicture(WPARAM wParam,LPARAM lParam);//2014.9.4 zsy ����ͼƬ��Ϣ
	afx_msg LRESULT OnUpdateSendSuspend(WPARAM wParam,LPARAM lParam);//2014.9.12 zsy ������ͣ
	//2014.3.27 zsy
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CMFCStatusBar     m_wndStatusBar; //2014.8.26 zsy �޸�״̬��������Ϊprotect,�����߳��ڵĵ���
	CSplitterWnd m_wndSplitter;
	//2014.5.9
	BOOL b_enableMenu_Receive;//�Ƿ�ʼ�������ݣ���ʼ����Ϊfalse��δ����Ϊtrue��
	//BOOL b_oneRecThread;//�Ƿ���Ҫ�˳�ThreadPacketView�̣߳�false���˳���2014.8.23
	BOOL b_enableMenu_StopReceive;//ֹͣ���ղ˵��Ŀ��Ƽ���false�����ã�true����
    //BOOL b_enableThread_Receive;//���ƽ����̵߳���ֹ true������ֹ�̣߳�false����ִ�в���
	BOOL b_enableMenu_Send;//2014.8.31 �Ƿ�ʼ�������ݣ�δ��ʼ����Ϊture����ʼΪfalse
	BOOL b_enableMenu_StopSend;//2014.8.31 �Ƿ�ֹͣ���ݷ��ͣ�δֹͣΪtrue��ֹͣΪfalse
	BOOL b_suspend_send; //2014.9.12 �Ƿ���ͣ�������ݣ�����ͣΪtrue����ͣΪfalse��
	//2014.5.9
	afx_msg void OnStartRecv();
	// ��ÿ�������������ϸ��Ϣ
	bool GetEnableAdapterInfo(void);
	afx_msg void OnStopRecv();
//	afx_msg void OnSendProp();
	afx_msg void OnStartSend();
	afx_msg void OnUpdateStartRecv(CCmdUI *pCmdUI);
	// �������ؼ�
	CProgressCtrl m_progress;
	afx_msg void OnUpdateStopRecv(CCmdUI *pCmdUI);
	afx_msg void OnStopSend();
	afx_msg void OnUpdateStartSend(CCmdUI *pCmdUI);
	afx_msg void OnUpdateStopSend(CCmdUI *pCmdUI);
};


