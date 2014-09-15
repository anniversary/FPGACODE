
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "comWithFPGA.h"

#include "MainFrm.h"

//2014.3.25
//#include <afxcview.h>
#include "InfoView.h"
#include "RecvPackView.h"
#include "constdef.h"
#include "RecvPackView.h"
#include "SendChoice.h"
//2014.3.25
#include "Mmsystem.h" //读取声音文件的库文件

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND(ID_START_RECV, &CMainFrame::OnStartRecv)
	ON_MESSAGE(WM_UPDATE_SOURCE_MAC,&CMainFrame::OnUpdateSourMac)
	ON_MESSAGE(WM_UPDATE_DES_MAC,&CMainFrame::OnUpdateDesMac)
	ON_MESSAGE(WM_UPDATE_SEND_DES_MAC,&CMainFrame::OnUpdateSendDesMac)
	ON_MESSAGE(WM_UPDATE_SEND_FILE,&CMainFrame::OnUpdateSendFile)
	ON_MESSAGE(WM_UPDATE_SEND_MSG,&CMainFrame::OnUpdateSendMSG) //2014.08.23 zsy 发送短消息数据包
	ON_MESSAGE(WM_UPDATE_PROGRESS,&CMainFrame::OnUpdateProgress) //2014.8.26 zsy 更新进度条
	ON_MESSAGE(WM_UPDATE_SEND_VOICE,&CMainFrame::OnUpdateSendVoice) //2014.8.31 zsy 发送声音文件消息
	ON_MESSAGE(WM_UPDATE_SEND_PICTURE,&CMainFrame::OnUpdateSendPicture) //2014.9.4 zsy 发送图片文件消息
	ON_MESSAGE(WM_UPDATE_SEND_SUSPEND,&CMainFrame::OnUpdateSendSuspend) //2014.9.12 zsy 发送暂停
//	ON_COMMAND(ID_STOP_SEND, &CMainFrame::OnStopRecv)
ON_COMMAND(ID_STOP_RECV, &CMainFrame::OnStopRecv)
//ON_COMMAND(ID_SEND_PROP, &CMainFrame::OnSendProp)
ON_COMMAND(ID_START_SEND, &CMainFrame::OnStartSend)
ON_UPDATE_COMMAND_UI(ID_START_RECV, &CMainFrame::OnUpdateStartRecv)
ON_UPDATE_COMMAND_UI(ID_STOP_RECV, &CMainFrame::OnUpdateStopRecv)
ON_COMMAND(ID_STOP_SEND, &CMainFrame::OnStopSend)
ON_UPDATE_COMMAND_UI(ID_START_SEND, &CMainFrame::OnUpdateStartSend)
ON_UPDATE_COMMAND_UI(ID_STOP_SEND, &CMainFrame::OnUpdateStopSend)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_STATUS_SENDING,   //2014.08.23 数据发送状态
	//2014.3.26
	ID_INDICATOR_HOST_MAC,  //本地主机MAC地址
	ID_INDICATOR_DES_MAC,   //接收端目的主机MAC地址
	ID_INDICATOR_SEND_DES_MAC, //发送端目的主机MAC地址
	//ID_INDICATOR_CAPS,
	//ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
	//2014.3.26
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	//2014.5.9
	b_enableMenu_Receive = TRUE;//线程未开始接受数据
	//b_oneRecThread = FALSE; //是否需要退出ThreadPacketView线程，false不退出。
	b_enableMenu_StopReceive = FALSE;//停止接收菜单不可用
//	b_enableThread_Receive = FALSE; //接收数据线程控制变量，当值为false时停止线程
	//2014.5.9
	b_enableMenu_Send = TRUE; //2014.8.31 zsy  未开始发送数据
	b_enableMenu_StopSend = FALSE; //2014.8.31 zsy 已停止数据发送
    b_suspend_send = FALSE; //2014.9.12 zsy 暂停发送
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	// 设置用于绘制所有用户界面元素的视觉管理器
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
	if(!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}
	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// 允许用户定义的工具栏操作:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//2014.08.23 zsy 设置状态栏格式
	m_wndStatusBar.SetPaneInfo(1,ID_STATUS_SENDING,SBPS_POPOUT,120);
	m_wndStatusBar.SetPaneInfo(2,ID_INDICATOR_HOST_MAC,SBPS_POPOUT,120);
	m_wndStatusBar.SetPaneInfo(3,ID_INDICATOR_DES_MAC,SBPS_POPOUT,160);
	m_wndStatusBar.SetPaneInfo(4,ID_INDICATOR_SEND_DES_MAC,SBPS_POPOUT,160);
	//2014.8.23

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 启用工具栏和停靠窗口菜单替换
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// 加载用户定义的工具栏图像
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// 启用菜单个性化(最近使用的命令)
	// TODO: 定义您自己的基本命令，确保每个下拉菜单至少有一个基本命令。
	CList<UINT, UINT> lstBasicCommands;

	lstBasicCommands.AddTail(ID_FILE_NEW);
	lstBasicCommands.AddTail(ID_FILE_OPEN);
	lstBasicCommands.AddTail(ID_FILE_SAVE);
	lstBasicCommands.AddTail(ID_FILE_PRINT);
	lstBasicCommands.AddTail(ID_APP_EXIT);
	lstBasicCommands.AddTail(ID_EDIT_CUT);
	lstBasicCommands.AddTail(ID_EDIT_PASTE);
	lstBasicCommands.AddTail(ID_EDIT_UNDO);
	lstBasicCommands.AddTail(ID_APP_ABOUT);
	lstBasicCommands.AddTail(ID_VIEW_STATUS_BAR);
	lstBasicCommands.AddTail(ID_VIEW_TOOLBAR);

	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	//2014.8.26 zsy 创建状态栏，并隐藏
	CRect rect;
	m_wndStatusBar.GetItemRect(1,&rect);
	m_progress.Create(WS_CHILD|WS_VISIBLE,rect,&m_wndStatusBar,123);
	m_progress.SetPos(0);//设置进度栏上当前进度
	m_progress.ShowWindow(FALSE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// 基类将执行真正的工作

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// 为所有用户工具栏启用自定义按钮
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	//静态分割视图
	CRect rect;
	this->GetClientRect(&rect);
	int width = rect.Width()/4;
	if ( ! m_wndSplitter.CreateStatic(this,1,2) || 
		 /*! m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CInfoView),CSize(width,0),pContext)||
		 ! m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CRecvPackView),CSize(rect.Width() - width,0),pContext)*/ 
		 ! m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CRecvPackView),CSize(rect.Width() -width,0),pContext)||
		 ! m_wndSplitter.CreateView(0,1,RUNTIME_CLASS(CInfoView),CSize(width,0),pContext))
	{
		return FALSE;
	}
	return TRUE;
	//return CFrameWndEx::OnCreateClient(lpcs, pContext);
}


