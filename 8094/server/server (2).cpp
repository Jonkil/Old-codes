// server00.cpp
//

#include "stdafx.h"
#include "server.h"
#include <winsock2.h>
#include "rooms.h"

#define MAX_LOADSTRING 100



#pragma comment(lib, "ws2_32.lib")

#define PORT 55555
#define MAX_CONNECTS 15 
#define MAX_SIZE_ROOMS 1024
#define SIZE_BUFF 1024
#define MESS_BUFF (SIZE_BUFF * 10)

//=======================================================================================



//=======================================================================================

BOOL InitServer();
BOOL RunServer();
void ThreadProc(void *client);
void AcceptProc(void *n);
void ErrorApp();

//=======================================================================================
// Глобальные переменные:
HINSTANCE hInst;								
TCHAR szTitle[MAX_LOADSTRING];					
TCHAR szWindowClass[MAX_LOADSTRING];			
SOCKET ServerSocket;						// сокет сервера
SOCKADDR_IN LocalAddr;						// адрес сервера
char mess[MESS_BUFF];					// буффер для сообщений
HWND hMemo = NULL;							// handle поля сообщений
HWND hInfo = NULL;							// handle поля для информационных сообщений
Room **Rooms = NULL;						// массив указателей на комнаты
int NumRooms = 0;							// счётчик комнат
CRITICAL_SECTION csRooms;					// критическая секция для работы с массивом
											// комнат
CRITICAL_SECTION csWin;						// критическая секция для работы с главным
											// потоком

//=======================================================================================
// Объявления функций

ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	RoomList(HWND, UINT, WPARAM, LPARAM);
BOOL				InitServer();				// инициализация сервера
BOOL				RunServer();				// запуск сервера
void				ThreadProc(void *client);	// функция потока клиента
void				AcceptProc(void *n);		// функция потока сокета
void				ErrorApp();					// функция обработки ошибок

//=======================================================================================

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	
	MSG msg;
	HACCEL hAccelTable;

	// Инициализация строк:
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SERVER00, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Иницициализация приложения:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_SERVER00);

	// Главный цикл сообщений:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}

//---------------------------------------------------------------------------------------

//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрация класса окна.
//
//  КОММЕНТАРИЙ:
//
//    Эта функция используется только если Вы хотите чтобы приложение было совместимо
//    с системами Win32.

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_SERVER00);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_SERVER00;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//---------------------------------------------------------------------------------------

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; 

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);
	
	Rooms = (Room**)VirtualAlloc(NULL, MAX_SIZE_ROOMS, MEM_COMMIT, PAGE_READWRITE);
	ZeroMemory(Rooms, MAX_SIZE_ROOMS);
	
	InitializeCriticalSection(&csRooms);
	InitializeCriticalSection(&csWin);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//--------------------------------------------------------------------------------------

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent, i;
	PAINTSTRUCT ps;
	HDC hdc;
	RECT cr;
	BOOL ok;

	switch (message) 
	{
	case WM_CREATE:
		GetClientRect(hWnd, &cr);
		hMemo = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "", WS_CHILD |  WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL |
                    ES_READONLY | WS_VSCROLL ,cr.left, cr.top, cr.right, 
					cr.bottom - cr.bottom / 4, hWnd, (HMENU)IDC_MEMO, hInst, NULL);
		hInfo = CreateWindowEx(WS_EX_CLIENTEDGE, "edit", "", WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | WS_VSCROLL | ES_AUTOVSCROLL,
			cr.left, cr.top + (cr.bottom - cr.bottom / 4) + 2, cr.right, cr.bottom / 4 - 2, hWnd, 
			(HMENU)IDC_INFO, hInst, NULL);
		 ok = InitServer();
		 if(ok)
			 SetWindowText(hInfo, TEXT("Инициализация сервера прошла успешно.\r\n"));
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 
		
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_LISTEN:
			GetWindowText(hInfo, mess, SIZE_BUFF);
			sprintf(mess, "%sЗапуск сервера(Порт: %d)...\r\n", mess, PORT);
			SetWindowText(hInfo, mess);
			lstrcpy(mess, "Сообщения пользователей\r\n\r\n");
			SetWindowText(hMemo, mess);
			ok = RunServer();			
			break;
		case IDM_ROOMLIST:
			DialogBox(hInst, (LPCTSTR)IDD_ROOMLIST, hWnd, (DLGPROC)RoomList);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_SIZE:
		GetClientRect(hWnd, &cr);
		MoveWindow(hMemo, 0, 0, cr.right, cr.bottom - cr.bottom / 4, TRUE);
		MoveWindow(hInfo, 0, (cr.bottom - cr.bottom / 4) + 2, cr.right, cr.bottom / 4 - 2, TRUE);
		break;
	case WM_VSCROLL:
		SendMessage((HWND)lParam, EM_SCROLLCARET, 0, 0);
		break;

	case WM_DESTROY:
		DeleteCriticalSection(&csRooms);
		DeleteCriticalSection(&csWin);
		for(i = 0; i < NumRooms; i++)
			delete Rooms[i];
		VirtualFree(Rooms, SIZE_BUFF, MEM_DECOMMIT);
		WSACleanup();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// --------------------------------------------------------------------------------------

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

