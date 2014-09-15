#pragma once


// CMacEdit

class CMacEdit : public CEdit
{
	DECLARE_DYNAMIC(CMacEdit)

public:
	CMacEdit();
	virtual ~CMacEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};


