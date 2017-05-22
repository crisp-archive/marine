// MarineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Marine.h"
#include "MarineDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarineDlg dialog

CMarineDlg::CMarineDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMarineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarineDlg)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMarineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarineDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMarineDlg, CDialog)
//{{AFX_MSG_MAP(CMarineDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_WM_TIMER()
ON_BN_CLICKED(IDC_UP, OnUp)
ON_WM_CLOSE()
ON_BN_CLICKED(IDC_DOWN, OnDown)
ON_BN_CLICKED(IDC_LEFT, OnLeft)
ON_BN_CLICKED(IDC_RIGHT, OnRight)
ON_BN_CLICKED(IDC_ATTACK, OnAttack)
ON_BN_CLICKED(IDC_0, On0)
ON_BN_CLICKED(IDC_1, On1)
ON_BN_CLICKED(IDC_2, On2)
ON_BN_CLICKED(IDC_3, On3)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarineDlg message handlers

BOOL CMarineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	USERINFO* UserInfo=new USERINFO;
	GetUserInfo(UserInfo);

	m_OnGame.map=new MAPINFO;
	UserInfo->stage++;
	LoadMap(UserInfo->stage,m_OnGame.map);
	itoa((m_OnGame.map->number),m_OnGame.temp,10);

	int i,j;
	for(i=0;i<16;i++){
		for(j=0;j<15;j++){
			m_OnGame.m_nBase.map[i][j]=' ';
		}
		m_OnGame.m_nBase.map[i][j]='\0';
	}
	
	SetDlgItemText(IDC_NAME,m_OnGame.map->name);
	SetDlgItemText(IDC_MISSION,m_OnGame.map->mission);
	
	char text[30];
	strcpy(text,"U-");
	itoa((m_OnGame.map->number),m_OnGame.temp,10);
