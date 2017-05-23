/////////////////////////////////////////////////////////////////////////////////////////

#include <winsock2.h>
#include <windows.h>
#include <process.h>
#include <stdio.h>
#include "resource.h"

//=======================================================================================

#pragma comment(lib, "ws2_32.lib")

//=======================================================================================

#define SIZE_BUFF 1024
#define SIZE_STR 32

//=======================================================================================

HWND hMain = NULL;
HWND hAuth = NULL;
HINSTANCE hInst = NULL;
SOCKET Socket = INVALID_SOCKET;
char IPServer[SIZE_STR] = "";
char RoomName[SIZE_STR] = "";
char ClientName[SIZE_STR] = "";
char Mess[SIZE_BUFF] = "";
BOOL IsNoAuth = FALSE;

//=======================================================================================

LRESULT CALLBACK	DlgProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	AuthDlgProc(HWND, UINT, WPARAM, LPARAM);
void				RecvThread(void*);

//=======================================================================================

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	hInst = hInstance;

	hMain = (HWND)DialogBox(hInst, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)DlgProc);
	return 0;
}

//---------------------------------------------------------------------------------------

LRESULT CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	WSADATA wsaData;
	int iResult;
	DWORD wD;
	sockaddr_in ServerAddr;
	char tmp[SIZE_BUFF] = "";
	ULONG Par;

	switch (message)
	{
	case WM_INITDIALOG:
		iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
		Socket = socket(AF_INET, SOCK_STREAM, 0);
		SetDlgItemText(hDlg, IDC_EDIT3, "127.0.0.1");
		return TRUE;
	case WM_COMMAND:
		wD = LOWORD(wParam);
		switch(wD)
		{
		case IDC_CONNECT:
			if(IsNoAuth)
			{
				iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
				Socket = socket(AF_INET, SOCK_STREAM, 0);
				IsNoAuth = FALSE;
			}

			GetDlgItemText(hDlg, IDC_EDIT3, IPServer, SIZE_STR);

			ServerAddr.sin_family = AF_INET;
			ServerAddr.sin_port = htons(55555);
			ServerAddr.sin_addr.s_addr = inet_addr(IPServer);

			connect(Socket, (sockaddr*)&ServerAddr, sizeof(ServerAddr));
			send(Socket, "connect", 8, 0);
			recv(Socket, tmp, SIZE_BUFF, 0);
			SetDlgItemText(hDlg, IDC_EDIT2, tmp);
			hAuth = (HWND)DialogBox(hInst, MAKEINTRESOURCE(IDD_AUTH), (HWND)NULL, (DLGPROC)AuthDlgProc);
			sprintf(tmp, "auth %s %s", RoomName, ClientName);
			send(Socket, tmp, lstrlen(tmp) + 1, 0);
			recv(Socket, tmp, SIZE_BUFF, 0);
			GetDlgItemText(hDlg, IDC_EDIT2, Mess, SIZE_BUFF);
			lstrcat(Mess, "\r\n");
			lstrcat(Mess, tmp);
			SetDlgItemText(hDlg, IDC_EDIT2, Mess);
			lstrcpyn(tmp, tmp, 4);
			if(!lstrcmp(tmp, "ERR"))
			{
				IsNoAuth = TRUE;
				closesocket(Socket);
				WSACleanup();
				break;
			}
			sprintf(tmp, "×àò - %s", ClientName);
			SetWindowText(hDlg, tmp);
			hMain = hDlg;
			
			_beginthread(RecvThread, 0, NULL);
			break;
		case IDC_SEND:
			GetDlgItemText(hDlg, IDC_EDIT1, tmp, SIZE_BUFF);
			send(Socket, tmp, lstrlen(tmp), 0);
			SetDlgItemText(hDlg, IDC_EDIT1, "");
			break;
		case IDC_DISCONNECT:
			send(Socket, "end", 4, 0);
			closesocket(Socket);
			WSACleanup();
			IsNoAuth = TRUE;
			break;
		}
		return TRUE;
	case WM_CLOSE:
		WSACleanup();
		EndDialog(hDlg, 0);
		return TRUE;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------

LRESULT CALLBACK AuthDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			GetDlgItemText(hDlg, IDC_EDIT2, RoomName, SIZE_STR);
			GetDlgItemText(hDlg, IDC_EDIT3, ClientName, SIZE_STR);
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		else if(LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//---------------------------------------------------------------------------------------

void RecvThread(void* n)
{
	char tmp[SIZE_BUFF] = "";
	char tmp2[32] = "";
	int p = 0;

	while(TRUE)
			{
				int iResult = recv(Socket, tmp, SIZE_BUFF, 0);
				if(iResult == SOCKET_ERROR)
				{
					closesocket(Socket);
					_endthread();
					return;
				}
				GetDlgItemText(hMain, IDC_EDIT2, Mess, SIZE_BUFF);
				lstrcat(Mess, "\r\n");
				lstrcat(Mess, tmp);
				SetDlgItemText(hMain, IDC_EDIT2, Mess);
				p = SendMessage(GetDlgItem(hMain, IDC_EDIT2), EM_GETLINECOUNT, 0, 0);
				SendMessage(GetDlgItem(hMain, IDC_EDIT2), EM_LINESCROLL, NULL, p - 1); 
				
			}
}
/////////////////////////////////////////////////////////////////////////////////////////
