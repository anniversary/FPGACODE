// DlgSavedata.cpp : 实现文件
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "DlgSavedata.h"
#include "afxdialogex.h"


// CDlgSavedata 对话框

IMPLEMENT_DYNAMIC(CDlgSavedata, CDialogEx)

CDlgSavedata::CDlgSavedata(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgSavedata::IDD, pParent)
{
}

CDlgSavedata::~CDlgSavedata()
{
}

void CDlgSavedata::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_SAVE, m_ration1);
	DDX_Control(pDX, IDC_EDIT_FILENAME, m_save_edit);
	DDX_Control(pDX, IDC_MAC_EDIT1, m_des_mac1);
	DDX_Control(pDX, IDC_MAC_EDIT2, m_des_mac2);
	DDX_Control(pDX, IDC_MAC_EDIT3, m_des_mac3);
	DDX_Control(pDX, IDC_MAC_EDIT4, m_des_mac4);
	DDX_Control(pDX, IDC_MAC_EDIT5, m_des_mac5);
	DDX_Control(pDX, IDC_MAC_EDIT6, m_des_mac6);
	DDX_Control(pDX, IDC_CHECK_CLEAR, m_isClear);
	DDX_Control(pDX, IDC_COMBOX_FILE_FORMAT, m_fileformat_comb);
}


BEGIN_MESSAGE_MAP(CDlgSavedata, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_SAVE, &CDlgSavedata::OnBnClickedRadio1)
	ON_BN_CLICKED(IDOK, &CDlgSavedata::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO_NOTSAVE, &CDlgSavedata::OnBnClickedRadio2)
//	ON_CBN_SELCHANGE(IDC_COMBO_WORKMOD, &CDlgSavedata::OnCbnSelchangeComboWorkmod)
//	ON_BN_CLICKED(IDC_CHECK1, &CDlgSavedata::OnBnClickedCheck1)
//ON_BN_CLICKED(IDC_CHECK1, &CDlgSavedata::OnBnClickedCheck1)
//ON_BN_CLICKED(IDCANCEL, &CDlgSavedata::OnBnClickedCancel)
//ON_EN_MAXTEXT(IDC_MAC_EDIT1, &CDlgSavedata::OnMaxtextMacEdit1)
ON_CBN_SELCHANGE(IDC_COMBOX_FILE_FORMAT, &CDlgSavedata::OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


// CDlgSavedata 消息处理程序


void CDlgSavedata::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_save_edit.EnableWindow(true); //使编辑框控件可编辑
	m_fileformat_comb.EnableWindow(true);//使组合框控件可选择
}


