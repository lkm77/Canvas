#include "HiEasyX.h"
#include <list>
const std::wstring wndTitle=L"绘图";

hiex::Canvas canvas;

//用于撤销
//存储需要绘制的内容
std::vector<std::vector<POINT>>revocation;
//用于恢复
//存储被撤销的内容
std::vector<std::vector<POINT>>renew;

bool isLbuttonDown = false;
bool isCtrlDown = false;
void OnPaint() {
	//canvas.Clear();
	for (auto& ti : revocation) {
		canvas.Polyline(ti.data(), ti.size(), true, RGB(0, 0, 0));
	}
}
void OnMessage(const ExMessage& me) {
	switch (me.message)
	{
	case WM_LBUTTONDOWN:
		isLbuttonDown = true;
		if (revocation.empty() || revocation.back().size())
			revocation.push_back(std::vector<POINT>());
		break;
	case WM_RBUTTONDOWN:
		canvas.Clear();
		for (auto& ti : revocation) {
			renew.push_back(std::move(ti));
		}
		revocation.clear();
		break;
	case WM_MOUSEMOVE:
	{
		if (false == isLbuttonDown)break;
		POINT po1 = { me.x,me.y };
		POINT po2 = po1;
		if (revocation.back().size())po1 = revocation.back().back();
		canvas.Line(po1, po2, true, RGB(0, 0, 0));
		revocation.back().push_back(po2);
		renew.clear();
	}
	break;
	case WM_LBUTTONUP:
		isLbuttonDown = false;
		break;
	case WM_KEYDOWN:
		//判断是否是Ctrl键
		if (VK_CONTROL == me.vkcode)
			isCtrlDown = true;
		break;
	case WM_KEYUP:
		//判断是否是Ctrl键
		if (VK_CONTROL == me.vkcode)
			isCtrlDown = false;
		break;
	case WM_CHAR: {
		//处理Ctrl+z键(撤销)
		if ((TCHAR)26 == me.ch && isCtrlDown) {
			if (revocation.size()) {
				canvas.Clear();
				renew.push_back(std::move(revocation.back()));
				revocation.pop_back();
				OnPaint();
			}
		}
		//处理Ctrl+y键(恢复)
		if ((TCHAR)25 == me.ch && isCtrlDown) {
			if (renew.size()) {
				revocation.push_back(std::move(renew.back()));
				renew.pop_back();
				OnPaint();
			}
		}
		break;
	}

	default:
		break;
	}
}
int main() {
	hiex::Window wnd(1080, 960, 0, wndTitle.c_str());
	wnd.BindCanvas(&canvas);
	while (wnd.IsAlive())
	{
		if (wnd.IsSizeChanged()) {
			OnPaint();
		}
		if (wnd.IsSizeChanged()) {
			OnPaint();
		}
		ExMessage me;
		if (wnd.Peek_Message(&me)) {
			OnMessage(me);
		}
		hiex::DelayFPS(120);
		wnd.Redraw();
	}
}