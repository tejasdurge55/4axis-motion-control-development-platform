// 4axis platformDlg.cpp : implementation file
//

#include "stdafx.h"
#include "4axis platform.h"
#include "4axis platformDlg.h"
#include "GT400.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

short home(long pos)
{
	unsigned short status=0;
	long home_pos;
	GT_ClrSts();
	short temp=status;
	GT_CaptHome();     //specify the capture mode of home capturing.
	GT_PrflT();        ///specify T-curve of current axis.
	GT_SetPos(pos);
	GT_SetVel(4);
	GT_SetAcc(1);
	GT_Update();

	GT_GetSts(&status);  ///Get axis status
	do 
	{
		//GT_GetSts(&status);   //
		Sleep(5);
		if (!(status&0x400))   ///If the motion is completed but Home is nottriggered, exit.
		{    
			if (status&0x20)        //limit switch trigger first
			{
				GT_SetPos(-pos);
				//GT_SetVel(4);
	            //GT_SetAcc(1);
				GT_Update();
				GT_ClrSts();
			}
				
			else
				return 1;
		}
	
		MSG msg;
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		GT_GetSts(&status);
	} while(!(status&0x8));    ////Wait for Home capturing.
 
	GT_SmthStp();              //axis motion stop.
	GT_Update();
	do 
	{	
		MSG msg;
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GT_GetSts(&status);   
	} while((status&0x400));    ////Wait until the motion is completed.
	
	GT_GetCapt(&home_pos);      //Get position captured.
	GT_SetPos(home_pos);        //
	GT_SetVel(2);
	GT_Update();

	do 
	{	
		MSG msg;
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GT_GetSts(&status);   
	} while(status&0x400);      ////Wait until the motion is completed.
	
	GT_ClrSts();            //clear home status
	GT_CaptIndex();         ////specify the capture mode of home capturing.
	GT_SetPos(home_pos+11000);  //axis move more than one round. PPR=10000.
	GT_Update();

	do 
	{
		GT_GetSts(&status);
		if (!(status&0x400))
		{
			return 2;
		}
		MSG msg;
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} while(!(status&0x08));     // //Wait for Index capturing.. 

	GT_SmthStp();   
	GT_Update();
	do 
	{	
		MSG msg;
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GT_GetSts(&status);   
	} while(status&0x400);      //Wait until the motion is completed.

	GT_SetPos(home_pos+500);
	GT_Update();

	do 
	{	
		MSG msg;
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		GT_GetSts(&status);   
	} while(status&0x400);      //Wait until the motion is completed.

	Sleep(20);
	GT_ZeroPos();               //Specify actual and target position of zero.
 	return 0;

	}

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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy4axisplatformDlg dialog

CMy4axisplatformDlg::CMy4axisplatformDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy4axisplatformDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy4axisplatformDlg)
	m_pos1 = 0;
	m_pos2 = 0;
	m_pos3 = 0;
	m_pos4 = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy4axisplatformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy4axisplatformDlg)
	DDX_Control(pDX, IDC_fpos3, m_fpos3);
	DDX_Control(pDX, IDC_fpos2, m_fpos2);
	DDX_Control(pDX, IDC_fpos, m_fpos);
	DDX_Text(pDX, IDC_position1, m_pos1);
	DDX_Text(pDX, IDC_position2, m_pos2);
	DDX_Text(pDX, IDC_position3, m_pos3);
	DDX_Text(pDX, IDC_position4, m_pos4);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy4axisplatformDlg, CDialog)
	//{{AFX_MSG_MAP(CMy4axisplatformDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_on, Onon)
	ON_EN_CHANGE(IDC_position1, OnChangeposition1)
	ON_BN_CLICKED(IDC_ini1, Onini1)
	ON_BN_CLICKED(IDC_run1, Onrun1)
	ON_EN_CHANGE(IDC_position2, OnChangeposition2)
	ON_BN_CLICKED(IDC_ini2, Onini2)
	ON_BN_CLICKED(IDC_run2, Onrun2)
	ON_EN_CHANGE(IDC_position3, OnChangeposition3)
	ON_BN_CLICKED(IDC_ini3, Onini3)
	ON_BN_CLICKED(IDC_run3, Onrun3)
	ON_EN_CHANGE(IDC_position4, OnChangeposition4)
	ON_BN_CLICKED(IDC_ini4, Onini4)
	ON_BN_CLICKED(IDC_run4, Onrun4)
	ON_BN_CLICKED(IDC_off, Onoff)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy4axisplatformDlg message handlers