//	MessageBox(m_OnGame.temp);
	strcat(text,m_OnGame.temp);
	strcat(text," : ");
	strcat(text,m_OnGame.map->captain);
	SetDlgItemText(IDC_STATIC1,text);
	
	m_OnGame.m_nBase.bIsRun=FALSE;
	m_OnGame.m_nBase.bIsUp=TRUE;
	m_OnGame.m_nBase.nDirect=NORTH;
	m_OnGame.m_nBase.nHitPoint=10;
	m_OnGame.step=0;
	
	m_OnGame.m_nBase.nGas=m_OnGame.map->gas;
	m_OnGame.m_nBase.nBattery=m_OnGame.map->battery;
	m_OnGame.m_nBase.nTorpedo=m_OnGame.map->torpedo;
	
	CButton* b=(CButton*)GetDlgItem(IDC_UP);
	b->EnableWindow(FALSE);
	
	for(i=0;i<m_OnGame.map->size;i++){
		for(j=0;j<m_OnGame.map->size;j++){
			if(m_OnGame.map->map[i][j]=='U'){
				m_OnGame.m_nBase.Pos.x=i;
				m_OnGame.m_nBase.Pos.y=j;
			}
		}
	}
	
	if((UserInfo->stage)>20){
		Win();
		return TRUE;
	}
	else{
		delete UserInfo;
	}
	int is=MessageBox("是否开始游戏?",NULL,MB_OKCANCEL);
	if(is==IDOK){
		SetTimer(0,1000,NULL);
	}
	else{
		EndDialog(TRUE);
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMarineDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_OnGame.map->method==1){
		if(m_OnGame.UnReplace=='A'){
			Win();
			return;
		}
	}
	if(m_OnGame.map->method==2){
		if(CountEnemy()==0){
			Win();
			return;
		}
	}
	
	if(m_OnGame.m_nBase.bIsUp==FALSE){
		m_OnGame.m_nBase.nBattery--;
	}
	if(m_OnGame.m_nBase.bIsRun==TRUE){
		m_OnGame.step++;
		if(m_OnGame.m_nBase.bIsUp==TRUE && m_OnGame.m_nBase.nBattery<15){
			m_OnGame.m_nBase.nBattery++;
		}
	}
	if(m_OnGame.m_nBase.bIsRun==TRUE && m_OnGame.step!=0 && m_OnGame.step%m_OnGame.m_nBase.nSpeed==0){
		if(m_OnGame.m_nBase.bIsUp==TRUE){
			m_OnGame.m_nBase.nGas--;
			//		m_OnGame.m_nBase.nBattery++;
		}
//		else{
//			m_OnGame.m_nBase.nBattery--;
//		}
		m_OnGame.map->map[m_OnGame.m_nBase.Pos.x][m_OnGame.m_nBase.Pos.y]=' ';
//		m_OnGame.step=0;
		switch(m_OnGame.m_nBase.nDirect){
		case EAST:
			m_OnGame.m_nBase.Pos.y++;
			break;
		case WEST:
			m_OnGame.m_nBase.Pos.y--;
			break;
		case SOUTH:
			m_OnGame.m_nBase.Pos.x++;
			break;
		case NORTH:
			m_OnGame.m_nBase.Pos.x--;
			break;
		case WN:
			m_OnGame.m_nBase.Pos.y--;
			m_OnGame.m_nBase.Pos.x--;
			break;
		case WS:
			m_OnGame.m_nBase.Pos.y--;
			m_OnGame.m_nBase.Pos.x++;
			break;
		case EN:
			m_OnGame.m_nBase.Pos.y++;
			m_OnGame.m_nBase.Pos.x--;
			break;
		case ES:
			m_OnGame.m_nBase.Pos.y++;
			m_OnGame.m_nBase.Pos.x++;
			break;
		default:
			break;
		}
	}
	m_OnGame.UnReplace=m_OnGame.map->map[m_OnGame.m_nBase.Pos.x][m_OnGame.m_nBase.Pos.y];
	if(m_OnGame.m_nBase.Pos.x<0 || m_OnGame.m_nBase.Pos.y<0 || m_OnGame.m_nBase.Pos.x>m_OnGame.map->size-1 || m_OnGame.m_nBase.Pos.y>m_OnGame.map->size-1){
		Die();
		return;
	}
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x][m_OnGame.m_nBase.Pos.y]=='/'){
		Die();
		return;
	}
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x][m_OnGame.m_nBase.Pos.y]=='X'){
		Die();
		return;
	}
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x][m_OnGame.m_nBase.Pos.y]=='O'){
		Die();
		return;
	}
	
	m_OnGame.map->map[m_OnGame.m_nBase.Pos.x][m_OnGame.m_nBase.Pos.y]='U';
	
	FillScreen();
	
	switch(m_OnGame.m_nBase.nDirect){
	case EAST:
		strcpy(m_OnGame.temp,"东方");
		break;
	case WEST:
		strcpy(m_OnGame.temp,"西方");
		break;
	case SOUTH:
		strcpy(m_OnGame.temp,"南方");
		break;
	case NORTH:
		strcpy(m_OnGame.temp,"北方");
		break;
	case ES:
		strcpy(m_OnGame.temp,"东南");
		break;
	case EN:
		strcpy(m_OnGame.temp,"东北");
		break;
	case WS:
		strcpy(m_OnGame.temp,"西南");
		break;
	case WN:
		strcpy(m_OnGame.temp,"西北");
		break;
	default:
		break;
	}
	SetDlgItemText(IDC_DIRECT,m_OnGame.temp);
	
	if(m_OnGame.m_nBase.bIsUp==TRUE){
		if(CountEnemyNear()>0){
			m_OnGame.m_nBase.nHitPoint-=CountEnemyNear();
		}
	}
	if(m_OnGame.m_nBase.nHitPoint<=0){
		Die();
		return;
	}
	
	if(m_OnGame.m_nBase.bIsUp==FALSE){
		if(m_OnGame.m_nBase.nBattery<=2){
			strcpy(m_OnGame.temp,"潜艇电量过少");
			SetDlgItemText(IDC_ALARM,m_OnGame.temp);
		}
		if(m_OnGame.m_nBase.nBattery<0){
			Die();
			return;
		}
	}
	
	if(m_OnGame.m_nBase.bIsUp==TRUE && m_OnGame.m_nBase.bIsRun==TRUE){
		if(m_OnGame.m_nBase.nGas<=2){	
			strcpy(m_OnGame.temp,"潜艇燃料过少 ");
			SetDlgItemText(IDC_ALARM,m_OnGame.temp);
		}
		if(m_OnGame.m_nBase.nGas<0){
			Die();
			return;
		}
	}
	if(m_OnGame.m_nBase.nTorpedo==0){
		CButton* b=(CButton*)GetDlgItem(IDC_ATTACK);
		b->EnableWindow(FALSE);
	}
	
	itoa(m_OnGame.m_nBase.nTorpedo,m_OnGame.temp,10);
	SetDlgItemText(IDC_TORPEDO,m_OnGame.temp);
	itoa(m_OnGame.m_nBase.nBattery,m_OnGame.temp,10);
	SetDlgItemText(IDC_BAT,m_OnGame.temp);
	itoa(m_OnGame.m_nBase.nGas,m_OnGame.temp,10);
	SetDlgItemText(IDC_GAS,m_OnGame.temp);
	itoa(m_OnGame.m_nBase.nHitPoint,m_OnGame.temp,10);
	SetDlgItemText(IDC_HP,m_OnGame.temp);
	
	CDialog::OnTimer(nIDEvent);
}


void CMarineDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMarineDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CMarineDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// Get user info from profile
void CMarineDlg::GetUserInfo(USERINFO* UserInfo)
{
	char* path=new char[240];
	char file[MAX_PATH];
	char* ret=new char[16];
	GetCurrentDirectory(240,path);
	strcpy(file,path);
	strcat(file,"\\user.thi");
	GetPrivateProfileString("USER","NAME","Default Name",ret,16,file);
	strcpy(UserInfo->name,ret);
	UserInfo->number=GetPrivateProfileInt("USER","NUMBER",1,file);
	UserInfo->stage=GetPrivateProfileInt("USER","STAGE",0,file);
	/*	MessageBox(UserInfo->name);
	itoa(UserInfo->number,ret,10);
	MessageBox(ret);
	itoa(UserInfo->stage,ret,10);
	MessageBox(ret);  */
	delete path,ret;
	return;
}

void CMarineDlg::LoadMap(int stage,MAPINFO* map)
{
	char* path=new char[240];
	char file[MAX_PATH],number[3];
	GetCurrentDirectory(240,path);
	strcpy(file,path);
	strcat(file,"\\map\\map");
	itoa(stage,number,10);
	strcat(file,number);
	strcat(file,".thi");
	GetPrivateProfileString("map","name","Default Name",path,240,file);
	strcpy(map->name,path);
	GetPrivateProfileString("map","mission","Default Name",path,240,file);
	strcpy(map->mission,path);
	GetPrivateProfileString("map","captain","Default Name",path,240,file);
	strcpy(map->captain,path);
	
	map->method=GetPrivateProfileInt("map","method",1,file);
	map->size=GetPrivateProfileInt("map","size",1,file);
	map->torpedo=GetPrivateProfileInt("map","torpedo",10,file);
	map->gas=GetPrivateProfileInt("map","gas",10,file);
	map->battery=GetPrivateProfileInt("map","battery",10,file);
	map->number=GetPrivateProfileInt("map","number",1,file);

	int i;
	for(i=1;i<=map->size;i++){
		itoa(i,number,10);
		GetPrivateProfileString("map",number,"Default Name",path,240,file);
		strcpy(map->map[i-1],path);
		//		MessageBox(map->map[i-1]);
	}
	delete path;
	/*	MessageBox(map->name);
	MessageBox(map->mission); */
	return;
}