void CMainFrame::OnStartRecv()
{
	// TODO: 在此添加命令处理程序代码
	//1.设置是否对数据进行保存
	CDlgSavedata  dlg_save_data;
	if(IDOK == dlg_save_data.DoModal())
	{//1.获得可用适配器的详细信息：MAC，IP等等
		BOOL b_getAdapter = GetEnableAdapterInfo();
		if(b_getAdapter == false)
		{
			AfxMessageBox(_T("未找到可用的网络设备！"),MB_OK|MB_ICONWARNING);
			return;
		}
		//::PostMessage(m_hWnd,WM_UPDATE_SOURCE_MAC,NULL,NULL);
		SendMessageA(m_hWnd,WM_UPDATE_SOURCE_MAC,NULL,NULL);

		theApp.pMyView = (CView *)m_wndSplitter.GetPane(0,0); //获取分割视图左边的一个视图 （0行0列）
		theApp.pMyTreeView = (CView *)m_wndSplitter.GetPane(0,1); //获取分割视图右边的一个视图（0行1列）
		//THREADPARAMS* ptp = new THREADPARAMS;
		//ptp->hWnd = m_wndSplitter.GetPane(0,0)->m_hWnd  ; //获取分割视图左边的一个视图 （0行0列）
		AfxBeginThread(ThreadPacketView,NULL); //开始接受数据线程
		//2014.5.9 此处需要将菜单选项“发送数据”设置为不可选，防止多次创建线程。由于VS与VC在菜单选项控件的不同，只能通过消息相应来更改，不能通过EnableMenuItem函数来更改。
		b_enableMenu_Receive = FALSE; //数据接收菜单不可用
		b_enableMenu_StopReceive = TRUE;//停止接收菜单可用
		//b_oneRecThread = FALSE; //ThreadPacketView线程已经退出
	}
	else
		return;
}


// 获得可用适配器的详细信息
bool CMainFrame::GetEnableAdapterInfo(void)
{    
	/////网卡MAC地址
	//PIP_ADAPTER_INFO结构体指针存储本机网卡信息
	PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO;
	//得到结构体大小，用于GetAdaptersInfo参数
	unsigned long stSize = sizeof(IP_ADAPTER_INFO);
	//调用GetAdaptersInfo函数，填充pIpAdapterInfo指针变量，其中stSize参数既是一个输入量，也是一个输出量
	int nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	if(ERROR_BUFFER_OVERFLOW == nRel)
	{
		//如果函数返回的是ERROR_BUFFER_OVERFLOW
		//则说明GetAdaptesInfo参数传递的内存空间不够，同时其传出stSize,表示需要的空间大小
		//这也是说明为什么stSize既是一个输入量，也是一个输出量
		//释放原来的内存空间
		delete pIpAdapterInfo;
		//重新申请内存空间用来存储所有网卡信息
		pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
		//再次调用GetAdaptersInfo函数，填充pIpAdaptersInfo指针变量
		nRel = GetAdaptersInfo(pIpAdapterInfo,&stSize);
	}
    if (ERROR_SUCCESS == nRel)
    {
		theApp.pIpAdapter_user = pIpAdapterInfo; //找到可用适配器
		//delete pIpAdapterInfo;
		return true;
    }
	if(pIpAdapterInfo)
		delete pIpAdapterInfo;
	return false;
}
//更新状态栏本机主机MAC地址
LRESULT CMainFrame::OnUpdateSourMac(WPARAM wParam, LPARAM lParam)
{
	//状态栏显示本机MAC地址
	CString strMac;
	CString strMac1;
	for (DWORD i = 0; i< theApp.pIpAdapter_user->AddressLength; i++)
	{
		/*if (i < theApp.pIpAdapter_user->AddressLength-1)
		{   
			BYTE by = theApp.pIpAdapter_user->Address[i];
			strMac1.Format(_T("%02X:"),by);
			strMac += strMac1;
		}
		else
		{
			strMac1.Format(_T("%02X"),(BYTE)theApp.pIpAdapter_user->Address[i]);
			strMac += strMac1;
		}*/
		theApp.c_src_macAddr[i] = theApp.pIpAdapter_user->Address[i];
	}
	strMac.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"),theApp.c_src_macAddr[0],theApp.c_src_macAddr[1],
		           theApp.c_src_macAddr[2],theApp.c_src_macAddr[3],theApp.c_src_macAddr[4],theApp.c_src_macAddr[5]);
	//strMac1 = strMac;
	//theApp.str_src_macAddr =strMac;
	strMac = _T("本地主机MAC地址--") + strMac;
	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(strMac);
	int index = 0;
	index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_HOST_MAC);
	m_wndStatusBar.SetPaneInfo(index,ID_INDICATOR_HOST_MAC,SBPS_POPOUT,sz.cx);
	m_wndStatusBar.SetPaneText(index,strMac);
	return 0;
}


