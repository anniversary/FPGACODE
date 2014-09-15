// InfoView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "InfoView.h"

// CInfoView

IMPLEMENT_DYNCREATE(CInfoView, CTreeView)

CInfoView::CInfoView()
{

}

CInfoView::~CInfoView()
{
}

BEGIN_MESSAGE_MAP(CInfoView, CTreeView)
	ON_MESSAGE(WM_UPDATE_PACKET_CONTENT,&CInfoView::OnUpdatePacketContent)
END_MESSAGE_MAP()


// CInfoView ���

#ifdef _DEBUG
void CInfoView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CInfoView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CInfoView ��Ϣ�������
LRESULT CInfoView::OnUpdatePacketContent(WPARAM wp,LPARAM lp)
{   
	GetTreeCtrl().DeleteAllItems(); //���������ʾ
    TreeViewItemInfo *pItemInfo = (TreeViewItemInfo*)wp;
	int iRow = pItemInfo->row;
	CString strData = pItemInfo->strData;
	delete pItemInfo;

	//���������ʾ
	CString iTemName;
	iTemName.LoadString(IDS_DATA_CONTENT);
	HTREEITEM hItem,hsubItem;
	hItem = GetTreeCtrl().InsertItem(iTemName,TVI_ROOT); //���ڵ㡰�������ݡ�
	hsubItem = GetTreeCtrl().InsertItem(strData,hItem); //�ӽڵ㣬��ʾ������������
	GetTreeCtrl().Expand(hItem,TVE_EXPAND); //���ڵ�չ��
	return 0;
}

void CInfoView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	//��ʼ������ͼ
	GetTreeCtrl().ModifyStyle(NULL,TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS); //������ʾ���
	                         //TVS_HASBUTTONS����ʾ���У�+����-����С��������ʾĳ���ܷ�չ������չ��
	                         //TVS_HASLINES���ڸ��ڵ����ӽڵ�������Ը���������ʾ���Ľṹ
	                         //TVS_SHOWSELALWAYS����ʹ�ؼ�ʧȥ���뽹�㣬����ʾ�����ѡ��״̬
	                         //TVS_LINESATROOT���ڸ��ڵ㴦����
	                         //TVS_EDITLABELS���û����Ա༭�ڵ�ı�ǩ�ı�
	                         //TVS_DISABLEDRAGDROP����ֹ���οؼ�����TVN_BEGINDRAG֪ͨ��Ϣ������֧���϶�����
	
}                           

