// RecvPackView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "RecvPackView.h"


// CRecvPackView

IMPLEMENT_DYNCREATE(CRecvPackView, CListView)

CRecvPackView::CRecvPackView()
{
}

CRecvPackView::~CRecvPackView()
{
}

BEGIN_MESSAGE_MAP(CRecvPackView, CListView)
        ON_MESSAGE(WM_UPDATE_LIST_VIEW,OnUpdateListView)
		ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CRecvPackView ���

#ifdef _DEBUG
void CRecvPackView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CRecvPackView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CRecvPackView ��Ϣ�������


void CRecvPackView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//�б��ʼ��
	long l_style = GetWindowLong(m_hWnd,GWL_STYLE); //��õ�ǰ���ڷ��
	l_style &=~LVS_TYPEMASK;   //�����ʾ��ʽλ
	l_style |= (LVS_REPORT|LVS_SHOWSELALWAYS); //���÷�񣺱�����
	SetWindowLong(m_hWnd,GWL_STYLE,l_style); 

	DWORD dwStyle = GetListCtrl().GetExtendedStyle();//�����չ���
	dwStyle |= (LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/);
	GetListCtrl().SetExtendedStyle(dwStyle); //������չ���

	GetListCtrl().SetTextColor(RGB(10,10,80)); //�����ı���ɫ
	CRect rect;
	GetListCtrl().GetClientRect(&rect);
	int nColInterval = rect.Width()/20;
	//�����б���
	CString strItem;
	strItem.LoadString(IDS_PACKET_NUMBER); //���
	GetListCtrl().InsertColumn(0,strItem,LVCFMT_CENTER,nColInterval);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_PACKET_SOURCE_ADDRESS); //Դ��ַ
	GetListCtrl().InsertColumn(1,strItem,LVCFMT_CENTER,nColInterval*4);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_PACKET_DEST_ADDRESS); //Ŀ�ĵ�ַ
	GetListCtrl().InsertColumn(2,strItem,LVCFMT_CENTER,nColInterval*4);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_PACKET_PRO_TYPE); //Э������
	GetListCtrl().InsertColumn(3,strItem,LVCFMT_CENTER,nColInterval*2);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_DATA_LENGTH); //���ݳ���
	GetListCtrl().InsertColumn(4,strItem,LVCFMT_CENTER,nColInterval*2);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_DATA_TIME); //ʱ��
	GetListCtrl().InsertColumn(5,strItem,LVCFMT_CENTER,nColInterval*2);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_DATA_CONTENT); //��������
	GetListCtrl().InsertColumn(6,strItem,LVCFMT_CENTER,rect.Width()-13*nColInterval);
	strItem.ReleaseBuffer();
	//GetListCtrl().ShowWindow(SW_SHOWNORMAL);

	//iNumber_packet = 0;  //�����ܽ��յ������ݰ�����Ϊ0
	//theApp.allPacket.RemoveAll(); //����������ݰ���Ϣ

	//GetListCtrl().InsertColumn(0,_T("��"),LVCFMT_LEFT,30);
	//GetListCtrl().InsertColumn(1,_T("����ֵ"),LVCFMT_LEFT,20);


}
LRESULT CRecvPackView::OnUpdateListView(WPARAM wParam,LPARAM lParam)
{   
	//�б������ʾ�������̫�����ݰ�
	Packet_Info *onePacket = (Packet_Info*)wParam;
	CString str_Text,str_Text1;
	LV_ITEM lv_Item;
	lv_Item.mask = LVIF_TEXT;
	lv_Item.iItem = theApp.m_Number_packet;
	lv_Item.iSubItem = 0;
	for (int i = 0; i < COLUMN_NUMBER; i++)
	{
		switch(i)
		{
		case 0:
			str_Text.Format(_T("%d"),lv_Item.iItem+1);
			lv_Item.pszText = (LPWSTR)(LPCTSTR)str_Text;
			GetListCtrl().InsertItem(&lv_Item);     //���
			break;
		case 1:
			GetListCtrl().SetItemText(lv_Item.iItem,i,onePacket->str_source_address);   //Դ��ַ
			break;
		case 2:
			GetListCtrl().SetItemText(lv_Item.iItem,i,onePacket->str_des_address);   //Ŀ�ĵ�ַ
			break;
		case 3:
			/*
			switch(onePacket->type_length)
			{
			case 0x0800:
				GetListCtrl().SetItemText(lv_Item.iItem,i,_T("IP"));  //�ϲ�Э��ΪIPЭ��
				break;
			case 0x0806:
				GetListCtrl().SetItemText(lv_Item.iItem,i,_T("ARP"));  //�ϲ�Э��ΪARPЭ��
				break;
			case 0x8035:
				GetListCtrl().SetItemText(lv_Item.iItem,i,_T("RARP"));  //�ϲ�Э��ΪRARPЭ��
			default:
				break;
			}*/
			if (onePacket->type_length < 0x0600)
                GetListCtrl().SetItemText(lv_Item.iItem,i,_T("Ethernet")); //���ϲ�Э�飬ֻ�����ݳ���
			break;
		case 4:
			str_Text.Format(_T("%d"),onePacket->type_length); //���ݳ���
			GetListCtrl().SetItemText(lv_Item.iItem,i,str_Text);
			break;
		case 5:
			str_Text.Format(_T("%s"),(CStringW)onePacket->str_time);
			GetListCtrl().SetItemText(lv_Item.iItem,i,str_Text);  //ʱ��
			break;
		case 6:
			{
				//str_Text.Format(_T("%s"),_T(""));
				
				int len2 = MultiByteToWideChar(CP_ACP,0,(LPCSTR)onePacket->c_date,-1,NULL,0);

				wchar_t *pwText;
				pwText = new wchar_t[len2];
				if(!pwText)
				{
					delete []pwText;
				}
				//MultiByteToWideChar(CP_ACP,0,(LPCSTR)onePacket->c_date,-1,(LPWSTR)(LPCTSTR)str_Text,len2); �˴�����ֱ��ʹ��(LPWSTR)(LPCTSTR)str_Text��������ʾ���ڴ�й¶
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)onePacket->c_date,-1,pwText,len2);
				
				GetListCtrl().SetItemText(lv_Item.iItem,i,pwText);  //��������
				delete []pwText;
				break;
			}
		default:
			break;
		}
	}
	theApp.m_Number_packet++;
	return 0;
}

void CRecvPackView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//SendMessage(WM_UPDATE_PACKET_CONTENT,NULL,NULL);
	int row; //�к�
	//int colunm; //�к�
	LVHITTESTINFO lvhti;
	lvhti.pt = point ;
	row = GetListCtrl().SubItemHitTest(&lvhti);   //�õ��кţ���0��ʼ
	//colunm = lvhti.iSubItem ;
	if (row == -1)
	{
		return ;
	}
	TreeViewItemInfo *itemInfo = new TreeViewItemInfo;
	itemInfo->row = row;
	itemInfo->strData = GetListCtrl().GetItemText(row,6);

	::SendMessageA(theApp.pMyTreeView->m_hWnd,WM_UPDATE_PACKET_CONTENT,(WPARAM)itemInfo,NULL);
	if (!itemInfo)
	{
		delete itemInfo;
	}
	CListView::OnLButtonDblClk(nFlags, point);
}