//-----------------------------------------------------------------------------------------

LRESULT CALLBACK RoomList(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char buf[SIZE_BUFF] = "";
	int i, j;

	switch (message)
	{
	case WM_INITDIALOG:
		EnterCriticalSection(&csRooms);
		if(NumRooms == 0)
			lstrcpy(buf, "Нет открытых комнат.\r\n");
		else
		{
			for(i = 0; i < NumRooms; i++)
			{
				lstrcat(buf, "Комната \"");
				lstrcat(buf, Rooms[i]->Name);
				lstrcat(buf, "\"\r\n");
				for(j = 0; j < Rooms[i]->Count; j++)
				{
					lstrcat(buf, "\t");
					lstrcat(buf, Rooms[i]->List[j].Name);
					lstrcat(buf, "\r\n");
				}
			}
		}
		LeaveCriticalSection(&csRooms);

		SetDlgItemText(hDlg, IDC_EDIT1, buf);
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////

BOOL InitServer()
{
	WSADATA WSAData; 
	if (WSAStartup (MAKEWORD(2,1), &WSAData) != 0) 
	{ 
		ErrorApp();
		return FALSE; 
	} 

	ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(ServerSocket == INVALID_SOCKET)
	{
		ErrorApp();
		WSACleanup();
		return FALSE;
	}
	return TRUE;
}

//-----------------------------------------------------------------------------------------

BOOL RunServer()
{	
	LocalAddr.sin_family = AF_INET;
	LocalAddr.sin_port = htons(PORT);
	LocalAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind (ServerSocket, (sockaddr *)&LocalAddr, sizeof(LocalAddr)) == SOCKET_ERROR) 
	{ 
		ErrorApp();
		closesocket(ServerSocket); 
		WSACleanup();
		return FALSE; 
	}

	if (listen (ServerSocket, MAX_CONNECTS) == SOCKET_ERROR) 
	{ 
		ErrorApp();
		closesocket (ServerSocket); 
		WSACleanup();
		return FALSE; 
	} 
 
		_beginthread(AcceptProc, 0, NULL);

		
	
	
	return TRUE;
}
//---------------------------------------------------------------------------------------------

void ThreadProc(void *client)
{
	char tmp[MAX_LOADSTRING];
	char tmp1[SIZE_BUFF];
	char s[MAX_LOADSTRING];
	char szServer[SIZE_BUFF];
	int n, i;
	Room *CurRoom = NULL;
	User cl = *(User*)client;
	char date[11];
	char time[9];
	SYSTEMTIME st;

	while(TRUE)
		{
			ZeroMemory(szServer, SIZE_BUFF);
			int n = recv(cl.Socket, szServer, SIZE_BUFF, 0);
			if(n == -1)
			{
				ErrorApp();
				closesocket(cl.Socket);
				return;
			}
			
			if(!lstrcmp(szServer, "connect"))
			{
				ZeroMemory(szServer, SIZE_BUFF);
				lstrcpy(szServer, "Список открытых комнат:\r\n");

				EnterCriticalSection(&csRooms);
				if(NumRooms == 0)
				{
					lstrcat(szServer, "Нет открытых комнат.\r\n");
				}
				else
				{
					for(i = 0; i < NumRooms; i++)
					{
						lstrcat(szServer, _itoa(i + 1, tmp, 10));
						lstrcat(szServer, ". ");
						lstrcat(szServer, Rooms[i]->Name);
						lstrcat(szServer, "\r\n");
					}
				}
				LeaveCriticalSection(&csRooms);

				n = send(cl.Socket, szServer, SIZE_BUFF, 0);
				if(n == SOCKET_ERROR)
					ErrorApp();
				continue;
			}
			
			if(!lstrcmp(szServer, "end"))
			{
				if(!CurRoom)
				{
					send(cl.Socket, "Не верная команда!", 19, 0);
					closesocket(cl.Socket);
					_endthread();
					return;
				}
				GetLocalTime(&st);
				GetTimeFormat(LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &st, "HH:mm:ss", time, 9);
				GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &st, "dd.MM.yyyy", date, 11);
				sprintf(tmp, "%s - %s Пользователь \"%s\"  из комнаты \"%s\" закрыл соединение.\r\n", date, time, cl.Name, CurRoom->Name);

				EnterCriticalSection(&csWin);
				GetWindowText(hInfo, tmp1, SIZE_BUFF);
				lstrcat(tmp1, tmp);
				SetWindowText(hInfo, tmp1);
				 n = SendMessage(hInfo, EM_GETLINECOUNT, 0, 0);
				 SendMessage(hInfo, EM_LINESCROLL, 0, n - 1); 
				LeaveCriticalSection(&csWin);

				EnterCriticalSection(&csRooms);
				CurRoom->Del(cl.Name);
				if(CurRoom->Count == 0)
				{
					delete CurRoom;
					NumRooms--;
				}
				LeaveCriticalSection(&csRooms);
				
				closesocket(cl.Socket);
				_endthread();
				return;
			}
			else
			{
				n = strcspn(szServer, " ");
				ZeroMemory(s, MAX_LOADSTRING);
				lstrcpyn(s, szServer, n + 1);
			}

			if(!strcmp(s, "auth"))
			{
				ZeroMemory(tmp, MAX_LOADSTRING);
				lstrcpy(tmp, (LPCTSTR)((int)szServer + n + 1));
				n = strcspn(tmp, " ");
				ZeroMemory(s, MAX_LOADSTRING);
				lstrcpyn(s, tmp, n + 1);
				lstrcpy(tmp, (LPCTSTR)((int)tmp + n + 1));
				lstrcpyn(cl.Name, tmp, 31);
				ZeroMemory(szServer, SIZE_BUFF);
				i = 0;

				EnterCriticalSection(&csRooms);
				if(NumRooms != 0)
				{
					for(i = 0; i < NumRooms; i++)
						if(!strcmp(Rooms[i]->Name, s))
						{
							for(int j = 0; j < Rooms[i]->Count; j++)
							{
								if(!lstrcmp(Rooms[i]->List[j].Name, cl.Name))
								{
									ZeroMemory(szServer, SIZE_BUFF);
									sprintf(szServer, "ERR: пользователь \"%s\" уже существует в комнате \"%s\".\r\n", cl.Name, Rooms[i]->Name);
									LeaveCriticalSection(&csRooms);
									n = send(cl.Socket, szServer, SIZE_BUFF, 0);
									if(n == SOCKET_ERROR)
										ErrorApp();
									closesocket(cl.Socket);
									_endthread();
									return;
								}
							}
							Rooms[i]->Add(cl);
							CurRoom = Rooms[i];
							sprintf(szServer, "OK: %s Вы зарегистрированны. Приятного общения.\r\n", cl.Name);
							n = send(cl.Socket, szServer, SIZE_BUFF, 0);
							if(n == SOCKET_ERROR)
								ErrorApp();
							break;
						}
				}
				if(i == NumRooms)
				{
					Rooms[NumRooms++] = new Room;
					Rooms[i]->Count = 0;
					lstrcpyn(Rooms[i]->Name, s, 31);
					Rooms[i]->Add(cl);
					CurRoom = Rooms[i];
					sprintf(szServer, "OK: %s Вы зарегистрированны. Приятного общения.\r\n", cl.Name);
					n = send(cl.Socket, szServer, SIZE_BUFF, 0);
					if(n == SOCKET_ERROR)
						ErrorApp();
				}
				LeaveCriticalSection(&csRooms);

				GetLocalTime(&st);
				GetTimeFormat(LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &st, "HH:mm:ss", time, 9);
				GetDateFormat(LOCALE_USER_DEFAULT, LOCALE_USE_CP_ACP, &st, "dd.MM.yyyy", date, 11);
				sprintf(tmp, "%s - %s Подключен новый пользователь\r\nИмя: %s\r\nКомната: %s\r\n", date, time, cl.Name, CurRoom->Name);

				EnterCriticalSection(&csWin);
				GetWindowText(hInfo, tmp1, SIZE_BUFF);
				lstrcat(tmp1, tmp);
				SetWindowText(hInfo, tmp1);
				n = SendMessage(hInfo, EM_GETLINECOUNT, 0, 0);
				 SendMessage(hInfo, EM_LINESCROLL, 0, n - 1); 
				LeaveCriticalSection(&csWin);

				continue;
			}
			if(!CurRoom)
			{
				send(cl.Socket, "Не верная команда!", 19, 0);
				closesocket(cl.Socket);
				_endthread();
				return;
			}
			sprintf(tmp, "%s::%s: %s", CurRoom->Name, cl.Name, szServer);

			EnterCriticalSection(&csRooms);
			for(i = 0; i < CurRoom->Count; i++)
			{
				n = send(CurRoom->List[i].Socket, tmp, SIZE_BUFF, 0);
				if(n == SOCKET_ERROR)
					ErrorApp();
			}
			LeaveCriticalSection(&csRooms);

			EnterCriticalSection(&csWin);
			if(((lstrlen(mess)) + (lstrlen(tmp))) >= (MESS_BUFF - 3))
				ZeroMemory(mess, MESS_BUFF);
			lstrcat(tmp, "\r\n");
			lstrcat(mess, tmp);
			SetWindowText(hMemo, mess);
			n = SendMessage(hMemo, EM_GETLINECOUNT, 0, 0);
			SendMessage(hMemo, EM_LINESCROLL, 0, n - 1); 
			LeaveCriticalSection(&csWin);
		}
		_endthread();
}

//----------------------------------------------------------------------------------------

void ErrorApp()
{
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	MessageBox( NULL, (LPSTR)lpMsgBuf, "Ошибка", MB_OK|MB_ICONINFORMATION );

	LocalFree( lpMsgBuf );
}

//----------------------------------------------------------------------------------------

void AcceptProc(void *n)
{
	User Client;
	int accept_sin_len = sizeof (Client.Addres);

	while(TRUE)
	{
		Client.Socket = accept (ServerSocket, (sockaddr *)&Client.Addres, &accept_sin_len); 


		if (Client.Socket == INVALID_SOCKET) 
		{ 
			ErrorApp();
			break; 
		}
		lstrcpy(Client.Name, "");
		_beginthread(ThreadProc, 0, (void*)&Client);
	}
	closesocket (ServerSocket);
	_endthread();
}
//////////////////////////////////////////////////////////////////////////////////////////