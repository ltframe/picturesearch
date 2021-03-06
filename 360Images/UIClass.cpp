#include "stdafx.h"

#include "UIClass.h"
#include <Psapi.h>
#include <Shellapi.h>
#include "resource.h"
#define WM_TRY (WM_USER+101)
#define IDI_TRAY 0
#define	IDM_SHOW 1000
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

UIClass::UIClass(void)
{	
	hModule = ::GetModuleHandle(0);
	applicationpath  = (wchar_t*)malloc((MAX_PATH+1)*sizeof(wchar_t)); 
	GetModuleFileName(NULL, applicationpath, MAX_PATH);
	(wcsrchr(applicationpath, '\\'))[1] = 0;
	int ww = GetSystemMetrics(SM_CXSCREEN),wh=GetSystemMetrics(SM_CYSCREEN);
	OleInitialize(0);
	ltskinview= new LTFrameView(this,L"360Images",NULL,WS_POPUP | WS_VISIBLE,0,0,ww,wh,0);
	SetWindowText(ltskinview->windowHandle(),L"360Images");	
	ltskinview->SetViewTransparent(true);
	ltskinview->SetAllowKeyDownAutoScroll(false);
	ltskinview->EnableDragFrameChangeSize(false);
	ShowNotifyIcon(ltskinview->windowHandle());
	wstring path = wstring(applicationpath)+L"./template/pic.html";
	ltskinview->loadFile(path.c_str());
	ltskinview->EnableDragFrameChangeSize(false);
	ltskinview->MessageLoop();
}


UIClass::~UIClass(void)
{
	OleUninitialize();
	free(applicationpath);
	delete ltskinview;
}

void UIClass::DeleteNotifyIcon(HWND hwnd)
{
	NOTIFYICONDATA nid;
	nid.hWnd=hwnd;
	nid.uID=IDI_TRAY;
	Shell_NotifyIcon(NIM_DELETE,&nid);//在托盘区添加图标
}

void UIClass::MinWindow(int t)
{
	if(!t){
		::ShowWindow(ltskinview->windowHandle(),SW_HIDE);	
		ModifyMenu(hMenu, IDM_SHOW, MF_STRING,  IDM_SHOW,	L"显示");
	}
	else{
		::ShowWindow(ltskinview->windowHandle(),SW_SHOW);
		ModifyMenu(hMenu, IDM_SHOW, MF_STRING,  IDM_SHOW,	L"隐藏");

	}

}

void UIClass::OnFinalMessage()
{
		delete this;
}
LRESULT UIClass::HandleUserMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(uMsg==WM_TRY)
	{
		switch(lParam) 
        { 
        case WM_RBUTTONDOWN: 
            { 
                POINT pt; GetCursorPos(&pt); 
                   SetForegroundWindow(hwnd); 
                   int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, hwnd, 
                     NULL); 
                if(cmd == IDM_SHOW) 
				{
					if(IsWindowVisible(hwnd))
						MinWindow(0);
					else
						MinWindow(1);
				}
				if(cmd ==IDM_ABOUT)
					DialogBox(hModule, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
				if(cmd == IDM_EXIT){
					DeleteNotifyIcon(hwnd);
                    ltskinview->CloseWindow();
				}
            } 
            break; 
        case WM_LBUTTONDBLCLK: 
			MinWindow(1);
            break; 
        } 
	}
	return ltskinview->HandleMessage(hwnd,uMsg,wParam,lParam);

}
LPCWSTR UIClass::GetWindowClassName()
{
	return L"360Images";
}

void UIClass::ShowNotifyIcon(HWND hwnd)
{
	NOTIFYICONDATA nid;
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd=hwnd;
	nid.uID=IDI_TRAY;
	nid.uFlags=NIF_MESSAGE|NIF_ICON|NIF_TIP ;
	nid.uCallbackMessage=WM_TRY;
	nid.hIcon=LoadIcon(::GetModuleHandle(0),MAKEINTRESOURCE(IDI_SMALL));
	int xxx = GetLastError();
	wcscpy(nid.szTip,_T("好搜桌面图片搜索器"));
	hMenu = CreatePopupMenu(); 
	AppendMenu(hMenu, MF_STRING, IDM_SHOW, TEXT("隐藏")); 
    AppendMenu(hMenu, MF_STRING, IDM_ABOUT, TEXT("关于")); 
    AppendMenu(hMenu, MF_STRING, IDM_EXIT, TEXT("退出")); 
	Shell_NotifyIcon(NIM_ADD,&nid);
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}