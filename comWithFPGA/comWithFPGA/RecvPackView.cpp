// RecvPackView.cpp : 实现文件
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


// CRecvPackView 诊断

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


// CRecvPackView 消息处理程序


void CRecvPackView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//列表初始化
	long l_style = GetWindowLong(m_hWnd,GWL_STYLE); //获得当前窗口风格
	l_style &=~LVS_TYPEMASK;   //清除显示方式位
	l_style |= (LVS_REPORT|LVS_SHOWSELALWAYS); //设置风格：报表风格
	SetWindowLong(m_hWnd,GWL_STYLE,l_style); 

	DWORD dwStyle = GetListCtrl().GetExtendedStyle();//获得扩展风格
	dwStyle |= (LVS_EX_FULLROWSELECT/*|LVS_EX_GRIDLINES*/);
	GetListCtrl().SetExtendedStyle(dwStyle); //设置扩展风格

	GetListCtrl().SetTextColor(RGB(10,10,80)); //设置文本颜色
	CRect rect;
	GetListCtrl().GetClientRect(&rect);
	int nColInterval = rect.Width()/20;
	//插入列标题
	CString strItem;
	strItem.LoadString(IDS_PACKET_NUMBER); //序号
	GetListCtrl().InsertColumn(0,strItem,LVCFMT_CENTER,nColInterval);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_PACKET_SOURCE_ADDRESS); //源地址
	GetListCtrl().InsertColumn(1,strItem,LVCFMT_CENTER,nColInterval*4);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_PACKET_DEST_ADDRESS); //目的地址
	GetListCtrl().InsertColumn(2,strItem,LVCFMT_CENTER,nColInterval*4);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_PACKET_PRO_TYPE); //协议类型
	GetListCtrl().InsertColumn(3,strItem,LVCFMT_CENTER,nColInterval*2);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_DATA_LENGTH); //数据长度
	GetListCtrl().InsertColumn(4,strItem,LVCFMT_CENTER,nColInterval*2);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_DATA_TIME); //时间
	GetListCtrl().InsertColumn(5,strItem,LVCFMT_CENTER,nColInterval*2);
	strItem.ReleaseBuffer();

	strItem.LoadString(IDS_DATA_CONTENT); //数据内容
	GetListCtrl().InsertColumn(6,strItem,LVCFMT_CENTER,rect.Width()-13*nColInterval);
	strItem.ReleaseBuffer();
	//GetListCtrl().ShowWindow(SW_SHOWNORMAL);

	//iNumber_packet = 0;  //设置总接收到得数据包个数为0
	//theApp.allPacket.RemoveAll(); //清空所有数据包信息

	//GetListCtrl().InsertColumn(0,_T("项"),LVCFMT_LEFT,30);
	//GetListCtrl().InsertColumn(1,_T("属性值"),LVCFMT_LEFT,20);


}
LRESULT CRecvPackView::OnUpdateListView(WPARAM wParam,LPARAM lParam)
{   
	//列表框中显示捕获的以太网数据包
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
			GetListCtrl().InsertItem(&lv_Item);     //序号
			break;
		case 1:
			GetListCtrl().SetItemText(lv_Item.iItem,i,onePacket->str_source_address);   //源地址
			break;
		case 2:
			GetListCtrl().SetItemText(lv_Item.iItem,i,onePacket->str_des_address);   //目的地址
			break;
		case 3:
			/*
			switch(onePacket->type_length)
			{
			case 0x0800:
				GetListCtrl().SetItemText(lv_Item.iItem,i,_T("IP"));  //上层协议为IP协议
				break;
			case 0x0806:
				GetListCtrl().SetItemText(lv_Item.iItem,i,_T("ARP"));  //上层协议为ARP协议
				break;
			case 0x8035:
				GetListCtrl().SetItemText(lv_Item.iItem,i,_T("RARP"));  //上层协议为RARP协议
			default:
				break;
			}*/
			if (onePacket->type_length < 0x0600)
                GetListCtrl().SetItemText(lv_Item.iItem,i,_T("Ethernet")); //无上层协议，只有数据长度
			break;
		case 4:
			str_Text.Format(_T("%d"),onePacket->type_length); //数据长度
			GetListCtrl().SetItemText(lv_Item.iItem,i,str_Text);
			break;
		case 5:
			str_Text.Format(_T("%s"),(CStringW)onePacket->str_time);
			GetListCtrl().SetItemText(lv_Item.iItem,i,str_Text);  //时间
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
				//MultiByteToWideChar(CP_ACP,0,(LPCSTR)onePacket->c_date,-1,(LPWSTR)(LPCTSTR)str_Text,len2); 此处不能直接使用(LPWSTR)(LPCTSTR)str_Text，否则提示堆内存泄露
				MultiByteToWideChar(CP_ACP,0,(LPCSTR)onePacket->c_date,-1,pwText,len2);
				
				GetListCtrl().SetItemText(lv_Item.iItem,i,pwText);  //数据内容
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//SendMessage(WM_UPDATE_PACKET_CONTENT,NULL,NULL);
	int row; //行号
	//int colunm; //列号
	LVHITTESTINFO lvhti;
	lvhti.pt = point ;
	row = GetListCtrl().SubItemHitTest(&lvhti);   //得到行号，从0开始
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
