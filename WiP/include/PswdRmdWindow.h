#pragma once

#include "Buttons.h"
#include <string>
#include <iostream>

class PswdRmdWindow : public MyWindowBase
{
	using Base = MyWindowBase;
public:
	static const wchar_t* s_className;

	void create();

	enum class ChildId {
		None,
		CenterButton,
		LeftButton,
		RightButton,
		Timer,
	};

	static const int TimerId = 1003;

	void ShowWin(bool istrue);

private:
	virtual void OnDestroy() override;
	virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

	void OnPaint(PAINTSTRUCT& ps);
	void OnCommand(WPARAM wParam, LPARAM lParam);

	bool my_IsDlgButtonChecked(ChildId id) { return IsDlgButtonChecked(hwnd(), static_cast<int>(id)); }
	void my_CheckDlgButton(ChildId id, bool checked) { CheckDlgButton(hwnd(), static_cast<int>(id), checked ? BST_CHECKED : BST_UNCHECKED); }

	HWND hwnd_PswdRmdWin_EditLogin = nullptr;

	Buttons m_centralButton;
	Buttons m_leftButton;
	Buttons m_rightButton;

	std::wstring m_red_allert = L"";

	HFONT m_font = nullptr;
	HBRUSH m_brush = nullptr;

	const int windx = 600;
	const int windy = 400;
	const int button_size_x = 175;
	const int button_size_y = 25;
	const int red_allert_size_x = 350;
};