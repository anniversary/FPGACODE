// MacEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "comWithFPGA.h"
#include "MacEdit.h"


// CMacEdit

IMPLEMENT_DYNAMIC(CMacEdit, CEdit)

CMacEdit::CMacEdit()
{

}

CMacEdit::~CMacEdit()
{
}


BEGIN_MESSAGE_MAP(CMacEdit, CEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()



// CMacEdit ��Ϣ�������
//2014.5.10
void CMacEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(false == (((nChar>='0') && (nChar<='9')) || ((nChar>='a')&&(nChar<='f')) ||(nChar>='A')&&(nChar<='F') || (nChar == 0x08 ))) //����Ƿ���16�������룺0~9,a~f,A~F,�������˸��
	{
		AfxMessageBox(IDS_MAC_WARNING,MB_OK|MB_ICONWARNING);//������16��������0~9��a~f��A~F
		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
