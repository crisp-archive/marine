// MarineDlg.h : header file
//

#if !defined(AFX_MARINEDLG_H__2FD6D809_E786_4D55_B94D_B9D9EDB74541__INCLUDED_)
#define AFX_MARINEDLG_H__2FD6D809_E786_4D55_B94D_B9D9EDB74541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMarineDlg dialog

class CMarineDlg : public CDialog
{
	// Construction
public:
	CMarineDlg(CWnd* pParent = NULL);	// standard constructor
	
	typedef struct tagUSERINFO
	{
		char name[20];
		int number;
		int stage;
	} USERINFO;
	
	typedef struct tagMAPINFO
	{
		char name[30];
		char mission[100];
		int size;
		int method;
		char map[16][16];
		int torpedo,gas,battery;
		char captain[20];
		int number;
	} MAPINFO;
	
	typedef struct tagBASICINFO
	{
		int nGas;
		int nBattery;
		int nHitPoint;
		int nTorpedo;

		int nDirect;
		int nSpeed;
		BOOL bIsRun;
		BOOL bIsUp;

		char map[16][16];

		POINT Pos;

	} BASICINFO;
	
	
	typedef struct tagONGAME
	{
		char m_szScreen[400];
		MAPINFO* map;
		BASICINFO m_nBase;
		char temp[100];
		int step;
		char UnReplace;
	} ONGAME;
	
#define NORTH 1
#define WN 2
#define WEST 3
#define WS 4
#define SOUTH 5
#define ES 6
#define EAST 7
#define EN 8
	
	ONGAME m_OnGame;
	void GetUserInfo(USERINFO* UserInfo);
	
	void LoadMap(int stage,MAPINFO* map);
	void FillScreen();
	void Die();
	void Win();
	int CountEnemy();
	int CountEnemyNear();
	
	// Dialog Data
	//{{AFX_DATA(CMarineDlg)
	enum { IDD = IDD_MARINE_DIALOG };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarineDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	//{{AFX_MSG(CMarineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUp();
	afx_msg void OnClose();
	afx_msg void OnDown();
	afx_msg void OnLeft();
	afx_msg void OnRight();
	afx_msg void OnAttack();
	afx_msg void On0();
	afx_msg void On1();
	afx_msg void On2();
	afx_msg void On3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARINEDLG_H__2FD6D809_E786_4D55_B94D_B9D9EDB74541__INCLUDED_)
