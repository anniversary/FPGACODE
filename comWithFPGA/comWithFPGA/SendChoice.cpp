// SendChoice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "SendChoice.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CSendChoice �Ի���

IMPLEMENT_DYNAMIC(CSendChoice, CDialogEx)

CSendChoice::CSendChoice(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSendChoice::IDD, pParent)
	, m_send_choice(0)
{

	m_breaktime = 0;
	m_packetLength = 0;
}

CSendChoice::~CSendChoice()
{
}

void CSendChoice::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_RADIO1, m_ration2);
	DDX_Radio(pDX, IDC_RADIO_SHORTMSG, m_send_choice);
	DDX_Text(pDX, IDC_EDIT_BREAKTIME, m_breaktime);
	DDX_Text(pDX, IDC_EDIT_PACKET_LENGTH, m_packetLength);
	DDV_MinMaxInt(pDX, m_packetLength, 1, 1500);
	DDX_Control(pDX, IDC_MAC_EDIT1, m_mac_edit1);
	DDX_Control(pDX, IDC_MAC_EDIT2, m_mac_edit2);
	DDX_Control(pDX, IDC_MAC_EDIT3, m_mac_edit3);
	DDX_Control(pDX, IDC_MAC_EDIT4, m_mac_edit4);
	DDX_Control(pDX, IDC_MAC_EDIT5, m_mac_edit5);
	DDX_Control(pDX, IDC_MAC_EDIT6, m_mac_edit6);
}


BEGIN_MESSAGE_MAP(CSendChoice, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSendChoice::OnBnClickedOk)
END_MESSAGE_MAP()


// CSendChoice ��Ϣ�������


BOOL CSendChoice::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_send_choice = 0;//���Ͷ���Ϣ
	m_mac_edit1.LimitText(2);//����MAC��ַ�ĵ��ַ���Ϊ2λ16����
	m_mac_edit2.LimitText(2);
	m_mac_edit3.LimitText(2);
	m_mac_edit4.LimitText(2);
	m_mac_edit5.LimitText(2);
	m_mac_edit6.LimitText(2);

	m_breaktime = theApp.m_breaktime; //���ݰ�����ʱ����
	m_packetLength = theApp.m_packetLength;//���ݰ�����������
	CString str_MAC;//MAC��ַ
	str_MAC.Format(_T("%02x"),theApp.c_send_des_macAddr[0]);
	m_mac_edit1.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_send_des_macAddr[1]);
	m_mac_edit2.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_send_des_macAddr[2]);
	m_mac_edit3.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_send_des_macAddr[3]);
	m_mac_edit4.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_send_des_macAddr[4]);
	m_mac_edit5.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_send_des_macAddr[5]);
	m_mac_edit6.SetWindowTextW(str_MAC);

	UpdateData(FALSE);//��������ֵˢ�µ��ؼ���
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSendChoice::OnBnClickedOk() 
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);//���ؼ���ֵ���µ�������
	//2014.4.3���õ��༭���ϵĿؼ���MAC��ַ
	CString  strSendText1,strSendText2,strSendText3,strSendText4,strSendText5,strSendText6;
	int i_Sendtext1,i_Sendtext2,i_Sendtext3,i_Sendtext4,i_Sendtext5,i_Sendtext6;
	m_mac_edit1.GetWindowText(strSendText1);   //�˴���Ҫ����ǿ��ת��
	m_mac_edit2.GetWindowText(strSendText2);
	m_mac_edit3.GetWindowText(strSendText3);
	m_mac_edit4.GetWindowText(strSendText4);
	m_mac_edit5.GetWindowText(strSendText5);
	m_mac_edit6.GetWindowText(strSendText6);
	//����������ȷ��
	if ((strSendText1 == "") || (strSendText2 == "") || (strSendText3 == "") || (strSendText4 == "") || (strSendText5 == "") || (strSendText6 == ""))
	{
		MessageBox(_T("�����MAC��ַ�����������������룡"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	i_Sendtext1 = strtol((CStringA)strSendText1,NULL,16);
	i_Sendtext2 = strtol((CStringA)strSendText2,NULL,16);
	i_Sendtext3 = strtol((CStringA)strSendText3,NULL,16);
	i_Sendtext4 = strtol((CStringA)strSendText4,NULL,16);
	i_Sendtext5 = strtol((CStringA)strSendText5,NULL,16);
	i_Sendtext6 = strtol((CStringA)strSendText6,NULL,16);
	theApp.c_send_des_macAddr[0] = i_Sendtext1;//����MAC��ַ
	theApp.c_send_des_macAddr[1] = i_Sendtext2;
	theApp.c_send_des_macAddr[2] = i_Sendtext3;
	theApp.c_send_des_macAddr[3] = i_Sendtext4;
	theApp.c_send_des_macAddr[4] = i_Sendtext5;
	theApp.c_send_des_macAddr[5] = i_Sendtext6;
	theApp.m_packetLength = m_packetLength;//���뷢�����ݰ�����
	theApp.m_breaktime = m_breaktime;//���뷢�����ݰ�ʱ����

	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_DES_MAC,NULL,NULL);  //������ܷ�����Ϣ������Ŀ��������MAC��ַ
	//��ÿ�������������ϸ��Ϣ��MAC��IP�ȵ�
	BOOL b_getAdapter = ((CMainFrame*)AfxGetMainWnd())->GetEnableAdapterInfo();
	if(b_getAdapter == false)
	{
		MessageBox(_T("δ�ҵ����õ������豸��"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	//2014.4.3
	switch (m_send_choice)
	{
	case -1: //δѡ��
		AfxMessageBox(_T("��ѡ�������ݵ����ͣ�"),MB_OK|MB_ICONWARNING);
		return;
		break;
	case 0: //���Ͷ���Ϣ
		{//2014.08.23 zsy
		    CDlgSendMsg dlg_sendMsg;
		    if(IDOK == dlg_sendMsg.DoModal())
			{
				//1.��ÿ�������������ϸ��Ϣ��MAC��IP�ȵ�
				BOOL b_getAdapter = ((CMainFrame*)AfxGetMainWnd())->GetEnableAdapterInfo();
				if(b_getAdapter == false)
				{
					AfxMessageBox(_T("δ�ҵ����õ������豸��"),MB_OK|MB_ICONWARNING);
					return;
				}
				SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SOURCE_MAC,NULL,NULL);
			}
		//2014.08.23 zsy
		    break;
		}
	case 1: //�����ļ�
		{
			CString filter;
			//filter = _T("All files(*.*)|*.*||"); //�ļ���ʽ
			filter = _T("TextFiles(*.txt)|*.txt|PcapDumpFiles(*.cap)|*.cap||");//�ļ���ʽ
			CFileDialog dlg_file(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
			if (dlg_file.DoModal() == IDOK)
			{
				FilePragram *filePargram = new FilePragram;
				//theApp.str_openfileName = dlg_file.GetPathName(); //��ȡ�������ļ�����c:\a.txt
				CString str_openfileEx = dlg_file.GetFileExt();//��ȡ�ļ���׺����.txt
				filePargram->fileName = dlg_file.GetPathName();//��ȡ�������ļ�����c:\a.txt
				if (str_openfileEx == _T("cap"))
				{
					filePargram->i_fileEx = 0; //�����ļ���׺��־
				}
				else
				{
					filePargram->i_fileEx = 1;
				}
				if (filePargram->fileName == _T(""))
				{
					MessageBox(_T("����ļ���ַʧ�ܣ�"),NULL,MB_OK|MB_ICONWARNING);
					return ;
				}
				SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_FILE,(WPARAM)filePargram,NULL);
				//delete filePargram; �˴������ͷ��ڴ棬���������,ȥ�߳����ͷ��ڴ�
			}
			break;
		}
	case 2: //����ͼ��
		{
			CString filter;
			filter = _T("All files(*.*)|*.*||"); //�ļ���ʽ
			//filter = _T("TextFiles(*.txt)|*.txt|PcapDumpFiles(*.cap)|*.cap||");//�ļ���ʽ
			CFileDialog dlg_file(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
			if (dlg_file.DoModal() == IDOK)
			{
				PictureParam *pictureParam = new PictureParam;
				//theApp.str_openfileName = dlg_file.GetPathName(); //��ȡ�������ļ�����c:\a.txt
				CString str_openfileEx = dlg_file.GetFileExt();//��ȡ�ļ���׺����.txt
				pictureParam->pictureName = dlg_file.GetPathName();//��ȡ�������ļ�����c:\a.txt
				
				if (pictureParam->pictureName == _T(""))
				{
					MessageBox(_T("����ļ���ַʧ�ܣ�"),NULL,MB_OK|MB_ICONWARNING);
					return ;
				}
				SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_PICTURE,(WPARAM)pictureParam,NULL);
				//delete filePargram; �˴������ͷ��ڴ棬���������,ȥ�߳����ͷ��ڴ�
			}
			break;
			break;
		}
	case 3: //����������.wav��ʽ��
		{
			CString filter;
			filter = _T("VoiceFiles(*.wav)|*.wav||");//�ļ���ʽ
			CFileDialog dlg_file(TRUE,_T(".wav"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
			if (dlg_file.DoModal() == IDOK)
			{
				VoiceParam *filePargram = new VoiceParam;
				//theApp.str_openfileName = dlg_file.GetPathName(); //��ȡ�������ļ�����c:\a.txt
				CString str_openfileEx = dlg_file.GetFileExt();//��ȡ�ļ���׺����.txt
				filePargram->fileName = dlg_file.GetPathName();//��ȡ�������ļ�����c:\a.txt
				if (filePargram->fileName == _T(""))
				{
					MessageBox(_T("����ļ���ַʧ�ܣ�"),NULL,MB_OK|MB_ICONWARNING);
					return ;
				}
				SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_VOICE,(WPARAM)filePargram,NULL);
				//delete filePargram; �˴������ͷ��ڴ棬��������⣬�߳����ͷ��ڴ�
			}
			break;
		}
	default:
		break;
	}
	CDialogEx::OnOK();
}