//更新状态栏接收端目的主机MAC地址
LRESULT CMainFrame::OnUpdateDesMac(WPARAM wParam, LPARAM lParam)
{
	//状态栏显示目的主机MAC地址
	CString strMac2,strMac3;
	strMac3.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"),theApp.c_des_macAddr[0],theApp.c_des_macAddr[1],
		             theApp.c_des_macAddr[2],theApp.c_des_macAddr[3],theApp.c_des_macAddr[4],theApp.c_des_macAddr[5]);
	theApp.str_des_macAddr = strMac3;
	strMac2 = _T("接收端目的主机MAC地址--") + strMac3;
	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(strMac2);
	int index = 0;
	index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_DES_MAC);
	m_wndStatusBar.SetPaneInfo(index,ID_INDICATOR_DES_MAC,SBPS_POPOUT,sz.cx);
	m_wndStatusBar.SetPaneText(index,strMac2);
	return 0;
}
//更新状态栏发送端目的主机MAC地址
LRESULT CMainFrame::OnUpdateSendDesMac(WPARAM wParam, LPARAM lParam)
{
	//状态栏显示目的主机MAC地址
	CString strMac4,strMac5;
	strMac5.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"),theApp.c_send_des_macAddr[0],theApp.c_send_des_macAddr[1],
		theApp.c_send_des_macAddr[2],theApp.c_send_des_macAddr[3],theApp.c_send_des_macAddr[4],theApp.c_send_des_macAddr[5]);
	//theApp.str_send_des_macAddr = strMac5;
	strMac4 = _T("发送端目的主机MAC地址--") + strMac5;
	CClientDC dc(this);
	CSize sz = dc.GetTextExtent(strMac4);
	int index = 0;
	index = m_wndStatusBar.CommandToIndex(ID_INDICATOR_SEND_DES_MAC);
	m_wndStatusBar.SetPaneInfo(index,ID_INDICATOR_SEND_DES_MAC,SBPS_POPOUT,sz.cx);
	m_wndStatusBar.SetPaneText(index,strMac4);
	return 0;
}
//接收数据的线程
UINT ThreadPacketView(LPVOID pParam)    //返回值为0表示正常结束，返回值为1为不正常
{   
	//THREADPARAMS* ptp = (THREADPARAMS*)pParam;
	//添加变量
	//HWND hWnd = ptp->hWnd;
	//delete ptp;

	char err_content[PCAP_ERRBUF_SIZE]; //返回错误信息
	//char *packet_filter = NULL;
	//packet_filter = new char[filter_len];//申请内存空间
	//char *packet_filter = (char*)malloc(filter_len); //申请内存空间
	//memset(packet_filter,0x0,filter_len);   // 初始化数组
	//strcpy(packet_filter,(char*)(LPCTSTR)str_filter);
	//packet_filter = (char*)(LPCTSTR)str_filter;
	CString str_filter; //设置过滤规则,必须申请为CStringA类型，否则后面cstring转化为char*会出问题
	char *filter_buf = (char *)malloc(filter_len);
	memset(filter_buf,0x0,filter_len);
	struct bpf_program fcode; //used in pcap_compile()
	if (/*theApp.isDesMac == false ||*/ theApp.str_des_macAddr ==_T("00:00:00:00:00:00")) //未设置目的主机MAC地址，则按照本地主机的MAC地址接收
	{
		//str_filter = _T("ether dst ") + theApp.str_src_macAddr;//本地源为接收包的目的地址
		str_filter.Format(_T("ether dst %02x:%02x:%02x:%02x:%02x:%02x"),theApp.c_src_macAddr[0],theApp.c_src_macAddr[1],
			               theApp.c_src_macAddr[2],theApp.c_src_macAddr[3],theApp.c_src_macAddr[4],theApp.c_src_macAddr[5]);

	}
	else  //按照本地主机MAC地址和目的主机MAC地址接收数据包
	{
        //str_filter = _T("ether src ") + theApp.str_des_macAddr +_T(" and ether dst ") + theApp.str_src_macAddr;  
		str_filter.Format(_T("ether src %02x:%02x:%02x:%02x:%02x:%02x and ether dst  %02x:%02x:%02x:%02x:%02x:%02x"),
			theApp.c_des_macAddr[0],theApp.c_des_macAddr[1],theApp.c_des_macAddr[2],theApp.c_des_macAddr[3],theApp.c_des_macAddr[4],theApp.c_des_macAddr[5],
			theApp.c_src_macAddr[0],theApp.c_src_macAddr[1],theApp.c_src_macAddr[2],theApp.c_src_macAddr[3],theApp.c_src_macAddr[4],theApp.c_src_macAddr[5]);
	}
	strcpy_s(filter_buf,filter_len,(CStringA)str_filter);
    //开始数据包接收设置
	CString  strDeviceName;
	strDeviceName.Format(_T("%s%s"),_T("\\Device\\NPF_"),(CStringW)(theApp.pIpAdapter_user->AdapterName));  //此处必须用CStringW进行强制转换，否则会出现乱码
	pcap_t  *pcap_handle = pcap_open_live((char *)(LPCTSTR)strDeviceName,65535,1,1000,err_content);
	if (pcap_handle == NULL)
	{
		AfxMessageBox(_T("winpcap打开有误，请检查网络适配器，请检查后重新接收！"),MB_ICONWARNING|MB_OK);
		return  1;
	}

	u_int32_t  net_mask = 0xffffff; //子网掩码255.255.255.0
	//u_int32_t  net_ip;//网络地址（不包括主机号）
    //net_mask = inet_addr(theApp.pIpAdapter_user->IpAddressList.IpMask.String);
	//net_mask = atoi(theApp.pIpAdapter_user->IpAddressList.IpMask.String);

	//pcap_lookupnet((char*)(LPCTSTR)strDeviceName,&net_ip,&net_mask,err_content);
	//net_mask = 0 ; //255.255.255.0
	//char packet_filter[] = "ip" ;
	if(pcap_compile(pcap_handle,&fcode,filter_buf,1,net_mask) < 0)
	{
		AfxMessageBox(_T("winpcap编译有误，请检查网络适配器，再进行数据接收！"),MB_ICONWARNING|MB_OK);
		return 1;
	}
	/*set the filter*/
	if (pcap_setfilter(pcap_handle,&fcode)<0)
	{
		AfxMessageBox(_T("数据接收有误，请检查选择的网络适配器，再进行数据接收!"),MB_ICONWARNING|MB_OK);
		return 1;
	}
	if (pcap_datalink(pcap_handle) != DLT_EN10MB)    //返回数据链路层类型
	{
		AfxMessageBox(_T("非以太网错误！"),MB_ICONWARNING|MB_OK);
		return 1;
	}
	//开始捕获数据包
	theApp.theApp_receive_pcap_handle  = pcap_handle ; //赋给全局变量，便于控制

	//根据i_radio的值来判断是否需要把接收到得数据保存进文件
	if(0 == theApp.i_radio)  //保存数据
	{
		switch (theApp.i_combox)
		{
		case 0:
			{//保存为.cap数据包文件格式
				pcap_dumper_t *user_dumpfile; //存储文件
				user_dumpfile = pcap_dump_open(pcap_handle,(CStringA)theApp.str_fileName); //打开存储文件,此处必须用CString强制转换类型
				//user_dumpfile = pcap_dump_open(pcap_handle,"bb.txt");
				if(NULL == user_dumpfile)
				{
					AfxMessageBox(_T("打开.cap存储文件失败！"),MB_OK|MB_ICONWARNING);
					return 1;
				}
				pcap_loop(pcap_handle,-1,ethernet_protocol_packet_callback,(u_char *)user_dumpfile);//注册回调函数，开始捕获数据包，然后调用回调函数处理此数据包
				pcap_dump_close(user_dumpfile); //结束文件存储
				break;
			}
		case 1:
			{//保存为.txt文件格式2014.8.30 zsy
				try
				{
				    CFile *file = new CFile;
					if (0 == file->Open(theApp.str_fileName,CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate)) //modeNoTruncate指文件存在但没有截去原有内容时打开文件
					{
						AfxMessageBox(_T("打开.txt存储文件失败!"),MB_OK|MB_ICONWARNING);
						return 1;
					}
					pcap_loop(pcap_handle,-1,ethernet_protocol_packet_callback,(u_char *)file);//注册回调函数，开始捕获数据包，然后调用回调函数处理此数据包
					file->Close();//关闭文件
					delete file;
					break;
				}
                catch (CException* e)
				{
					e->ReportError();
					e->Delete();
					return 1;//申请内存失败
				}
				
			}
		case 2:
			{
				//保存为.wav语音文件
				HMMIO voicefile;//定义HMMIO句柄
				voicefile = mmioOpen((LPWSTR)(LPCTSTR)(theApp.str_fileName),NULL,MMIO_READWRITE);//以读写模式打开所给的WAV文件
				if (NULL == voicefile)
				{
					AfxMessageBox(_T("打开声音文件失败！"),MB_OK|MB_ICONWARNING);
					return 1;
				}
				pcap_loop(pcap_handle,-1,ethernet_protocol_packet_callback,(u_char *)voicefile);//注册回调函数，开始捕获数据包，然后调用回调函数处理此数据包
				mmioClose(voicefile,MMIO_FHOPEN);//关闭wav文件
				break;
			}
		case 3:
			{
				//保存为图像文件
				break;
			}
		
		default:
			break;
		}
		
	}
	else   //不保存数据
		pcap_loop(pcap_handle,-1,ethernet_protocol_packet_callback,NULL);//注册回调函数，开始捕获数据包，然后调用回调函数处理此数据包

	pcap_close(pcap_handle);
	//2014.5.9，停止数据包接收线程
	//((CMainFrame*)(AfxGetApp()->m_pMainWnd))->b_enableThread_Receive = FALSE;//初始化为最初值
	//AfxEndThread(0); //结束线程
	//2014.5.9
	if(theApp.theApp_receive_pcap_handle!=NULL)
	    theApp.theApp_receive_pcap_handle = NULL;//接收结束
	return 0;//结束线程
}
//Time:2014.3.29 zsy
//函数名：以太网回调函数
//功能：处理侦听到过滤后的数据包
void ethernet_protocol_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
{
	u_short  ethernet_type;    /*以太网类型*/
	struct ethernet_header *ethernet_protocol;    /*以太网协议*/
	u_char *mac_string;     /*以太网地址字符串*/
	CString  str_mac;
	/*c语言形式获得时间          
	char timestr[16];   //捕获数据包的时间
	time_t local_tv_sec;
	struct tm ltime  ;
	//将时间戳转换为可识别的格式
	local_tv_sec = packet_header->ts.tv_sec;
	localtime_s(&ltime,&local_tv_sec);
	strftime(timestr,sizeof timestr,"%H:%M:%S",&ltime);
	int  ilen;  //数据长度
	//time = ctime((const time_t*)&packet_header->ts.tv_sec);

	ilen = packet_header->len;

	Packet_Info *onePacket = new Packet_Info;
	//onePacket->str_time = timestr;   //存入时间,时：分：秒
	onePacket->str_time.Format(_T("%s,%.6d"),(CStringW)timestr,packet_header->ts.tv_usec); //存入时间,（时：分：秒,微妙）,必须使用强制转换CStringW，否则乱码
	//onePacket->iLength = ilen;
	*/
	//MFC形式获得时间
	CTime time(packet_header->ts.tv_sec);
	Packet_Info *onePacket = new Packet_Info;//新建一个存储变量
	onePacket->str_time.Format(_T("%02d:%02d:%02d"),time.GetHour(),time.GetMinute(),time.GetSecond()); //存入时间，时：分：秒

	ethernet_protocol = (struct ethernet_header *)packet_content; /*获得以太网协议内容*/
	ethernet_type = ntohs(ethernet_protocol->ether_type);
	//onePacket->type_length = ethernet_type-14 ; //存入数据协议类型或者数据长度,FPGA上传的数据包中数据长度包括了源地址+目的地址+数据类型长度=14字节
	//存入数据段内容,仅对于以太网数据包
	onePacket->type_length = ethernet_type ;
	onePacket->c_date = new u_char[ethernet_type+1];
	if (!onePacket->c_date )
	{
		delete []onePacket->c_date;
	}
	if (onePacket->type_length < 0x0600)
	{
		int i =0;
		for ( i; i < onePacket->type_length; i++) 
		{
			onePacket->c_date[i] = *(packet_content + 14 + i);
		}
		onePacket->c_date[ethernet_type] = '\0';
	}
	else
		return;
	mac_string = ethernet_protocol->ether_shost;  //获得源以太网址(转化为MAC地址1E:2B:3C:4D:5E:6F形式)
	str_mac.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"),*mac_string,*(mac_string+1),*(mac_string+2),
		*(mac_string+3),*(mac_string+4),*(mac_string+5));
	onePacket->str_source_address = str_mac;  //存入源以太网地址

	mac_string = ethernet_protocol->ether_dhost;  //获得目的以太网址
	str_mac.Format(_T("%02x:%02x:%02x:%02x:%02x:%02x"),*mac_string,*(mac_string+1),*(mac_string+2),
		*(mac_string+3),*(mac_string+4),*(mac_string+5));
	onePacket->str_des_address = str_mac;  //存入目的以太网地址

	theApp.allPacket.Add(onePacket);
	//存储数据包的内容到文件
	if (argument != NULL)
	{
        switch(theApp.i_combox)
		{
		case 0:
			{//数据包形式存储到.cap文件中
				pcap_dumper_t *dumperfile = (pcap_dumper_t *)argument;
				pcap_dump((u_char*)dumperfile,packet_header,packet_content);
				break;
			}
		case 1:
			{
				//文本形式存储到.txt文件
				CFile *newfile =(CFile *)argument;
			    newfile->SeekToEnd();
				newfile->Write(onePacket->c_date,onePacket->type_length);
				break;
			}
		case 2:
			{//文件保存为.wav声音文件
				HMMIO voicefile1 = (HMMIO)argument;
				mmioWrite(voicefile1,(const char*)(onePacket->c_date),onePacket->type_length);
				break;
			}
		case 3:
			{//文件存储到图像文件
				break;
			}
		
		default:
			break;
		}

	}

    //发送更新列表视图的自定义消息
	SendMessageA(((CMainFrame*)AfxGetMainWnd())->m_hWnd,WM_UPDATE_SEND_SUSPEND,(WPARAM)onePacket,NULL);
	SendMessageA(theApp.pMyView->m_hWnd,WM_UPDATE_LIST_VIEW,(WPARAM)onePacket,NULL);

}