BOOL CMy4axisplatformDlg::OnInitDialog()
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy4axisplatformDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMy4axisplatformDlg::OnPaint() 
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMy4axisplatformDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy4axisplatformDlg::Onon() 
{
	// TODO: Add your control notification handler code here
short rtn;
int i=0;
while (i<5)
{
		rtn = GT_Axis(i);
		rtn = GT_AxisOn();
		i++;
}	
}

void CMy4axisplatformDlg::OnChangeposition1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CMy4axisplatformDlg::UpdateData(TRUE);
}

void CMy4axisplatformDlg::Onini1() 
{
	// TODO: Add your control notification handler code here
	short rtn;

	rtn=GT_LmtsOn();
	rtn=GT_LmtSns(0x10);

int i =0;
while (i<5)
{
rtn=GT_Axis(i);
rtn=GT_RstSts(0xff9f);
		
	rtn = GT_Open();
	rtn = GT_Reset();
i++;

}
	rtn = GT_LmtSns(0);
	rtn=GT_SwitchtoCardNo(0); 
	rtn=GT_SetSmplTm(200);  //Set control period as 200us.

		rtn=GT_Axis(1);
		rtn=GT_SetIntrMsk(0);

    rtn=GT_Axis(1); //error(rtn); //Set the first axis as current axis.
	rtn=GT_ClrSts(); //error(rtn); //Clear the wrong status of current axis.
	rtn=GT_CtrlMode(0); //error(rtn); //Set to output analog.
	//rtn=GT_StepDir(); //error(rtn); //Set to output pulse + direction signal.
	rtn=GT_AxisOn();


		

	rtn=GT_LmtSns(0xff);
		
	
}

void CMy4axisplatformDlg::Onrun1() 
{
	// TODO: Add your control notification handler code here
int a=0;
short rtn;
char cpos[1000];
long pos;
long epos;
float degree;


	rtn=GT_Axis(1);
rtn=GT_AxisOn();



while (a<1000)
{	//rtn=GT_Axis(1);
//rtn=GT_AxisOn();
	

GT_GetAtlPos(&epos);
pos=epos;
degree=(epos*360)/2400;
sprintf(cpos,"%f",degree);//pos //"%Ld"
m_fpos.SetWindowText(cpos);	
a=a+1;


}
	
	


rtn=GT_Axis(1);
rtn=GT_SetKp(3);
rtn=GT_SetKi(2);
rtn=GT_SetKd(8);
rtn=GT_Update();
rtn=GT_AxisOn();



rtn=GT_PrflT();
rtn=GT_SetPos(m_pos1);
rtn=GT_SetAcc(0.1);
rtn=GT_SetVel(1);
rtn=GT_Update();	
}

void CMy4axisplatformDlg::OnChangeposition2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CMy4axisplatformDlg::UpdateData(TRUE);
}

void CMy4axisplatformDlg::Onini2() 
{
	// TODO: Add your control notification handler code here
	short rtn;

	rtn=GT_LmtsOn();
	rtn=GT_LmtSns(0x10);

int i =0;
while (i<5)
{
rtn=GT_Axis(i);
rtn=GT_RstSts(0xff9f);
		
	rtn = GT_Open();
	rtn = GT_Reset();
i++;

}
	rtn = GT_LmtSns(0);
	rtn=GT_SwitchtoCardNo(0); 
	rtn=GT_SetSmplTm(200);  //Set control period as 200us.

		

	rtn=GT_Axis(2);
		rtn=GT_SetIntrMsk(0);

    rtn=GT_Axis(2); //error(rtn); //Set the first axis as current axis.
	rtn=GT_ClrSts(); //error(rtn); //Clear the wrong status of current axis.
	rtn=GT_CtrlMode(0); //error(rtn); //Set to output analog.
	//rtn=GT_StepDir(); //error(rtn); //Set to output pulse + direction signal.
	rtn=GT_AxisOn();

		

	rtn=GT_LmtSns(0xff);
		
	
}

void CMy4axisplatformDlg::Onrun2() 
{
	// TODO: Add your control notification handler code here

int a=0;
short rtn;
char cpos2[1000];
long pos2;
long epos2;


	rtn=GT_Axis(2);
rtn=GT_AxisOn();



while (a<1000)
{	rtn=GT_Axis(2);
rtn=GT_AxisOn();
	

GT_GetAtlPos(&epos2);
pos2=epos2;
sprintf(cpos2,"%Ld",pos2);
m_fpos2.SetWindowText(cpos2);	
a=a+1;


}	
	
	
	

rtn=GT_Axis(2);
rtn=GT_SetKp(3);
rtn=GT_SetKi(2);
rtn=GT_SetKd(8);
rtn=GT_Update();
rtn=GT_AxisOn();
rtn=GT_PrflT();
rtn=GT_SetPos(m_pos2);
rtn=GT_SetAcc(0.1);
rtn=GT_SetVel(1);
rtn=GT_Update();
	
}

