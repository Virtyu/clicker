#include "util.hpp"

HHOOK hhk_mouse;

LRESULT __stdcall util::hooking::hh_mouse_callback( int nCode, WPARAM wParam, LPARAM lParam )
{
	MSLLHOOKSTRUCT *hook = ( MSLLHOOKSTRUCT * ) lParam;

	if ( ( hook->flags == LLMHF_INJECTED ) || ( hook->flags == LLMHF_LOWER_IL_INJECTED ) )
		return false;

	if ( ( hook->flags & LLMHF_INJECTED ) == LLMHF_INJECTED )
		return false;

	if ( wParam != WM_MOUSEMOVE )
	{
		if ( ( hook->flags == LLMHF_INJECTED ) || ( hook->flags == LLMHF_LOWER_IL_INJECTED ) )
			return false;

		switch ( wParam )
		{
			case WM_LBUTTONDOWN:
				vars::b_first_click = true;
				vars::b_mouse_down = true;
				break;
			case WM_LBUTTONUP:
				vars::b_mouse_down = false;
				break;
		}
	}

	return CallNextHookEx( hhk_mouse, nCode, wParam, lParam );
}

DWORD __stdcall util::hooking::m_hook_all( )
{
	hhk_mouse = SetWindowsHookEx( WH_MOUSE_LL, &util::hooking::hh_mouse_callback, nullptr, 0 );

	MSG lpMsg;
	while ( GetMessage( &lpMsg, nullptr, 0, 0 ) )
	{
		TranslateMessage( &lpMsg );
		DispatchMessage( &lpMsg );
	}

	UnhookWindowsHookEx( hhk_mouse );

	return EXIT_SUCCESS;
}

void util::input::left_down( )
{
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput( 1, &Input, sizeof( INPUT ) );
}

void util::input::left_up( )
{
	INPUT Input = { 0 };
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput( 1, &Input, sizeof( INPUT ) );
}

bool util::is_focused( )
{
	return GetActiveWindow( );
}