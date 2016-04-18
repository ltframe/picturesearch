#include "stdafx.h"
#include "InterFace.h"
#include <string>
using namespace std; 

class UIClass : public I_LTFRAME_IWebView
{
public:
	UIClass(void);
	~UIClass(void);
	virtual LRESULT HandleUserMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
	virtual LPCWSTR GetWindowClassName();
	virtual void OnFinalMessage();
	void ShowNotifyIcon(HWND hwnd);
	void MinWindow(int);
	void DeleteNotifyIcon(HWND hwnd);
private:
    HMENU hMenu ;
	LTFrameView* ltskinview;
	wchar_t* applicationpath;
	HMODULE hModule;
};