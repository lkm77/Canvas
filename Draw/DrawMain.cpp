#include "HiEasyX.h"
#include "DrawFile.h"
#include "DrawFactory.h"
#include "resource.h"
const std::wstring wndTitle=L"��ͼ";

hiex::Window* drawWndPointer;
hiex::Canvas canvas;

//���ڳ���
//�洢��Ҫ���Ƶ�����
std::vector<Draw::DrawBaseUPtr> revocation;
//���ڻָ�
//�洢������������
std::vector<Draw::DrawBaseUPtr> renew;
//���ڴ洢����
Draw::DrawImageUPtr backgroundImage(new Draw::DrawImage);
DrawFile drFi;
bool isLbuttonDown = false;
void OnPaint() {
	//��ͼ����
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
		if (MessageBox(NULL, L"�Ƿ��½�,δ�������ݽ���ʧ", L"�½�", MB_YESNO| MB_ICONWARNING) == IDYES) {
			canvas.Clear();
			revocation.clear();
			renew.clear();
			backgroundImage->RemoveImage();
		}
		break;
	}
	case ID_Open: {
		if (MessageBox(NULL, L"�Ƿ��,δ�������ݽ���ʧ", L"��", MB_YESNO| MB_ICONWARNING) == IDYES) {
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
		//����Ctrl+s��(����)
		drFi.Save(*drawWndPointer,false);
		break;
	}
	case ID_SaveAs: {
		drFi.Save(*drawWndPointer, true);
		break;
	}
	case ID_Redo:
		//����Ctrl+y��(�ָ�)
		if (renew.size()) {
		revocation.push_back(std::move(renew.back()));
		renew.pop_back();
		OnPaint();
		}
		break;
	case ID_Revoke:
		//����Ctrl+z��(����)
		if (revocation.size()) {
		canvas.Clear();
		renew.push_back(std::move(revocation.back()));
		revocation.pop_back();
		OnPaint();
		}
		break;
	case ID_About: {
		std::wstring strDrVe = L"�汾:";
		strDrVe += DrawVersion;
		MessageBox(NULL, strDrVe.c_str(), L"����", MB_OK);
	}
	default:
		break;
	}
}
void OnDropFiles(HWND hwnd, HDROP hDropInfo) {
	UINT  nFileCount = DragQueryFile(hDropInfo, (UINT)-1, NULL, 0);
	TCHAR szFileName[_MAX_PATH] = _T("");
	DWORD dwAttribute;

	// ��ȡ��ק�����ļ����ļ���
	for (UINT i = 0; i < nFileCount; i++)
	{
		DragQueryFile(hDropInfo, i, szFileName, sizeof(szFileName));
		dwAttribute = ::GetFileAttributes(szFileName);

		// �Ƿ�Ϊ�ļ���
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
//���ؼ��ټ�
HACCEL haccel = LoadAccelerators(GetModuleHandle(NULL), (wchar_t*)IDR_ACCELERATOR1);
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//if(msg!=WM_COMMAND)
	MSG Msg{};
	Msg.hwnd = hWnd;
	Msg.message = msg;
	Msg.wParam = wParam;
	Msg.lParam = lParam;
	//������ټ�
	//���Ϊ���ټ�ֱ��Ĭ�ϴ���
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
		if (MessageBox(NULL, L"�Ƿ��,δ�������ݽ���ʧ", L"��", MB_YESNO | MB_ICONWARNING) == IDYES) {
			canvas.Clear();
			revocation.clear();
			renew.clear();
			OnDropFiles(hWnd, (HDROP)wParam);
			OnPaint();
		}
		break;
	default:
		return HIWINDOW_DEFAULT_PROC;	// ��ʶʹ��Ĭ����Ϣ��������������
		break;
	}
	hiex::DelayFPS(120);
	return 0;
}
int main() {
	hiex::Window drawWnd;
	drawWnd.PreSetStyleEx(WS_EX_ACCEPTFILES);
	drawWnd.Create(1080, 960, 0, wndTitle.c_str(), WndProc);
	//���ò˵�
	SetMenu(drawWnd.GetHandle(), LoadMenu(GetModuleHandle(NULL), (wchar_t*)IDR_MENU1));
	drawWndPointer = &drawWnd;
	drawWnd.BindCanvas(&canvas);
	while (drawWnd.IsAlive())
	{
		//drawWnd.Redraw();
		hiex::DelayFPS(1,true);
	}
}