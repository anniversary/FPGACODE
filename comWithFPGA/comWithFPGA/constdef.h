#define WM_UPDATE_SOURCE_MAC WM_USER+1    //���͸���״̬����������MAC��ַ
#define WM_UPDATE_DES_MAC WM_USER+2   //���͸���״̬�����ն�Ŀ������MAC��ַ
#define WM_UPDATE_LIST_VIEW WM_USER+3   //���͸����б���ͼ�İ���Ϣ
#define WM_UPDATE_SEND_DES_MAC  WM_USER+4   //���͸���״̬�����Ͷ�Ŀ������MAC��ַ
#define WM_UPDATE_PACKET_CONTENT  WM_USER+5   //������б���ͼ���ұ�����ͼ�������ݰ�������ʾ����Ϣ
#define WM_UPDATE_SEND_FILE     WM_USER+6    //����ѡ����ļ����������ݰ�
#define WM_UPDATE_SEND_MSG    WM_USER+7    //2014.8.23 zsy ����ѡ��Ķ���Ϣ�������ݰ�
#define WM_UPDATE_PROGRESS    WM_USER+8     //2014.8.23 zsy ���½�����״̬
#define WM_UPDATE_SEND_VOICE   WM_USER+9   //2014.8.31 zsy ���������ļ�����Ϣ
#define WM_UPDATE_SEND_PICTURE WM_USER+10  //2014.9.4 zsy ����ͼƬ
#define WM_UPDATE_SEND_SUSPEND WM_USER+11  //2014.9.12 zsy ��ͣ����



#define COLUMN_NUMBER  7     //listview ����ʾ������Ŀ��:��š�Դ��ַ��Ŀ�ĵ�ַ��Э�����͡�ʱ�䡢���ݳ��ȡ���������
#define filter_len 1024    //���˹�����󳤶�
#define data_len 1500   //���ݳ���46~1500
#define HDLC_lenB 15   //2014.9.15 zsy ����HDLC֡�ĳ���Ϊ120bits=15Bytes
#pragma  once
#pragma pack(1)   //����1�ֽڷ�ʽ���ж���
struct ethernet_header{
	u_char ether_dhost[6]; //Ŀ����̫����ַ
	u_char ether_shost[6];//Դ��̫����ַ
	u_short ether_type; //��̫������

};
/*
struct IfInfo_t{  //�Զ���ӿ���Ϣ
	ULONG  IPAddr;    //IP��ַ
	ULONG  IPMask;    //��������
	BYTE Source_MACAddr[MAX_ADAPTER_ADDRESS_LENGTH];  //Դmac��ַ
	BYTE Des_MacAddr[MAX_ADAPTER_ADDRESS_LENGTH];  //Ŀ��Mac��ַ
	char DeviceName[MAX_ADAPTER_NAME_LENGTH+4]; //�豸��
	char Description; //�豸����
    //pcap_t* adhandle;   //������
};
*/
#pragma pack()   //ȡ��1�ֽڶ��뷽ʽ

struct  Packet_Info{
	CString str_source_address; //Դ��ַ
	CString str_des_address; //Ŀ�ĵ�ַ
	//CString str_IP_type;  //IPЭ������
	CString str_time;   //�������ݰ���ʱ��
	u_short type_length; //������Э��涨��������0x0800ʱ������˵������װ����ʹ�õ�Э�����ͣ���С��0x0800���ڱ�ʾ���ݵĴ�С
	//u_char c_date[data_len] ;//���ݰ�����
	u_char *c_date; //��̬�������ݰ������ڴ�
	//int  iLength;  //���ݳ���
};
typedef struct tagTHREADPARAMS{  
	HWND hWnd;//��ͼ�ľ��
}THREADPARAMS;
typedef struct tagMsgParam{
	CString  msg;
	pcap_t *adhandle; //�����������
}MsgParam;
typedef struct tagTreeViewInfo{    //listview �� treeview��ͼ���ݵĽṹ��
	int row ;  //�к�
	CString strData; //��������
}TreeViewItemInfo;
typedef struct tagProcessPragram{
	//int  is_show123; //�Ƿ���ʾ��������1������Ҫ��ʾ��2������Ҫ���أ�3�����������
	int  i_pos; //��ǰ״̬����ʾ����
}ProgressParam;
typedef struct tagFilePragram{//��ͨ�ļ��Ľṹ��
	CString fileName;//�ļ���ȫ��
	int i_fileEx; //�ļ�����׺��0��ʾ.cap,1��ʾ.txt
}FilePragram;
typedef struct tagVoicePragram{//2014.8.31 zsy �����ļ��Ľṹ��
	CString fileName;//�ļ���ȫ��
}VoiceParam;
typedef struct tagPictureParam{
	CString pictureName;//�ļ���ȫ��
}PictureParam;
typedef struct tagHDLCParam{//2014.9.13 zsy ����HDLC��֡�ṹ��Ԥ����120bits
    //u_char beginTag;//��ʼ��־
	u_char revAddr;//����վ��ַ�����
	u_char multiNS;//���ݷ�����Ϣ֡���Ǽ��֡�����ֽ��н�����ͬ����1���1��2λ��ʾ����֡������,
	             //��1λΪ��0����ʾ����Ϣ֡����1��2λΪ��10���Ǽ��֡����11�����ޱ��֡
	             //��Ϣ֡ʱ��2~8λ��ŷ���֡��ţ�
	             //���֡ʱ��3��4λ��ʾS֡���ͱ��룬00�����վ�����01���ܾ����գ�10������δ������11��ѡ��ܾ�
	u_char multiNR;//��1λ������ѯλ����2~8λ��ʾ�¸�Ԥ��Ҫ���յ�֡�����
	             //��Ϣ֡ʱ����ѯλΪ1��ʾҪ����ѯ�Ĵ�վ������Ӧ
	             //���֡ʱ����ѯλΪ1��ʾ���շ�ȷ�Ͻ���
	u_char content[10]; //������Ϣ�ֶΣ��洢10���ֽ�
	u_short codeCRC;//crcУ���ֶ�
}HDLCParam; 