void CMarineDlg::FillScreen()
{	
	int i,j;
	int x,y;

	for(i=0;i<16;i++){
		for(j=0;j<16;j++){
			if(m_OnGame.map->map[i][j]=='A')
			m_OnGame.m_nBase.map[i][j]='A';
		}
	}

	x=m_OnGame.m_nBase.Pos.x;
	y=m_OnGame.m_nBase.Pos.y;

	for(i=x-2;i<=x+2;i++){
		for(j=y-2;j<=y+2;j++){
			if(i>=0 && i<m_OnGame.map->size && j>=0 && j<m_OnGame.map->size){
				m_OnGame.m_nBase.map[i][j]=(m_OnGame.map->map[i][j]);
			}
		}
	}

	strcpy(m_OnGame.m_szScreen,"");
	for(i=0;i<16;i++){
	//	MessageBox(map[i]);
		strcat(m_OnGame.m_szScreen,m_OnGame.m_nBase.map[i]);
		strcat(m_OnGame.m_szScreen,"\r\n");
	}
	
	SetDlgItemText(IDC_MAP,m_OnGame.m_szScreen);
	return;
}

void CMarineDlg::OnClose() 
{
	KillTimer(TRUE);
	delete m_OnGame.map;
	CDialog::OnClose();
}

void CMarineDlg::OnUp() 
{
	CButton* b=(CButton*)GetDlgItem(IDC_UP);
	b->EnableWindow(FALSE);
	b=(CButton*)GetDlgItem(IDC_DOWN);
	b->EnableWindow(TRUE);
	b=(CButton*)GetDlgItem(IDC_3);
	b->EnableWindow(TRUE);
	
	m_OnGame.m_nBase.bIsUp=TRUE;
	m_OnGame.m_nBase.bIsRun=FALSE;
}

void CMarineDlg::OnDown() 
{
	CButton* b=(CButton*)GetDlgItem(IDC_UP);
	b->EnableWindow(TRUE);
	b=(CButton*)GetDlgItem(IDC_DOWN);
	b->EnableWindow(FALSE);
	b=(CButton*)GetDlgItem(IDC_3);
	b->EnableWindow(FALSE);
	
	m_OnGame.m_nBase.bIsUp=FALSE;
	m_OnGame.m_nBase.bIsRun=FALSE;
}

void CMarineDlg::OnLeft()
{
	if(m_OnGame.m_nBase.nDirect<8){
		m_OnGame.m_nBase.nDirect++;
	}
	else{
		m_OnGame.m_nBase.nDirect=1;
	}
}

void CMarineDlg::OnRight() 
{
	if(m_OnGame.m_nBase.nDirect>1){
		m_OnGame.m_nBase.nDirect--;
	}
	else{
		m_OnGame.m_nBase.nDirect=8;
	}
}

void CMarineDlg::OnAttack() 
{
	int x,y;
	x=m_OnGame.m_nBase.Pos.x;
	y=m_OnGame.m_nBase.Pos.y;
	m_OnGame.m_nBase.nTorpedo--;
	switch(m_OnGame.m_nBase.nDirect){
	case EAST:
		y=m_OnGame.m_nBase.Pos.y+1;
		break;
	case WEST:
		y=m_OnGame.m_nBase.Pos.y-1;
		break;
	case SOUTH:
		x=m_OnGame.m_nBase.Pos.x+1;
		break;
	case NORTH:
		x=m_OnGame.m_nBase.Pos.x-1;
		break;
	case WN:
		y=m_OnGame.m_nBase.Pos.y-1;
		x=m_OnGame.m_nBase.Pos.x-1;
		break;
	case WS:
		y=m_OnGame.m_nBase.Pos.y-1;
		x=m_OnGame.m_nBase.Pos.x+1;
		break;
	case EN:
		y=m_OnGame.m_nBase.Pos.y+1;
		x=m_OnGame.m_nBase.Pos.x-1;
		break;
	case ES:
		y=m_OnGame.m_nBase.Pos.y+1;
		x=m_OnGame.m_nBase.Pos.x+1;
		break;
	default:
		break;
	}
	if(m_OnGame.map->map[x][y]=='X' || m_OnGame.map->map[x][y]=='O'){
		m_OnGame.map->map[x][y]=' ';
	}
}