void CMy4axisplatformDlg::OnChangeposition3() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CMy4axisplatformDlg::UpdateData(TRUE);
}

void CMy4axisplatformDlg::Onini3() 
{
	// TODO: Add your control notification handler code here
	short rtn;

	rtn=GT_LmtsOn();
	rtn=GT_LmtSns(0x10);

int i =0;
while (i<5)
{
rtn=GT_Axis(i);
rtn=GT_RstSts(0xff9f);
		
	rtn = GT_Open();
	rtn = GT_Reset();
i++;

}
	rtn = GT_LmtSns(0);
	rtn=GT_SwitchtoCardNo(0); 
	rtn=GT_SetSmplTm(200);  //Set control period as 200us.

		rtn=GT_Axis(3);
		rtn=GT_SetIntrMsk(0);

    rtn=GT_Axis(3); //error(rtn); //Set the first axis as current axis.
	rtn=GT_ClrSts(); //error(rtn); //Clear the wrong status of current axis.
	rtn=GT_CtrlMode(1); //error(rtn); //Set to output PWM.
	rtn=GT_StepDir(); //error(rtn); //Set to output pulse + direction signal.
	rtn=GT_AxisOn();

	rtn=GT_LmtSns(0xff);
		
		
}

void CMy4axisplatformDlg::Onrun3() 
{
	// TODO: Add your control notification handler code here
int c=0;
short rtn;
char cpos3[1000];
long pos3;
long epos3;
float degree3;


	rtn=GT_Axis(3);
rtn=GT_AxisOn();



while (c<1000)
{	//rtn=GT_Axis(3);
//rtn=GT_AxisOn();
	

GT_GetAtlPos(&epos3);
pos3=epos3;
degree3=(epos3*360)/2400;
sprintf(cpos3,"%f",degree3);//pos //"%Ld"
m_fpos3.SetWindowText(cpos3);	
c=c+1;


}


	
		
rtn=GT_Axis(3);
rtn=GT_Update();
rtn=GT_AxisOn();
rtn=GT_PrflT();
rtn=GT_SetPos(m_pos3);
rtn=GT_SetAcc(0.1);
rtn=GT_SetVel(0.07);
rtn=GT_Update();	
}

void CMy4axisplatformDlg::OnChangeposition4() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CMy4axisplatformDlg::UpdateData(TRUE);
}

void CMy4axisplatformDlg::Onini4() 
{
	// TODO: Add your control notification handler code here
short rtn;

	rtn=GT_LmtsOn();
	rtn=GT_LmtSns(0x10);

int i =0;
while (i<5)
{
rtn=GT_Axis(i);
rtn=GT_RstSts(0xff9f);
		
	rtn = GT_Open();
	rtn = GT_Reset();
i++;

}
	rtn = GT_LmtSns(0);
	rtn=GT_SwitchtoCardNo(0); 
	rtn=GT_SetSmplTm(200);  //Set control period as 200us.

		rtn=GT_Axis(4);
		rtn=GT_SetIntrMsk(0);

    rtn=GT_Axis(4); //error(rtn); //Set the first axis as current axis.
	rtn=GT_ClrSts(); //error(rtn); //Clear the wrong status of current axis.
	rtn=GT_CtrlMode(1); //error(rtn); //Set to output PWM.
	rtn=GT_StepDir(); //error(rtn); //Set to output pulse + direction signal.
	rtn=GT_AxisOn();

	rtn=GT_LmtSns(0xff);
		
}

void CMy4axisplatformDlg::Onrun4() 
{
	// TODO: Add your control notification handler code here
short rtn;	
rtn=GT_Axis(4);
rtn=GT_Update();
rtn=GT_AxisOn();
rtn=GT_PrflT();
rtn=GT_SetPos(m_pos4);
rtn=GT_SetAcc(0.1);
rtn=GT_SetVel(0.07);
rtn=GT_Update();	
}

void CMy4axisplatformDlg::Onoff() 
{
	// TODO: Add your control notification handler code here
short rtn;
int i=0;
while (i<5)
{	
		rtn=GT_Axis(i);
		rtn=GT_AxisOff();
		i++;
}	
}
