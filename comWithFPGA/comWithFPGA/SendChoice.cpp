// SendChoice.cpp : 实现文件
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "SendChoice.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CSendChoice 对话框

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


// CSendChoice 消息处理程序


BOOL CSendChoice::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_send_choice = 0;//发送短消息
	m_mac_edit1.LimitText(2);//设置MAC地址的的字符数为2位16进制
	m_mac_edit2.LimitText(2);
	m_mac_edit3.LimitText(2);
	m_mac_edit4.LimitText(2);
	m_mac_edit5.LimitText(2);
	m_mac_edit6.LimitText(2);

	m_breaktime = theApp.m_breaktime; //数据包发送时间间隔
	m_packetLength = theApp.m_packetLength;//数据包数据区长度
	CString str_MAC;//MAC地址
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

	UpdateData(FALSE);//将变量的值刷新到控件中
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSendChoice::OnBnClickedOk() 
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);//将控件的值更新到变量中
	//2014.4.3：得到编辑框上的控件的MAC地址
	CString  strSendText1,strSendText2,strSendText3,strSendText4,strSendText5,strSendText6;
	int i_Sendtext1,i_Sendtext2,i_Sendtext3,i_Sendtext4,i_Sendtext5,i_Sendtext6;
	m_mac_edit1.GetWindowText(strSendText1);   //此处需要类型强制转化
	m_mac_edit2.GetWindowText(strSendText2);
	m_mac_edit3.GetWindowText(strSendText3);
	m_mac_edit4.GetWindowText(strSendText4);
	m_mac_edit5.GetWindowText(strSendText5);
	m_mac_edit6.GetWindowText(strSendText6);
	//检查输入的正确性
	if ((strSendText1 == "") || (strSendText2 == "") || (strSendText3 == "") || (strSendText4 == "") || (strSendText5 == "") || (strSendText6 == ""))
	{
		MessageBox(_T("输入的MAC地址不完整，请重新输入！"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	i_Sendtext1 = strtol((CStringA)strSendText1,NULL,16);
	i_Sendtext2 = strtol((CStringA)strSendText2,NULL,16);
	i_Sendtext3 = strtol((CStringA)strSendText3,NULL,16);
	i_Sendtext4 = strtol((CStringA)strSendText4,NULL,16);
	i_Sendtext5 = strtol((CStringA)strSendText5,NULL,16);
	i_Sendtext6 = strtol((CStringA)strSendText6,NULL,16);
	theApp.c_send_des_macAddr[0] = i_Sendtext1;//存入MAC地址
	theApp.c_send_des_macAddr[1] = i_Sendtext2;
	theApp.c_send_des_macAddr[2] = i_Sendtext3;
	theApp.c_send_des_macAddr[3] = i_Sendtext4;
	theApp.c_send_des_macAddr[4] = i_Sendtext5;
	theApp.c_send_des_macAddr[5] = i_Sendtext6;
	theApp.m_packetLength = m_packetLength;//存入发送数据包长度
	theApp.m_breaktime = m_breaktime;//存入发送数据包时间间隔

	::PostMessage(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_DES_MAC,NULL,NULL);  //向主框架发送消息，更新目的主机的MAC地址
	//获得可用适配器的详细信息：MAC，IP等等
	BOOL b_getAdapter = ((CMainFrame*)AfxGetMainWnd())->GetEnableAdapterInfo();
	if(b_getAdapter == false)
	{
		MessageBox(_T("未找到可用的网络设备！"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	//2014.4.3
	switch (m_send_choice)
	{
	case -1: //未选中
		AfxMessageBox(_T("请选择发送数据的类型！"),MB_OK|MB_ICONWARNING);
		return;
		break;
	case 0: //发送短消息
		{//2014.08.23 zsy
		    CDlgSendMsg dlg_sendMsg;
		    if(IDOK == dlg_sendMsg.DoModal())
			{
				//1.获得可用适配器的详细信息：MAC，IP等等
				BOOL b_getAdapter = ((CMainFrame*)AfxGetMainWnd())->GetEnableAdapterInfo();
				if(b_getAdapter == false)
				{
					AfxMessageBox(_T("未找到可用的网络设备！"),MB_OK|MB_ICONWARNING);
					return;
				}
				SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SOURCE_MAC,NULL,NULL);
			}
		//2014.08.23 zsy
		    break;
		}
	case 1: //发送文件
		{
			CString filter;
			//filter = _T("All files(*.*)|*.*||"); //文件格式
			filter = _T("TextFiles(*.txt)|*.txt|PcapDumpFiles(*.cap)|*.cap||");//文件格式
			CFileDialog dlg_file(TRUE,_T(".txt"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
			if (dlg_file.DoModal() == IDOK)
			{
				FilePragram *filePargram = new FilePragram;
				//theApp.str_openfileName = dlg_file.GetPathName(); //获取完整的文件：如c:\a.txt
				CString str_openfileEx = dlg_file.GetFileExt();//获取文件后缀。如.txt
				filePargram->fileName = dlg_file.GetPathName();//获取完整的文件：如c:\a.txt
				if (str_openfileEx == _T("cap"))
				{
					filePargram->i_fileEx = 0; //设置文件后缀标志
				}
				else
				{
					filePargram->i_fileEx = 1;
				}
				if (filePargram->fileName == _T(""))
				{
					MessageBox(_T("获得文件地址失败！"),NULL,MB_OK|MB_ICONWARNING);
					return ;
				}
				SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_FILE,(WPARAM)filePargram,NULL);
				//delete filePargram; 此处不能释放内存，否则出问题,去线程里释放内存
			}
			break;
		}
	case 2: //发送图像
		{
			CString filter;
			filter = _T("All files(*.*)|*.*||"); //文件格式
			//filter = _T("TextFiles(*.txt)|*.txt|PcapDumpFiles(*.cap)|*.cap||");//文件格式
			CFileDialog dlg_file(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
			if (dlg_file.DoModal() == IDOK)
			{
				PictureParam *pictureParam = new PictureParam;
				//theApp.str_openfileName = dlg_file.GetPathName(); //获取完整的文件：如c:\a.txt
				CString str_openfileEx = dlg_file.GetFileExt();//获取文件后缀。如.txt
				pictureParam->pictureName = dlg_file.GetPathName();//获取完整的文件：如c:\a.txt
				
				if (pictureParam->pictureName == _T(""))
				{
					MessageBox(_T("获得文件地址失败！"),NULL,MB_OK|MB_ICONWARNING);
					return ;
				}
				SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_PICTURE,(WPARAM)pictureParam,NULL);
				//delete filePargram; 此处不能释放内存，否则出问题,去线程里释放内存
			}
			break;
			break;
		}
	case 3: //发送语音（.wav格式）
		{
			CString filter;
			filter = _T("VoiceFiles(*.wav)|*.wav||");//文件格式
			CFileDialog dlg_file(TRUE,_T(".wav"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,filter);
			if (dlg_file.DoModal() == IDOK)
			{
				VoiceParam *filePargram = new VoiceParam;
				//theApp.str_openfileName = dlg_file.GetPathName(); //获取完整的文件：如c:\a.txt
				CString str_openfileEx = dlg_file.GetFileExt();//获取文件后缀。如.txt
				filePargram->fileName = dlg_file.GetPathName();//获取完整的文件：如c:\a.txt
				if (filePargram->fileName == _T(""))
				{
					MessageBox(_T("获得文件地址失败！"),NULL,MB_OK|MB_ICONWARNING);
					return ;
				}
				SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_SEND_VOICE,(WPARAM)filePargram,NULL);
				//delete filePargram; 此处不能释放内存，否则出问题，线程里释放内存
			}
			break;
		}
	default:
		break;
	}
	CDialogEx::OnOK();
}
