// DlgSavedata.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "DlgSavedata.h"
#include "afxdialogex.h"


// CDlgSavedata �Ի���

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


// CDlgSavedata ��Ϣ�������


void CDlgSavedata::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_save_edit.EnableWindow(true); //ʹ�༭��ؼ��ɱ༭
	m_fileformat_comb.EnableWindow(true);//ʹ��Ͽ�ؼ���ѡ��
}


void CDlgSavedata::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//2014.3.27���õ��༭���ϵĿؼ���MAC��ַ
	CString  strText1,strText2,strText3,strText4,strText5,strText6;
	int i_text1,i_text2,i_text3,i_text4,i_text5,i_text6;
	m_des_mac1.GetWindowText(strText1);   
	m_des_mac2.GetWindowText(strText2);
	m_des_mac3.GetWindowText(strText3);
	m_des_mac4.GetWindowText(strText4);
	m_des_mac5.GetWindowText(strText5);
	m_des_mac6.GetWindowText(strText6);
	//����������ȷ��
	if ((strText1 == "") || (strText2 == "") || (strText3 == "") || (strText4 == "") || (strText5 == "") || (strText6 == "") //�ͣ��õ�ַÿһ���Ϊ��
		|| (strText1.GetLength() != 2) || (strText2.GetLength() != 2) || (strText3.GetLength() != 2) || (strText4.GetLength() != 2)
		|| (strText5.GetLength() != 2) || (strText2.GetLength() != 2))//MAC��ַÿһ�����������λ16����)
	{
		MessageBox(_T("�����MAC��ַ�����������������룡"),NULL,MB_OK|MB_ICONWARNING);
		return;
	}
	i_text1 = strtol((CStringA)strText1,NULL,16);//�˴���Ҫ����ǿ��ת��
	i_text2 = strtol((CStringA)strText2,NULL,16);
	i_text3 = strtol((CStringA)strText3,NULL,16);
	i_text4 = strtol((CStringA)strText4,NULL,16);
	i_text5 = strtol((CStringA)strText5,NULL,16);
	i_text6 = strtol((CStringA)strText6,NULL,16);
	if (BST_CHECKED == m_ration1.GetCheck()) //����ֵBST_CHECKErDΪѡ��״̬,����ֵBST_UNCHECKEDΪδѡ��״̬
	{
		CString  fileName,fileFormat,newfile;
		theApp.i_radio = 0 ; //ѡ���ˣ���������
		theApp.i_combox = m_fileformat_comb.GetCurSel();
		m_save_edit.GetWindowTextW(fileName);  //�õ�������ļ���
		m_fileformat_comb.GetWindowTextW(fileFormat); //�õ�ѡ����ļ���ʽ
		if (fileName == _T(""))
		{
			AfxMessageBox(_T("��������Ҫ�������ݵ��ļ���!"),MB_OK|MB_ICONWARNING);
			return;
		}
		newfile.Format(_T("%s%s"),fileName,fileFormat); //�ļ������ļ���ʽ
		if(false == CreatNewFile(newfile)) //�����ļ�
		{
			//MessageBox(_T("�������ݱ����ļ�ʧ�ܣ�"),NULL,MB_OK|MB_ICONWARNING);
			return;
		}
	}
	if (BST_CHECKED == m_isClear.GetCheck()) //ѡ�������¿�ʼ�������ݣ��˺󽫽�����ղ���
	{
		theApp.allPacket.RemoveAll();//��ս������ݰ��洢��
		theApp.m_Number_packet = 0;//��ս������ݰ���Ŀ
	}
	theApp.c_des_macAddr[0] = i_text1;
	theApp.c_des_macAddr[1] = i_text2;
	theApp.c_des_macAddr[2] = i_text3;
	theApp.c_des_macAddr[3] = i_text4;
	theApp.c_des_macAddr[4] = i_text5;
	theApp.c_des_macAddr[5] = i_text6;
	//theApp.isDesMac = true ;//�Ѿ�����ֶ������Ŀ������MAC��ַ����־��Ϊ��
	::SendMessageA(AfxGetMainWnd()->m_hWnd,WM_UPDATE_DES_MAC,NULL,NULL);  //������ܷ�����Ϣ������Ŀ��������MAC��ַ
	CDialogEx::OnOK();
}


BOOL CDlgSavedata::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ration1.SetCheck(BST_CHECKED);
	m_des_mac1.LimitText(2);//����MAC��ַ�ĵ��ַ���Ϊ2λ16����
	m_des_mac2.LimitText(2);
	m_des_mac3.LimitText(2);
	m_des_mac4.LimitText(2);
	m_des_mac5.LimitText(2);
	m_des_mac6.LimitText(2);

	CString str_MAC;//MAC��ַ
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

	//2014.8.30 ������Ͽ����ļ���ʽ
	m_fileformat_comb.InsertString(0,_T(".cap"));
	m_fileformat_comb.InsertString(1,_T(".txt"));
	m_fileformat_comb.InsertString(2,_T(".wav"));//2014.9.1 wav�����ļ�
	m_fileformat_comb.SetCurSel(0);//����.cap�ļ�ΪĬ��ѡ����
	UpdateData(FALSE);//��������ֵˢ�µ��ؼ���
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


// //�����µ����ݱ����ļ�
bool CDlgSavedata::CreatNewFile(CString fileName)
{
	//theApp.str_fileName.Format(_T("%s%s"),_T("SaveData\\"),fileName);
	//theApp.str_fileName = fileName;
	CString newfileName;
	CString directrory;
	CFile  file;
	CFileException exception;//���������Ϣ
	directrory = _T("SaveData");
	if (! (GetFileAttributes(directrory) == FILE_ATTRIBUTE_DIRECTORY))  //���ж��Ƿ�����ļ��У�������ǣ��򴴽��ļ���
	{
		if(! CreateDirectory(directrory,NULL))
		{
			MessageBox(_T("����SaveData�ļ���ʧ�ܣ�"),NULL,MB_OK|MB_ICONWARNING);
			return false;
		}
	}
	newfileName.Format(_T("%s\\%s"),directrory,fileName);
	if (! file.Open(newfileName,CFile::modeCreate | CFile::modeReadWrite, &exception)) //�����ļ�
	{
	   MessageBox(_T("���������ļ�ʧ�ܣ�"),NULL,MB_OK|MB_ICONWARNING);
	   return false;
	}
	theApp.str_fileName = file.GetFilePath();
	return true;
}

void CDlgSavedata::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_save_edit.EnableWindow(false); //ʹ�༭��ؼ����ɱ༭
	m_fileformat_comb.EnableWindow(false);//ʹ��Ͽ�ؼ�����ѡ��
}



void CDlgSavedata::OnCbnSelchangeCombo1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
