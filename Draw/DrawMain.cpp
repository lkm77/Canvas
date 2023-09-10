#include "HiEasyX.h"
#include "DrawFile.h"
#include "DrawFactory.h"
#include "resource.h"
const std::wstring wndTitle=L"绘图";

hiex::Window* drawWndPointer;
hiex::Canvas canvas;

//用于撤销
//存储需要绘制的内容
std::vector<Draw::DrawBaseUPtr> revocation;
//用于恢复
//存储被撤销的内容
std::vector<Draw::DrawBaseUPtr> renew;
//用于存储背景
Draw::DrawImageUPtr backgroundImage(new Draw::DrawImage);
DrawFile drFi;
bool isLbuttonDown = false;
void OnPaint() {
	//绘图画布
	hiex::Canvas paintCanvas(canvas.GetWidth(), canvas.GetHeight(), canvas.GetBkColor());
	paintCanvas.BeginBatchDrawing();
	if (backgroundImage)backgroundImage->ReDraw(paintCanvas);
	for (auto& ti : revocation) {
		ti->ReDraw(paintCanvas);
	}
	canvas.PutImageIn_Alpha(
		0, 0,
		paintCanvas.GetImagePointer()
	);
	paintCanvas.EndBatchDrawing();
	if(drawWndPointer)drawWndPointer->Redraw();
}
void OnMenu(WPARAM wParam) {
	switch (LOWORD(wParam))
	{
	case ID_NewBuilt: {
		if (MessageBox(NULL, L"是否新建,未保存内容将丢失", L"新建", MB_YESNO| MB_ICONWARNING) == IDYES) {
			canvas.Clear();
			revocation.clear();
			renew.clear();
			backgroundImage->RemoveImage();
		}
		break;
	}
	case ID_Open: {
		if (MessageBox(NULL, L"是否打开,未保存内容将丢失", L"打开", MB_YESNO| MB_ICONWARNING) == IDYES) {
			canvas.Clear();
			revocation.clear();
			renew.clear();
			//backgroundImage->RemoveImage();
			drFi.Load(*drawWndPointer, backgroundImage);
			OnPaint();
		}
		break;
	}
	case ID_Save: {
		//处理Ctrl+s键(保存)
		drFi.Save(*drawWndPointer,false);
		break;
	}
	case ID_SaveAs: {
		drFi.Save(*drawWndPointer, true);
		break;
	}
	case ID_Redo:
		//处理Ctrl+y键(恢复)
		if (renew.size()) {
		revocation.push_back(std::move(renew.back()));
		renew.pop_back();
		OnPaint();
		}
		break;
	case ID_Revoke:
		//处理Ctrl+z键(撤销)
		if (revocation.size()) {
		canvas.Clear();
		renew.push_back(std::move(revocation.back()));
		revocation.pop_back();
		OnPaint();
		}
		break;
	case ID_About: {
		std::wstring strDrVe = L"版本:";
		strDrVe += DrawVersion;
		MessageBox(NULL, strDrVe.c_str(), L"关于", MB_OK);
	}
	default:
		break;
	}
}
void OnDropFiles(HWND hwnd, HDROP hDropInfo) {
	UINT  nFileCount = DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	TCHAR szFileName[_MAX_PATH] = _T("");
	DWORD dwAttribute;

	// 获取拖拽进来文件和文件夹
	for (UINT i = 0; i < nFileCount; i++)
	{
		DragQueryFile(hDropInfo, i, szFileName, sizeof(szFileName));
		dwAttribute = ::GetFileAttributes(szFileName);

		// 是否为文件夹
		if (dwAttribute & FILE_ATTRIBUTE_DIRECTORY)
		{
			SetCurrentDirectory(szFileName);
			//EnumerateFiles();
		}
		else
		{
			drFi.Load(szFileName, backgroundImage);
		}
	}

	::DragFinish(hDropInfo);
}
//加载加速键
HACCEL haccel = LoadAccelerators(GetModuleHandle(NULL), (wchar_t*)IDR_ACCELERATOR1);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//if(msg!=WM_COMMAND)
	MSG Msg{};
	Msg.hwnd = hWnd;
	Msg.message = msg;
	Msg.wParam = wParam;
	Msg.lParam = lParam;
	//翻译加速键
	//如果为加速键直接默认处理
	if(TranslateAccelerator(hWnd, haccel, &Msg))return HIWINDOW_DEFAULT_PROC;
	switch (msg)
	{
	case WM_COMMAND:
		OnMenu(wParam);
		break;
	case WM_LBUTTONDOWN:
	{
		isLbuttonDown = true;
		if (revocation.empty());
		else if (revocation.back()->IsDraw());
		else break;
		revocation.push_back(Draw::DrawFactory(Draw::Line));
		break;
	}
	case WM_RBUTTONDOWN:
		canvas.Clear();
		revocation.clear();
		renew.clear();
		if (drawWndPointer)drawWndPointer->Redraw();
		break;
	case WM_MOUSEMOVE:
	{
		if (false == isLbuttonDown)break;
		POINT po = { GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam) };
		if (revocation.back()->GetDrawType() == Draw::Line) {
			Draw::DrawLine* drLi = (Draw::DrawLine*)revocation.back().get();
			drLi->AddPoint(canvas, po);
		}
		drawWndPointer->Redraw();
		renew.clear();
		break;
	}
	case WM_LBUTTONUP:
		isLbuttonDown = false;
		break;
	case WM_SIZE:
		OnPaint();
		break;

	case WM_DROPFILES:
		if (MessageBox(NULL, L"是否打开,未保存内容将丢失", L"打开", MB_YESNO | MB_ICONWARNING) == IDYES) {
			canvas.Clear();
			revocation.clear();
			renew.clear();
			OnDropFiles(hWnd, (HDROP)wParam);
			OnPaint();
		}
		break;
	default:
		return HIWINDOW_DEFAULT_PROC;	// 标识使用默认消息处理函数继续处理
		break;
	}
	hiex::DelayFPS(120);
	return 0;
}
int main() {
	hiex::Window drawWnd;
	drawWnd.PreSetStyleEx(WS_EX_ACCEPTFILES);
	drawWnd.Create(1080, 960, 0, wndTitle.c_str(), WndProc);
	//设置菜单
	SetMenu(drawWnd.GetHandle(), LoadMenu(GetModuleHandle(NULL), (wchar_t*)IDR_MENU1));
	drawWndPointer = &drawWnd;
	drawWnd.BindCanvas(&canvas);
	while (drawWnd.IsAlive())
	{
		//drawWnd.Redraw();
		hiex::DelayFPS(1,true);
	}
}