void CMarineDlg::On0()
{
	m_OnGame.m_nBase.bIsRun=FALSE;
}

void CMarineDlg::On1() 
{
	m_OnGame.m_nBase.nSpeed=6;
	m_OnGame.m_nBase.bIsRun=TRUE;
//	m_OnGame.step=0;
}


void CMarineDlg::On3() 
{
	m_OnGame.m_nBase.nSpeed=2;
	m_OnGame.m_nBase.bIsRun=TRUE;
//	m_OnGame.step=0;
}

void CMarineDlg::On2() 
{
	m_OnGame.m_nBase.nSpeed=4;
	m_OnGame.m_nBase.bIsRun=TRUE;
//	m_OnGame.step=0;
}

void CMarineDlg::Die()
{
	char* path=new char[240];
	char file[MAX_PATH];
	GetCurrentDirectory(240,path);
	strcpy(file,path);
	strcat(file,"\\user.thi");
	WritePrivateProfileString("USER","STAGE","0",file);
	KillTimer(0);
	MessageBox("战争是无情的，您已经不幸地葬身海底。\r\nGame Over!\r\n请从头再来!","永垂不朽!");
	delete path;
	CMarineDlg::EndDialog(TRUE);
	return;
}


void CMarineDlg::Win()
{
	USERINFO* UserInfo=new USERINFO;
	GetUserInfo(UserInfo);
	char* path=new char[240];
	char num[5];
	char file[MAX_PATH];
	GetCurrentDirectory(240,path);
	strcpy(file,path);
	strcat(file,"\\user.thi");
	itoa(UserInfo->stage+1,num,10);
	WritePrivateProfileString("USER","STAGE",num,file);
	KillTimer(0);
	if(UserInfo->stage==20){
		int id=MessageBox("你非常幸运的的在战争中生存了下来.你是德意志帝国的王牌艇长.\r\n按取消重新游戏.","优秀的艇长:",MB_OKCANCEL);
		if(id=IDCANCEL){
			FILE* open=fopen("user.thi","w");
			fclose(open);
		}
	}
	else if(UserInfo->stage==8){
		MessageBox("你已经完成了猎杀潜艇:狼群的任务!\n请重新启动游戏进入猎杀潜艇:帝国崛起.","优秀的艇长:");
	}
	else if(UserInfo->stage<19){
		MessageBox("你已经出色的完成了任务!\n请重新启动游戏进入下一关.","优秀的艇长:");
	}
	delete path,UserInfo;
	CMarineDlg::EndDialog(TRUE);
	return;
}

int CMarineDlg::CountEnemyNear()
{
	int c=0;
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x+1][m_OnGame.m_nBase.Pos.y+1]=='X')
		c++;
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x+1][m_OnGame.m_nBase.Pos.y]=='X')
		c++;
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x+1][m_OnGame.m_nBase.Pos.y-1]=='X')
		c++;
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x][m_OnGame.m_nBase.Pos.y+1]=='X')
		c++;
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x][m_OnGame.m_nBase.Pos.y-1]=='X')
		c++;
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x-1][m_OnGame.m_nBase.Pos.y+1]=='X')
		c++;
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x-1][m_OnGame.m_nBase.Pos.y]=='X')
		c++;
	if(m_OnGame.map->map[m_OnGame.m_nBase.Pos.x-1][m_OnGame.m_nBase.Pos.y-1]=='X')
		c++;
	return c;
}

int CMarineDlg::CountEnemy()
{
	int i,j,c=0;
	for(i=0;i<m_OnGame.map->size;i++){
		for(j=0;j<m_OnGame.map->size;j++){
			if(m_OnGame.map->map[i][j]=='X' || m_OnGame.map->map[i][j]=='O'){
				c++;
			}
		}
	}
	return c;
}
//About
BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString cs;
	cs.Format(IDS_ABOUT);
	SetDlgItemText(IDC_ABOUT,cs);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
