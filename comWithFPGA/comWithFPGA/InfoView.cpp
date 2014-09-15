// InfoView.cpp : 实现文件
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


// CInfoView 诊断

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


// CInfoView 消息处理程序
LRESULT CInfoView::OnUpdatePacketContent(WPARAM wp,LPARAM lp)
{   
	GetTreeCtrl().DeleteAllItems(); //清除所有显示
    TreeViewItemInfo *pItemInfo = (TreeViewItemInfo*)wp;
	int iRow = pItemInfo->row;
	CString strData = pItemInfo->strData;
	delete pItemInfo;

	//添加内容显示
	CString iTemName;
	iTemName.LoadString(IDS_DATA_CONTENT);
	HTREEITEM hItem,hsubItem;
	hItem = GetTreeCtrl().InsertItem(iTemName,TVI_ROOT); //根节点“数据内容”
	hsubItem = GetTreeCtrl().InsertItem(strData,hItem); //子节点，显示具体数据内容
	GetTreeCtrl().Expand(hItem,TVE_EXPAND); //根节点展开
	return 0;
}

void CInfoView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	//初始化树视图
	GetTreeCtrl().ModifyStyle(NULL,TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS); //设置显示风格
	                         //TVS_HASBUTTONS：显示带有＂+＂或＂-＂的小方框来表示某项能否被展开或已展开
	                         //TVS_HASLINES：在父节点与子节点间连线以更清晰地显示树的结构
	                         //TVS_SHOWSELALWAYS：即使控件失去输入焦点，仍显示出项的选择状态
	                         //TVS_LINESATROOT：在根节点处连线
	                         //TVS_EDITLABELS：用户可以编辑节点的标签文本
	                         //TVS_DISABLEDRAGDROP：禁止树形控件发送TVN_BEGINDRAG通知消息，即不支持拖动操作
	
}                           

