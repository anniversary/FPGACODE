#pragma once

//2014.4.4 zsy
#include "afx.h"
#include "MacEdit.h"
//2014.4.4 zsy
// CDlgSavedata 对话框

class CDlgSavedata : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSavedata)

public:
	CDlgSavedata(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgSavedata();

// 对话框数据
	enum { IDD = IDD_PACKET_RECEIVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadio1();
	CButton m_ration1;
	CEdit m_save_edit; //文件名
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	// //创建新的数据保存文件
	bool CreatNewFile(CString fileName);
	afx_msg void OnBnClickedRadio2();
	CMacEdit m_des_mac1;
	CMacEdit m_des_mac2;
	CMacEdit m_des_mac3;
	CMacEdit m_des_mac4;
	CMacEdit m_des_mac5;
	CMacEdit m_des_mac6;
	CButton m_isClear;
//	afx_msg void OnMaxtextMacEdit1();
	afx_msg void OnCbnSelchangeCombo1();
	CComboBox m_fileformat_comb;
};