void CMainFrame::OnStopRecv()
{
	// TODO: 在此添加命令处理程序代码
	if (theApp.theApp_receive_pcap_handle != NULL)    
	{
		pcap_breakloop(theApp.theApp_receive_pcap_handle);  //停止监听数据包
		//2014.5.9
		theApp.theApp_receive_pcap_handle = NULL; //停止监听
//		b_enableThread_Receive = TRUE;//请终止接收数据线程
		b_enableMenu_Receive = TRUE; //数据接收菜单恢复响应
		b_enableMenu_StopReceive  = FALSE; // 停止接收菜单不可用
		//b_oneRecThread = TRUE ;//终止ThreadPacketView线程
	}
}
void gen_packet(unsigned char *buf,int len);
void CMainFrame::OnStartSend()
{
	// TODO: 在此添加命令处理程序代码
	/*//如果未进行发送端目的主机MAC地址设置，则不执行
	if (theApp.str_send_des_macAddr == _T(""))
	{
		MessageBox(_T("请先设置发送端目的主机的MAC地址！"),NULL,MB_OK|MB_ICONWARNING);
		return ;
	}
	*/
	CSendChoice dlg_sendChoice;
	dlg_sendChoice.DoModal();
	
	/*if(IDOK == dlg_sendChoice.DoModal())
	{//获得可用适配器的详细信息：MAC，IP等等
		BOOL b_getAdapter = GetEnableAdapterInfo();
		if(b_getAdapter == false)
		{
			MessageBox(_T("未找到可用的网络设备！"),NULL,MB_OK|MB_ICONWARNING);
			return;
		}
	}*/
	/*int packetlen = 50;
	unsigned char *buf = (unsigned char*)malloc(packetlen);
	memset(buf,0x0,packetlen);

	gen_packet(buf,packetlen);
	char err_content[PCAP_ERRBUF_SIZE]; //返回错误信息
	CString  strDeviceName;
	strDeviceName.Format(_T("%s%s"),_T("\\Device\\NPF_"),(CStringW)(theApp.pIpAdapter_user->AdapterName));  //此处必须用CStringW进行强制转换，否则会出现乱码
	pcap_t  *pcap_handle = pcap_open_live((char *)(LPCTSTR)strDeviceName,65535,1,1000,err_content);
	int ret = 0;
	ret = pcap_sendpacket(pcap_handle,buf,packetlen);
	if (ret == -1)
	{
		return ; //发送失败
	}
	*/

}
void gen_packet(unsigned char *buf,int len)
{
	int i = 0;
	//填充目的主机地址
	buf[0] = 0x00;
	buf[1] = 0x1E;
	buf[2] = 0xC9;
	buf[3] = 0x07;
	buf[4] = 0xAB;
	buf[5] = 0x67;

	//填充本地主机地址
	buf[6] = 0x00;
	buf[7] = 0x1E;
	buf[8] = 0xC9;
	buf[9] = 0x07;
	buf[10] = 0xAB;
	buf[11] = 0x67;
	//存入长度100
	buf[12] = 0x00;
	buf[13] = 0x24; 
	buf[14] = 0x7E;
	buf[49] = 0x7F;
	for (i = 15;i<len-1;i++)
	{
		buf[i] = i - 14;
	}

}

