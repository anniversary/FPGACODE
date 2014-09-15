// MacEdit.cpp : 实现文件
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



// CMacEdit 消息处理程序
//2014.5.10
void CMacEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(false == (((nChar>='0') && (nChar<='9')) || ((nChar>='a')&&(nChar<='f')) ||(nChar>='A')&&(nChar<='F') || (nChar == 0x08 ))) //检查是否是16进制输入：0~9,a~f,A~F,或者是退格键
	{
		AfxMessageBox(IDS_MAC_WARNING,MB_OK|MB_ICONWARNING);//请输入16进制数：0~9，a~f，A~F
		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}