void CDlgSavedata::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//2014.3.27：得到编辑框上的控件的MAC地址
	CString  strText1,strText2,strText3,strText4,strText5,strText6;
	int i_text1,i_text2,i_text3,i_text4,i_text5,i_text6;
	m_des_mac1.GetWindowText(strText1);   
	m_des_mac2.GetWindowText(strText2);
	m_des_mac3.GetWindowText(strText3);
	m_des_mac4.GetWindowText(strText4);
	m_des_mac5.GetWindowText(strText5);
	m_des_mac6.GetWindowText(strText6);
	//检查输入的正确性
	if ((strText1 == "") || (strText2 == "") || (strText3 == "") || (strText4 == "") || (strText5 == "") || (strText6 == "") //ＭＡＣ地址每一项不能为空
		|| (strText1.GetLength() != 2) || (strText2.GetLength() != 2) || (strText3.GetLength() != 2) || (strText4.GetLength() != 2)
		|| (strText5.GetLength() != 2) || (strText2.GetLength() != 2))//MAC地址每一项必须输入两位16进制)
	{
		MessageBox(_T("输入的MAC地址不完整，请重新输入！"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	i_text1 = strtol((CStringA)strText1,NULL,16);//此处需要类型强制转化
	i_text2 = strtol((CStringA)strText2,NULL,16);
	i_text3 = strtol((CStringA)strText3,NULL,16);
	i_text4 = strtol((CStringA)strText4,NULL,16);
	i_text5 = strtol((CStringA)strText5,NULL,16);
	i_text6 = strtol((CStringA)strText6,NULL,16);
	if (BST_CHECKED == m_ration1.GetCheck()) //返回值BST_CHECKErD为选中状态,返回值BST_UNCHECKED为未选中状态
	{
		CString  fileName,fileFormat,newfile;
		theApp.i_radio = 0 ; //选中了：保存数据
		theApp.i_combox = m_fileformat_comb.GetCurSel();
		m_save_edit.GetWindowTextW(fileName);  //得到保存的文件名
		m_fileformat_comb.GetWindowTextW(fileFormat); //得到选择的文件格式
		if (fileName == _T(""))
		{
			AfxMessageBox(_T("请输入需要保存数据的文件名!"),MB_OK|MB_ICONWARNING);
			return;
		}
		newfile.Format(_T("%s%s"),fileName,fileFormat); //文件名加文件格式
		if(false == CreatNewFile(newfile)) //创建文件
		{
			//MessageBox(_T("创建数据保存文件失败！"),NULL,MB_OK|MB_ICONWARNING);
			return;
		}
	}
	if (BST_CHECKED == m_isClear.GetCheck()) //选择了重新开始接收数据，此后将进行清空操作
	{
		theApp.allPacket.RemoveAll();//清空接收数据包存储项
		theApp.m_Number_packet = 0;//清空接收数据包数目
	}
	theApp.c_des_macAddr[0] = i_text1;
	theApp.c_des_macAddr[1] = i_text2;
	theApp.c_des_macAddr[2] = i_text3;
	theApp.c_des_macAddr[3] = i_text4;
	theApp.c_des_macAddr[4] = i_text5;
	theApp.c_des_macAddr[5] = i_text6;
	//theApp.isDesMac = true ;//已经获得手动收入的目的主机MAC地址，标志置为真
	::SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_DES_MAC,NULL,NULL);  //向主框架发送消息，更新目的主机的MAC地址
	CDialogEx::OnOK();
}


BOOL CDlgSavedata::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ration1.SetCheck(BST_CHECKED);
	m_des_mac1.LimitText(2);//设置MAC地址的的字符数为2位16进制
	m_des_mac2.LimitText(2);
	m_des_mac3.LimitText(2);
	m_des_mac4.LimitText(2);
	m_des_mac5.LimitText(2);
	m_des_mac6.LimitText(2);

	CString str_MAC;//MAC地址
	str_MAC.Format(_T("%02x"),theApp.c_des_macAddr[0]);
	m_des_mac1.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_des_macAddr[1]);
	m_des_mac2.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_des_macAddr[2]);
	m_des_mac3.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_des_macAddr[3]);
	m_des_mac4.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_des_macAddr[4]);
	m_des_mac5.SetWindowTextW(str_MAC);
	str_MAC.Format(_T("%02x"),theApp.c_des_macAddr[5]);
	m_des_mac6.SetWindowTextW(str_MAC);

	//2014.8.30 设置组合框中文件格式
	m_fileformat_comb.InsertString(0,_T(".cap"));
	m_fileformat_comb.InsertString(1,_T(".txt"));
	m_fileformat_comb.InsertString(2,_T(".wav"));//2014.9.1 wav声音文件
	m_fileformat_comb.SetCurSel(0);//设置.cap文件为默认选中项
	UpdateData(FALSE);//将变量的值刷新到控件中
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// //创建新的数据保存文件
bool CDlgSavedata::CreatNewFile(CString fileName)
{
	//theApp.str_fileName.Format(_T("%s%s"),_T("SaveData\\"),fileName);
	//theApp.str_fileName = fileName;
	CString newfileName;
	CString directrory;
	CFile  file;
	CFileException exception;//保存错误信息
	directrory = _T("SaveData");
	if (! (GetFileAttributes(directrory) == FILE_ATTRIBUTE_DIRECTORY))  //先判断是否存在文件夹，如果不是，则创建文件夹
	{
		if(! CreateDirectory(directrory,NULL))
		{
			MessageBox(_T("创建SaveData文件夹失败！"),NULL,MB_OK|MB_ICONWARNING);
			return false;
		}
	}
	newfileName.Format(_T("%s\\%s"),directrory,fileName);
	if (! file.Open(newfileName,CFile::modeCreate | CFile::modeReadWrite, &exception)) //创建文件
	{
	   MessageBox(_T("创建保存文件失败！"),NULL,MB_OK|MB_ICONWARNING);
	   return false;
	}
	theApp.str_fileName = file.GetFilePath();
	return true;
}

void CDlgSavedata::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_save_edit.EnableWindow(false); //使编辑框控件不可编辑
	m_fileformat_comb.EnableWindow(false);//使组合框控件不可选择
}



void CDlgSavedata::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
}