//读取文件时数据发送的pcap回调函数
void dispatch_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content);
//2014.4.6   从文件中选择数据包发送
UINT ThreadFileSend(LPVOID pParam) //返回值为0表示正常结束，返回值为1为不正常
{
	FilePragram *fileParm = (FilePragram *)pParam;

	char source[PCAP_BUF_SIZE];
	pcap_t *src_fp; //转储文件
	pcap_t *dest_fp; //输出的网络接口
	//得到本地网络适配器输出设备
	CString  strDeviceName;
	char err_content[PCAP_ERRBUF_SIZE]; //返回错误信息
	strDeviceName.Format(_T("%s%s"),_T("\\Device\\NPF_"),(CStringW)(theApp.pIpAdapter_user->AdapterName));  //此处必须用CStringW进行强制转换，否则会出现乱码
	dest_fp= pcap_open_live((char *)(LPCTSTR)strDeviceName,65535,1,1000,err_content);
	theApp.theApp_send_pcap_handle = dest_fp;
	if (dest_fp == NULL)
	{
		AfxMessageBox(_T("winpcap打开有误，请检查网络适配器，请检查后重新接收！"),MB_ICONWARNING|MB_OK);
		return  1;
	}

	switch(fileParm->i_fileEx)//2014.8.31 zsy
	{
	case 0:
		{//打开.cap文件
			//根据新的winpcap语法创建一个源字符串
			if (pcap_createsrcstr(
				source,   //源字符串
				PCAP_SRC_FILE, //要打开的是文件
				NULL,   //远程主机
				NULL,   //远程主机端口
				(CStringA)fileParm->fileName, //要打开的文件名
				err_content   //错误缓冲区
				) != 0)
			{
				MessageBoxW(NULL,_T("创建winpcap源字符串失败！"),NULL,MB_OK|MB_ICONWARNING);
				return 1;
			}
			//打开输入的源文件
			if ((src_fp = pcap_open(
				(CStringA)source,
				65536,
				PCAP_OPENFLAG_PROMISCUOUS,
				1000,
				NULL,
				err_content))
				== NULL)
			{
				AfxMessageBox(_T("winpcap打开输入的源文件失败！"),MB_OK|MB_ICONWARNING);
				return 1;
			}
			//读取解析并发送数据包，直到到达文件结尾处（EOF）
			pcap_loop(
				src_fp,
				0,
				dispatch_packet_callback,
				(u_char*)dest_fp);
			pcap_close(src_fp);//释放资源
			break;
		}
	case 1:
		{//打开.txt文件
			try{
				CFile file(fileParm->fileName,CFile::modeRead); //try结构结束时，CFile的析构函数会释放CFile资源，不需要手动释放
				DWORD dwNum = (DWORD)file.GetLength(); //GetLength返回的是64位的结果，强制转化为32位的DWORD
				DWORD dwNumPacket = 1 + (dwNum)/theApp.m_packetLength; //计算要发送的数据包个数。
				int packetlen = theApp.m_packetLength + 14;//数据区长度+源地址6字节+目的地址6字节+数据长度2字节
				unsigned char *buf = (unsigned char*)malloc(packetlen);
				memset(buf,0x0,packetlen);
				//填充目的主机地址
				for(int j = 0 ;j<6; j++)
				{
					buf[j] = theApp.c_send_des_macAddr[j];
				}
				//填充本地主机地址
				for(int k = 0 ;k<6 ;k++)
				{
					buf[6+k] = theApp.c_src_macAddr[k];
				}
				//填充每帧的数据长度和数据区数据
				int lengthH = theApp.m_packetLength/256;
				int lengthL = theApp.m_packetLength%256;
				int ret = -1;
				theApp.b_continueSend = TRUE;//设置自定义帧持续发送
				for (DWORD i=0;(i<dwNumPacket)&&(TRUE == theApp.b_continueSend);i++)
				{
					//填充长度
					if (i<dwNumPacket-1)
					{
						buf[12] = lengthH;
						buf[13] = lengthL;
						//填充数据
						file.Read(buf+14,theApp.m_packetLength);
						//开始发送数据	
						ret = pcap_sendpacket(dest_fp,buf,packetlen);
					}
					else
					{
						int num = dwNum-i*theApp.m_packetLength;//最后一帧的长度
						buf[12] = (int)num/256;
						buf[13] = (int)num%256;
						//填充数据
                        file.Read(buf+14,num);
						//开始发送数据	
						ret = pcap_sendpacket(dest_fp,buf,num+14);//此处发送的长度必须加上14（头部区域）
					}
					if (ret == -1)
					{
						return 2 ; //发送失败
					}
					//newParam->i_pos += perPos;

					//SendMessageA(((CMainFrame*)AfxGetMainWnd())->m_hWnd,WM_UPDATE_PROGRESS,(WPARAM)newParam,NULL);//更新进度条
				}
				if(buf != NULL)
					free(buf);
				break;
			}
			catch(CFileException* e){
				e->ReportError();
				e->Delete();
			}
			
		}
	default:
		break;
	}
	//释放资源
	pcap_close(dest_fp);
	delete fileParm;//处理完毕后释放申请的内存
	AfxMessageBox(_T("数据发送完毕！"),MB_OK|MB_USERICON);
	if(theApp.theApp_send_pcap_handle != NULL)
		theApp.theApp_send_pcap_handle = NULL;
    return 0;
}
//2014.8.23 zsy 短消息数据发送线程
//短消息发送数据量较小，因此不需要停止发送操作
UINT ThreadMsgSend(LPVOID pParam) //返回值为0表示正常结束，返回值为1为不正常,返回值为2为数据有发送失败
{
	tagMsgParam *pMsgC = (tagMsgParam*)pParam;
	CString str=pMsgC->msg;
	pcap_t *pcap_handle = pMsgC->adhandle;
	int j= str.GetLength();

	//*****2014.8.25 zsy******
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)str,-1,NULL,0,NULL,FALSE);
	char *psText;
	psText = new char[dwNum]; //
	if(!psText)
		delete []psText;
	WideCharToMultiByte(CP_OEMCP,NULL,(LPCWSTR)str,-1,psText,dwNum,NULL,FALSE);
	int num_HDLC = theApp.m_packetLength/HDLC_lenB; //2014.9.15 zsy 计算每一以太网帧中包含多少HDLC帧
	DWORD dwNumPacket = 1 + (dwNum-1)/theApp.m_packetLength; //计算要发送的数据包个数。-1是因为宽字节转换为多字节时多计算了一个结束字符。
	//DWORD dwNumPacket = 1 + (dwNum-1)/(theApp.m_packetLength-2); //计算要发送的数据包个数。-1是因为宽字节转换为多字节时多计算了一个结束字符。,测试用
    int packetlen = theApp.m_packetLength + 14;//数据区长度+源地址6字节+目的地址6字节+数据长度2字节
	unsigned char *buf = (unsigned char*)malloc(packetlen);
	memset(buf,0x0,packetlen);
	//填充目的主机地址
	for(int j = 0 ;j<6; j++)
	{
		 buf[j] = theApp.c_send_des_macAddr[j];
	}
	//填充本地主机地址
	for(int k = 0 ;k<6 ;k++)
	{
		buf[6+k] = theApp.c_src_macAddr[k];
	}
	//填充每帧的数据长度和数据区数据
	int lengthH = theApp.m_packetLength/256;
	int lengthL = theApp.m_packetLength%256;	
	/*char err_content[PCAP_ERRBUF_SIZE]; //返回错误信息
	CString  strDeviceName;
	strDeviceName.Format(_T("%s%s"),_T("\\Device\\NPF_"),(CStringW)(theApp.pIpAdapter_user->AdapterName));  //此处必须用CStringW进行强制转换，否则会出现乱码
	pcap_t  *pcap_handle = pcap_open_live((char *)(LPCTSTR)strDeviceName,65535,1,1000,err_content); */
	//theApp.theApp_send_pcap_handle = pcap_handle;//2014.8.31 zsy
	int ret = 0;
    //2014.8.26 zsy 设置进度条
	int perPos = 100/dwNumPacket ; //计算每发送一个数据包进度条的进度滑动距离
	ProgressParam *newParam = new ProgressParam;
	newParam->i_pos = 0;//进度条位置，满格为100
	//newParam->is_show123 = 1;
	SendMessageA(((CMainFrame*)AfxGetMainWnd())->m_hWnd,WM_UPDATE_PROGRESS,(WPARAM)newParam,NULL);//显示进度条,应该使用sendmessage发送消息，使得窗口过程处理完消息再返回
	                                                                                              //否则，会在后面删除内存后执行进队消息时访问不到存储空间而出错
	//newParam->is_show123 = 3;
	/*2014.9.15 zsy 设置定时器，发送起始询问帧，是否开始发送，等待确认帧，超时时间设置为100ms*/
    for (DWORD i=0;i<dwNumPacket;i++)
    {
		/*//填充长度,测试用代码
		if (i<dwNumPacket-1)
		{
			buf[12] = lengthH;
			buf[13] = lengthL;
			buf[14] = 0x7E;
			//填充数据
			for (int m = 0; m < theApp.m_packetLength-2; m++)
			{
				buf[14+m+1] = psText[m+i*(theApp.m_packetLength-2)];
			}
			buf[14+theApp.m_packetLength-1] = 0x7F;
			//开始发送数据	
			ret = pcap_sendpacket(pcap_handle,buf,packetlen);
		}
		else
		{
			int num = dwNum-1-i*(theApp.m_packetLength-2)+2;//最后一帧的长度
			buf[12] = (int)num/256;
		    buf[13] = (int)num%256;
			buf[14] = 0x7E;
			//填充数据
			for (int m = 0; m < num-2; m++)
			{
				buf[14+m+1] = psText[m+i*(theApp.m_packetLength-2)];
			}
			buf[14+num-1] = 0x7F;
			//开始发送数据	
			ret = pcap_sendpacket(pcap_handle,buf,num+14);//此处发送的长度必须加上14（头部区域）
		}*/
		//填充长度
		if (i<dwNumPacket-1)
		{
			buf[12] = lengthH;
			buf[13] = lengthL;
			//填充数据
			for (int m = 0; m < theApp.m_packetLength; m++)
			{
				buf[14+m] = psText[m+i*theApp.m_packetLength];
			}
			//开始发送数据	
			ret = pcap_sendpacket(pcap_handle,buf,packetlen);
		}
		else
		{
			int num = dwNum-1-i*theApp.m_packetLength;//最后一帧的长度
			buf[12] = (int)num/256;
		    buf[13] = (int)num%256;
			//填充数据
			for (int m = 0; m < num; m++)
			{
				buf[14+m] = psText[m+i*theApp.m_packetLength];
			}
			//开始发送数据	
			ret = pcap_sendpacket(pcap_handle,buf,num+14);//此处发送的长度必须加上14（头部区域）
		}
		if (ret == -1)
		{
			return 2 ; //发送失败
		}
		newParam->i_pos += perPos;

		SendMessageA(((CMainFrame*)AfxGetMainWnd())->m_hWnd,WM_UPDATE_PROGRESS,(WPARAM)newParam,NULL);//更新进度条
		::Sleep(theApp.m_breaktime);//数据包是发送时间间隔
    }
	if(psText != NULL)
	    delete []psText;
	if(newParam != NULL)
	    delete newParam;
	if (buf != NULL)
		free(buf);
	//*****2014.8.25*****
	//AfxEndThread(0); //2014.8.29 退出短消息发送线程
	//if(theApp.theApp_send_pcap_handle != NULL)
		//theApp.theApp_send_pcap_handle = NULL;
	return 0;//结束线程
}
//2104.9.4 zsy 图片发送线程
UINT ThreadPictureSend(LPVOID pParam)
{
	PictureParam *pPicture = (PictureParam*)pParam;
	CFile file(pPicture->pictureName,CFile::modeRead); //try结构结束时，CFile的析构函数会释放CFile资源，不需要手动释放
	DWORD dwNum = (DWORD)file.GetLength(); //GetLength返回的是64位的结果，强制转化为32位的DWORD
	return 0;
}
//2014.8.31 zsy 声音发送线程
UINT ThreadVoiceSend(LPVOID pParam) //0正常返回，1不正常返回
{
	VoiceParam *pVoice = (VoiceParam *)pParam;
	HMMIO voicefile;//定义HMMIO句柄
	voicefile = mmioOpen((LPWSTR)(LPCTSTR)(pVoice->fileName),NULL,MMIO_READWRITE);//以读写模式打开所给的WAV文件
	if (NULL == voicefile)
	{
		AfxMessageBox(_T("打开声音文件失败！"),MB_OK|MB_ICONWARNING);
		return 1;
	}
	//char source[PCAP_BUF_SIZE];
	//pcap_t *src_fp; //转储文件
	pcap_t *dest_fp; //输出的网络接口
	//得到本地网络适配器输出设备
	CString  strDeviceName;
	char err_content[PCAP_ERRBUF_SIZE]; //返回错误信息
	strDeviceName.Format(_T("%s%s"),_T("\\Device\\NPF_"),(CStringW)(theApp.pIpAdapter_user->AdapterName));  //此处必须用CStringW进行强制转换，否则会出现乱码
	dest_fp= pcap_open_live((char *)(LPCTSTR)strDeviceName,65535,1,1000,err_content);
	theApp.theApp_send_pcap_handle = dest_fp;
	if (dest_fp == NULL)
	{
		AfxMessageBox(_T("winpcap打开有误，请检查网络适配器，请检查后重新接收！"),MB_ICONWARNING|MB_OK);
		return  1;
	}

	mmioSeek(voicefile,4,SEEK_SET);//定位到WAVE文件的数据大小位置
	DWORD dwNum ;//数据文件大小,不包括标志符RIFF和数据大小本身这8个字节
	char sum[4];
	mmioRead(voicefile,sum,4);//16进制文件存储规律，对于WORD，先存储低位字节，然后存储高位字节；而DWORD
	                          //则先存储低两位的低位，然后是高两位的高位，然后是高两位的低位，然后是高两位的高位
	memcpy(&dwNum,&sum,sizeof(DWORD));//char[4]转化为DWORD
	mmioSeek(voicefile,0,SEEK_SET);//将句柄设置到文件起始位置
	DWORD dwNumPacket = 1 + (dwNum+8)/theApp.m_packetLength; //计算要发送的数据包个数。
	int packetlen = theApp.m_packetLength + 14;//数据区长度+源地址6字节+目的地址6字节+数据长度2字节
	unsigned char *buf = (unsigned char*)malloc(packetlen);
	memset(buf,0x0,packetlen);
	//填充目的主机地址
	for(int j = 0 ;j<6; j++)
	{
		buf[j] = theApp.c_send_des_macAddr[j];
	}
	//填充本地主机地址
	for(int k = 0 ;k<6 ;k++)
	{
		buf[6+k] = theApp.c_src_macAddr[k];
	}
	//填充每帧的数据长度和数据区数据
	int lengthH = theApp.m_packetLength/256;
	int lengthL = theApp.m_packetLength%256;
	int ret = -1;
	theApp.b_continueSend = TRUE;//设置自定义帧持续发送
	for (DWORD i=0;(i<dwNumPacket)&&(TRUE == theApp.b_continueSend);i++)
	{
		//填充长度
		if (i<dwNumPacket-1)
		{
			buf[12] = lengthH;
			buf[13] = lengthL;
			//填充数据
			//file.Read(buf+14,theApp.m_packetLength);
			mmioRead(voicefile,(char*)(buf+14),theApp.m_packetLength);
			//开始发送数据	
			ret = pcap_sendpacket(dest_fp,buf,packetlen);
		}
		else
		{
			int num = dwNum-i*theApp.m_packetLength;//最后一帧的长度
			buf[12] = (int)num/256;
			buf[13] = (int)num%256;
			//填充数据
			mmioRead(voicefile,(char*)(buf+14),num);
			//开始发送数据	
			ret = pcap_sendpacket(dest_fp,buf,num+14);//此处发送的长度必须加上14（头部区域）
		}
		if (ret == -1)
		{
			return 2 ; //发送失败
		}
		//newParam->i_pos += perPos;

		//SendMessageA(((CMainFrame*)AfxGetMainWnd())->m_hWnd,WM_UPDATE_PROGRESS,(WPARAM)newParam,NULL);//更新进度条
	}
	if(buf != NULL)
		free(buf);

	//释放资源
	delete pVoice;//处理完毕释放申请的内存
	mmioClose(voicefile,MMIO_FHOPEN);//关闭wav文件
	pcap_close(dest_fp);
	AfxMessageBox(_T("数据发送完毕！"),MB_OK|MB_USERICON);
	if(theApp.theApp_send_pcap_handle != NULL)
		theApp.theApp_send_pcap_handle = NULL;
	return 0;//结束线程
}
//开始发送文件中的数据
LRESULT CMainFrame::OnUpdateSendFile(WPARAM wParam,LPARAM lParam)
{
	FilePragram *filePar = (FilePragram *)wParam;
	AfxBeginThread(ThreadFileSend,(LPVOID)filePar); //开始文件数据发送线程
	return 0;
}
//2014.8.23 zsy 开始发送短消息中的数据
LRESULT CMainFrame::OnUpdateSendMSG(WPARAM wParam,LPARAM lParam)
{
	tagMsgParam *pMsgB = (tagMsgParam*)wParam;
	AfxBeginThread(ThreadMsgSend,(LPVOID)pMsgB); //开始短消息数据发送线程
	return 0;
}
//2014.8.31 zsy 开始发送声音文件
LRESULT CMainFrame::OnUpdateSendVoice(WPARAM wParam,LPARAM lParam)
{
	tagVoicePragram *pVoice = (tagVoicePragram*)wParam;
	AfxBeginThread(ThreadVoiceSend,(LPVOID)pVoice); //开启声音发送线程
	return 0;
}
LRESULT CMainFrame::OnUpdateSendPicture(WPARAM wParam,LPARAM lParam)
{
	tagPictureParam *pPicture = (tagPictureParam*)wParam;
	AfxBeginThread(ThreadPictureSend,(LPVOID)pPicture);//开启图片发送线程
	return 0 ;
}

