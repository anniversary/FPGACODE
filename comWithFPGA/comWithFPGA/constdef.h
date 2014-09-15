#define WM_UPDATE_SOURCE_MAC WM_USER+1    //发送更新状态栏本地主机MAC地址
#define WM_UPDATE_DES_MAC WM_USER+2   //发送更新状态栏接收端目的主机MAC地址
#define WM_UPDATE_LIST_VIEW WM_USER+3   //发送更新列表视图的包信息
#define WM_UPDATE_SEND_DES_MAC  WM_USER+4   //发送更新状态栏发送端目的主机MAC地址
#define WM_UPDATE_PACKET_CONTENT  WM_USER+5   //从左边列表视图向右边树视图发送数据包内容显示的消息
#define WM_UPDATE_SEND_FILE     WM_USER+6    //根据选择的文件来发送数据包
#define WM_UPDATE_SEND_MSG    WM_USER+7    //2014.8.23 zsy 根据选择的短消息发送数据包
#define WM_UPDATE_PROGRESS    WM_USER+8     //2014.8.23 zsy 更新进度条状态
#define WM_UPDATE_SEND_VOICE   WM_USER+9   //2014.8.31 zsy 发送声音文件的消息
#define WM_UPDATE_SEND_PICTURE WM_USER+10  //2014.9.4 zsy 发送图片
#define WM_UPDATE_SEND_SUSPEND WM_USER+11  //2014.9.12 zsy 暂停发送



#define COLUMN_NUMBER  7     //listview 中显示的列项目数:序号、源地址、目的地址、协议类型、时间、数据长度、数据内容
#define filter_len 1024    //过滤规则最大长度
#define data_len 1500   //数据长度46~1500
#define HDLC_lenB 15   //2014.9.15 zsy 定义HDLC帧的长度为120bits=15Bytes
#pragma  once
#pragma pack(1)   //按照1字节方式进行对齐
struct ethernet_header{
	u_char ether_dhost[6]; //目的以太网地址
	u_char ether_shost[6];//源以太网地址
	u_short ether_type; //以太网类型

};
/*
struct IfInfo_t{  //自定义接口信息
	ULONG  IPAddr;    //IP地址
	ULONG  IPMask;    //子网掩码
	BYTE Source_MACAddr[MAX_ADAPTER_ADDRESS_LENGTH];  //源mac地址
	BYTE Des_MacAddr[MAX_ADAPTER_ADDRESS_LENGTH];  //目的Mac地址
	char DeviceName[MAX_ADAPTER_NAME_LENGTH+4]; //设备名
	char Description; //设备描述
    //pcap_t* adhandle;   //保存句柄
};
*/
#pragma pack()   //取消1字节对齐方式

struct  Packet_Info{
	CString str_source_address; //源地址
	CString str_des_address; //目的地址
	//CString str_IP_type;  //IP协议类型
	CString str_time;   //捕获数据包的时间
	u_short type_length; //，根据协议规定，当大于0x0800时，用于说明所封装数据使用的协议类型，当小于0x0800用于表示数据的大小
	//u_char c_date[data_len] ;//数据包内容
	u_char *c_date; //动态申请数据包数据内存
	//int  iLength;  //数据长度
};
typedef struct tagTHREADPARAMS{  
	HWND hWnd;//视图的句柄
}THREADPARAMS;
typedef struct tagMsgParam{
	CString  msg;
	pcap_t *adhandle; //保存网卡句柄
}MsgParam;
typedef struct tagTreeViewInfo{    //listview 向 treeview视图传递的结构体
	int row ;  //行号
	CString strData; //数据内容
}TreeViewItemInfo;
typedef struct tagProcessPragram{
	//int  is_show123; //是否显示进度栏：1代表需要显示，2代表需要隐藏，3代表无需操作
	int  i_pos; //当前状态栏显示进度
}ProgressParam;
typedef struct tagFilePragram{//普通文件的结构体
	CString fileName;//文件名全称
	int i_fileEx; //文件名后缀，0表示.cap,1表示.txt
}FilePragram;
typedef struct tagVoicePragram{//2014.8.31 zsy 声音文件的结构体
	CString fileName;//文件名全称
}VoiceParam;
typedef struct tagPictureParam{
	CString pictureName;//文件名全称
}PictureParam;
typedef struct tagHDLCParam{//2014.9.13 zsy 定义HDLC的帧结构，预定义120bits
    //u_char beginTag;//起始标志
	u_char revAddr;//接收站地址或序号
	u_char multiNS;//根据发送信息帧还是监控帧，此字节中解析不同。第1或第1、2位表示传送帧的类型,
	             //第1位为“0”表示是信息帧，第1、2位为“10”是监控帧，“11”是无编号帧
	             //信息帧时，2~8位存放发送帧序号；
	             //监控帧时，3、4位表示S帧类型编码，00：接收就绪；01：拒绝接收；10：接收未就绪；11：选择拒绝
	u_char multiNR;//第1位代表轮询位，第2~8位表示下个预期要接收的帧的序号
	             //信息帧时：轮询位为1表示要求被轮询的从站给出响应
	             //监控帧时：轮询位为1表示接收方确认结束
	u_char content[10]; //具体信息字段，存储10个字节
	u_short codeCRC;//crc校验字段
}HDLCParam; 
