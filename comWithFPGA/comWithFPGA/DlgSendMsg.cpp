// DlgSendMsg.cpp : 实现文件
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "DlgSendMsg.h"
#include "afxdialogex.h"


// CDlgSendMsg 对话框

IMPLEMENT_DYNAMIC(CDlgSendMsg, CDialogEx)

CDlgSendMsg::CDlgSendMsg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSendMsg::IDD, pParent)
{

}

CDlgSendMsg::~CDlgSendMsg()
{
}

void CDlgSendMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_MSG, m_msgEdit);
}


BEGIN_MESSAGE_MAP(CDlgSendMsg, CDialogEx)
	//ON_EN_CHANGE(IDC_EDIT1, &CDlgSendMsg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK_MSG, &CDlgSendMsg::OnBnClickedMsgSend)
END_MESSAGE_MAP()


// CDlgSendMsg 消息处理程序
void CDlgSendMsg::OnBnClickedMsgSend()
{
	// TODO: 在此添加控件通知处理程序代码
	tagMsgParam *pMsgA =new tagMsgParam;
	m_msgEdit.GetWindowTextW(pMsgA->msg);
	if(pMsgA->msg==_T(""))
	{
		AfxMessageBox(_T("请输入要发送的内容！"),MB_OK|MB_ICONWARNING);
		return;
	}
	else
	{
		//开始发送短消息中的内容
		char err_content[PCAP_ERRBUF_SIZE]; //返回错误信息
		CString  strDeviceName;
		strDeviceName.Format(_T("%s%s"),_T("\\Device\\NPF_"),(CStringW)(theApp.pIpAdapter_user->AdapterName));  //此处必须用CStringW进行强制转换，否则会出现乱码
		pcap_t  *pcap_handle = pcap_open_live((char *)(LPCTSTR)strDeviceName,65535,1,1000,err_content);
		if (pcap_handle  == NULL)
		{
			AfxMessageBox(_T("无法获取网卡控制权！"),NULL,MB_ICONERROR);
			return;
		}
		pMsgA->adhandle = pcap_handle;
		SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_MSG,(WPARAM)pMsgA,NULL);//开启发送短消息数据内容线程
	}
	if (!pMsgA)
	{
		delete pMsgA;
	}
	CDialogEx::OnOK();
}