//2014.9.12 zsy 发送暂停功能
//对每一包信息判断是否是暂停发送消息
LRESULT CMainFrame::OnUpdateSendSuspend(WPARAM wParam,LPARAM lParam)
{
	Packet_Info *onePacket = (Packet_Info*)wParam;

	return 0;
}
//2014.8.26 zsy 更新进度条状态
LRESULT CMainFrame::OnUpdateProgress(WPARAM wParam,LPARAM lParam)
{
	ProgressParam *tagPro = (ProgressParam*)wParam;
	if (tagPro->i_pos <100)
	{
		m_progress.ShowWindow(TRUE);
		m_progress.SetPos(tagPro->i_pos);
	}
	else
	{
	    m_progress.SetPos(tagPro->i_pos);
		m_progress.ShowWindow(FALSE);
		m_wndStatusBar.SetPaneInfo(1,ID_OVER_SENDING,SBPS_POPOUT,120);
	}
	return 0;
}
void dispatch_packet_callback(u_char *argument,const struct pcap_pkthdr* packet_header,const u_char* packet_content)
{
	int ret = -1;

	//此处需要将接收到得文件中的数据包的源地址和数据地址互换

	if((ret =  pcap_sendpacket((pcap_t*)argument,packet_content,packet_header->caplen))== -1)
	{
		AfxMessageBox(_T("winpcap发送数据失败！"),MB_OK|MB_ICONWARNING);
		pcap_breakloop((pcap_t*)argument);
		return;
	}
	Sleep(theApp.m_breaktime);//数据帧发送时间间隔
}

