#include "HiEasyX.h"
#include "DrawSave.h"
const std::wstring wndTitle=L"绘图";

hiex::Window* drawWndPointer;
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
void OnChar(TCHAR ch) {
	switch (ch)
	{
		case (TCHAR)('z'-96) :
		//处理Ctrl+z键(撤销)
		if (isCtrlDown && revocation.size()) {
			canvas.Clear();
			renew.push_back(std::move(revocation.back()));
			revocation.pop_back();
			OnPaint();
		}
		break;
	case (TCHAR)('y' - 96) :
		//处理Ctrl+y键(恢复)
		if (isCtrlDown && renew.size()) {
			revocation.push_back(std::move(renew.back()));
			renew.pop_back();
			OnPaint();
		}
		break;
		case (TCHAR)('s' - 96) :{
			//处理Ctrl+s键(保存)
			hiex::Window saveWnd;
			saveWnd.PreSetPos(
				(drawWndPointer->GetPos().x + drawWndPointer->GetClientWidth() - 100) / 2,
				(drawWndPointer->GetPos().y + drawWndPointer->GetClientHeight() - 200) / 2
			);
			saveWnd.InitWindow(200, 100, 0, L"保存", nullptr, drawWndPointer->GetHandle());
			hiex::Canvas saveCanvas;
			saveWnd.BindCanvas(&saveCanvas);
			EnableSystemMenu(saveWnd.GetHandle(), false);
			EnableResizing(saveWnd.GetHandle(), false);
			DrawSave drawSave;
			drawSave.Save(L"save.txt", revocation);
			while (drawSave.IsSave()) {
				saveCanvas.CenterText(L"保存中,请稍后...");
				saveWnd.Redraw();
				hiex::DelayFPS(60);
			}
			saveWnd.CloseWindow();
		}
			break;
	default:
		break;
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
		OnChar(me.ch);
		break;
	}

	default:
		break;
	}
}
int main() {
	hiex::Window drawWnd(1080, 960, 0, wndTitle.c_str());
	drawWndPointer = &drawWnd;
	drawWnd.BindCanvas(&canvas);
	while (drawWnd.IsAlive())
	{
		if (drawWnd.IsSizeChanged()) {
			OnPaint();
		}
		ExMessage me;
		if (drawWnd.Peek_Message(&me)) {
			OnMessage(me);
		}
		drawWnd.Redraw();
		hiex::DelayFPS(120);
	}
}