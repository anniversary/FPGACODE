// DlgSendMsg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "DlgSendMsg.h"
#include "afxdialogex.h"


// CDlgSendMsg �Ի���

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


// CDlgSendMsg ��Ϣ�������
void CDlgSendMsg::OnBnClickedMsgSend()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	tagMsgParam *pMsgA =new tagMsgParam;
	m_msgEdit.GetWindowTextW(pMsgA->msg);
	if(pMsgA->msg==_T(""))
	{
		AfxMessageBox(_T("������Ҫ���͵����ݣ�"),MB_OK|MB_ICONWARNING);
		return;
	}
	else
	{
		//��ʼ���Ͷ���Ϣ�е�����
		char err_content[PCAP_ERRBUF_SIZE]; //���ش�����Ϣ
		CString  strDeviceName;
		strDeviceName.Format(_T("%s%s"),_T("\\Device\\NPF_"),(CStringW)(theApp.pIpAdapter_user->AdapterName));  //�˴�������CStringW����ǿ��ת����������������
		pcap_t  *pcap_handle = pcap_open_live((char *)(LPCTSTR)strDeviceName,65535,1,1000,err_content);
		if (pcap_handle  == NULL)
		{
			AfxMessageBox(_T("�޷���ȡ��������Ȩ��"),NULL,MB_ICONERROR);
			return;
		}
		pMsgA->adhandle = pcap_handle;
		SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_MSG,(WPARAM)pMsgA,NULL);//�������Ͷ���Ϣ���������߳�
	}
	if (!pMsgA)
	{
		delete pMsgA;
	}
	CDialogEx::OnOK();
}