//void CMainFrame::OnUpdateFrameMenu(HMENU hMenuAlt)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	CMenu menu;
//	menu.LoadMenuW(IDR_MAINFRAME);//获得菜单栏
//	CMenu* pSubMenu = menu.GetSubMenu(0);//得到发送数据菜单选项
//	pSubMenu->EnableMenuItem(0, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);
//	CFrameWndEx::OnUpdateFrameMenu(hMenuAlt);
//}

//2014.5.9 zsy
//功能：接收数据菜单的命令更新，保证始终只创建了一个接收数据线程
void CMainFrame::OnUpdateStartRecv(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(TRUE == b_enableMenu_Receive)   //接收数据菜单可用
		 pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);//接收数据菜单不可用
}

//2014.8.29 zsy
//功能：数据接收和停止接收菜单不能同时使用
void CMainFrame::OnUpdateStopRecv(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(TRUE == b_enableMenu_StopReceive)   //停止接收菜单可用
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);//接收数据菜单不可用
}

//2014.8.31 zsy
//功能：停止数据接收菜单
void CMainFrame::OnStopSend()
{
	// TODO: 在此添加命令处理程序代码
	if (NULL != theApp.theApp_send_pcap_handle)
	{
		pcap_breakloop(theApp.theApp_send_pcap_handle);//停止发送数据线程
		theApp.theApp_send_pcap_handle = NULL;
		b_enableMenu_Send = TRUE;//发送数据菜单可用
		b_enableMenu_StopSend = FALSE;//停止发送菜单不可用
		theApp.b_continueSend = FALSE;//停止自定义帧发送
	}
}

//2014.8.31 zsy
//功能：开始菜单的命令更新，保证发送数据和停止发送菜单同一时间只有一个可选
void CMainFrame::OnUpdateStartSend(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (TRUE == b_enableMenu_Send)//发送菜单可用
		pCmdUI->Enable(TRUE);
	else
	    pCmdUI->Enable(FALSE);
}

//2014.8.31 zsy
//功能：开始菜单的命令更新，保证发送数据和停止发送菜单同一时间只有一个可选
void CMainFrame::OnUpdateStopSend(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if(TRUE == b_enableMenu_StopSend)
		pCmdUI->Enable(TRUE);
	else
	    pCmdUI->Enable(FALSE);
}
