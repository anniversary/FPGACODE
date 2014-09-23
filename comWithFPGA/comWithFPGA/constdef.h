#include "pcap.h"

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
#define WM_UPDATE_SEND_RESEND WM_USER+12  //2014.9.20 zsy ����Ƿ���Ҫ���·���
#define WM_UPDATE_RECEIVE_FPGA_CHECK WM_USER+13 //2014.9.21 zsy ����Ƿ��յ�FPGA���͵���ͣ֡


#define COLUMN_NUMBER  7     //listview ����ʾ������Ŀ��:��š�Դ��ַ��Ŀ�ĵ�ַ��Э�����͡�ʱ�䡢���ݳ��ȡ���������
#define filter_len 1024    //���˹�����󳤶�
#define data_len 1500   //���ݳ���46~1500
#define HDLC_lenB 15   //2014.9.15 zsy ����HDLC֡�ĳ���Ϊ120bits=15Bytes
#define HDLC_lenB_inf 10 //2014.9.19 zsy ����HDLC֡����Ϣ�ֶ�Ϊ10Bytes
#define CRC_16_CCITT 0x1021 //2014.9.17 zsy �������ɶ���ʽ
#define HDLC_Serial_Max 1024 //2014.9.19 zsy �趨HDLC֡����������Ϊ1024����ģ1024����
#define PACKETSIZE 31   //2014.9.20 zsy   31=��̫��֡ͷ��14��+FPGA�����ֶΣ�2��+HDLC֡���壨15��
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
/*typedef struct tagHDLCParam{//2014.9.13 zsy ����HDLC��֡�ṹ��Ԥ����120bits
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
}HDLCParam; */
typedef struct tagHDLCParam{//2014.9.19 zsy ����HDLC��֡�ṹ��Ԥ����120bits
	//u_char beginTag;//��ʼ��־
	u_char controlPart[3];//3�ֽڵ�֡ͷ��������ַ�ֶΣ������ֶ�
	u_char content[10]; //������Ϣ�ֶΣ��洢10���ֽ�
	u_short codeCRC;//crcУ���ֶ�
}HDLCParam; 
typedef struct tagFPGAParam{//2014.9.15 ����FPGA�����ֶΣ�Ԥ����16bits,ʹ��λ��������
	u_char apperceive;//��֪ѡ��,0:�����и�֪
	u_char bps:2; //���ߴ������ʿ����ֶΡ�0:1.25Mbps,1:10Mbps,2:150bps
	u_char project:2;//��Ŀ���͡�0����Ƶ��1����Ƶ
	u_char operation:2;//���ݲ�����0���������ͣ�1������ز��ԣ�2��������ʾ
	u_char rateControl:2; //FPGA����λ�����������ֶΣ�����λ����FPGA������������ʱ���ֶ�Ϊ0�������ֶ�Ϊʱ2����ʾ��Ҫ��ͣ����,Ϊ3ʱ����ʾ�ָ�����
}FPGAParam;
/*typedef struct tagHDLCControlPartA{//2014.9.16 zsy ����HDLC����֡�����ֶ�
	u_char S_or_U:2;//S֡��U֡�жϱ�־��2��S֡��3��U֡
	u_char rateAnderror:2;//�������������ֶΣ�4�������0�����վ������߷��;�����1���ܾ����գ�2������δ������3��ѡ��ܾ�
	u_char fpgaSection:4;//��Ҫ�������ŵ������FPGA������Ϣ
	u_char PFSection:1;//P/F��־λ
	u_char requestNum:7;//������֡���
}HDLCSPart;
typedef struct tagHDLCControlPartB{//2014.9.16 zsy ������Ϣ֡�����ֶ�
	u_char S_or_U:1;//S֡��U֡�жϱ�־��0��I֡��1��S֡����U֡
	u_char sendNum:7;//���η���֡���
	u_char PFSection:1;//P/F��־λ��������վ�㷢��������֡��ȡ��P��λ�����ѯ�����ã�������λΪ1ʱ��
	                   //Ҫ�󱻲�ѯ�Ĵ�վ��������Ӧ���ڴ�վ����Ӧ��վ���֡��ȡ��F��λ�����������
	                   //���ͻ�ȷ�Ͻ��������ã�������λΪ1ʱ����ʾ��վ�����ݷ�����ϣ�������Ӧ���
	u_char requestNum:7;//������֡���
}HDLCIPart;*/
typedef struct tagHDLCControlPartA{//2014.9.19 zsy ����HDLC����֡�����ֶ�,��18bits��֡�Ų���10bits����
	u_char revAddr:2;//����վ��ַ�����,2bits
	u_char S_or_U:2;//S֡��U֡�жϱ�־��2��S֡��3��U֡
	u_char rateAnderror:2;//�������������ֶΣ�4�������0�����վ������߷��;�����1���ܾ����գ�2������δ������3��ѡ��ܾ�
	u_char fpgasectionA:2;//��Ҫ�������ŵ������FPGA������Ϣ�ֶ�A
	u_char fpgaSectionB:5;//��Ҫ�������ŵ������FPGA������Ϣ�ֶ�B
	u_char PFSection:1;//P/F��־λ
	u_char requestNumH:2;//������֡��ŵĸ���λ
	u_char requestNumL; //������֡��ŵĵͰ�λ
}HDLCSPart;
typedef struct tagHDLCControlPartB{//2014.9.19 zsy ������Ϣ֡�����ֶΣ���18bits��֡�Ų���10bits����
	u_char revAddr:2;//����վ��ַ�����,2bits
	u_char S_or_U:1;//S֡��U֡�жϱ�־��0��I֡��1��S֡����U֡
	u_char sendNumH:5;//���η���֡��ŵĸ�5λ
	u_char sendNumL:5;//���η���֡��ŵĵ�5λ
	u_char PFSection:1;//P/F��־λ��������վ�㷢��������֡��ȡ��P��λ�����ѯ�����ã�������λΪ1ʱ��
	                    //Ҫ�󱻲�ѯ�Ĵ�վ��������Ӧ���ڴ�վ����Ӧ��վ���֡��ȡ��F��λ�����������
	                    //���ͻ�ȷ�Ͻ��������ã�������λΪ1ʱ����ʾ��վ�����ݷ�����ϣ�������Ӧ���
	u_char requestNumH:2;//������֡��ŵĸ���λ
	u_char requestNumL; //������֡��ŵĵͰ�λ
}HDLCIPart;
typedef struct tagSendPacketParam{//2014.9.20 zsy ���淢��ʱ����̫�����ݰ����ݣ����ڲ����Ƶ��ش�
	u_char pData[PACKETSIZE];
}SendPacket;
