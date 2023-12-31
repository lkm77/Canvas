#include "DrawFile.h"

bool FileInfo::IsEffective()
{
	if (this->parentPath.empty())return false;
	if (this->name.empty())return false;
	if (this->exten.empty())return false;
	return true;
}
std::wstring FileInfo::GetPath()
{
	if (this->IsEffective())return this->parentPath + L"\\" + GetNameAndExten();
	else return std::wstring();
}
std::wstring FileInfo::GetName()
{
	return this->name;
}
std::wstring FileInfo::GetNameAndExten()
{
	if (this->GetName().empty() || this->GetExten().empty())return std::wstring();
	return this->GetName() + L"." + this->GetExten();
}
std::wstring FileInfo::GetExten()
{
	return this->exten;
}
int FileInfo::GetExtenSel()
{
	return this->extenSel;
}
FileType FileInfo::GetFileType()
{
	if (this->exten == L"")return other;
	return Image;
}
bool FileInfo::SetNameAndExten(std::wstring name)
{
	std::wregex r(L"^([^\\s\\\\/:*?<>\"|]+)[.]+([^\\s\\\\/:*?<>\"|]+)$");
	std::wsmatch ma;
	if (std::regex_match(name, ma, r)) {
		if (this->SetExten(ma[2])) {
			this->name = ma[1];
			return true;
		}
	}
	return false;
}
bool FileInfo::SetPath(std::wstring path)
{
	std::wregex r(L"^([a-zA-Z]:(?:[\\\\][^\\s\\\\/:*?<>\"|]+)*)[\\\\]([^\\s\\\\/:*?<>\"|]+)[.]+([^\\//:*?<>\"|]+)$");
	std::wsmatch ma;
	if (std::regex_match(path, ma, r)) {
		if (this->SetExten(ma[3])) {
			this->parentPath = ma[1];
			this->name = ma[2];
			return true;
		}
	}
	return false;
}
bool FileInfo::SetParentPath(std::wstring parentPath)
{
	return SetPath(parentPath + L"\\" + GetNameAndExten());
}
bool FileInfo::SetExten(std::wstring exten)
{
	for (size_t i = 0; i < fileFormatNumber; i++)
	{
		if (fileFormat[i] == exten) {
			this->exten = exten;
			this->extenSel = i;
			return true;
		}
	}
	return false;
}
DrawFile::DrawFile()
{
	//this->loadWnd.Create();
	//this->saveWnd.Create();
	//ShowWindow(this->loadWnd.GetHandle(), SW_HIDE);
	//ShowWindow(this->saveWnd.GetHandle(), SW_HIDE);
}
DrawFile::~DrawFile()
{
}
void DrawFile::Save(hiex::Window& wnd, bool isSaveAS)
{
	FileInfo saPa;
	if (!isSaveAS && this->savePath.IsEffective())saPa = this->savePath;
	else if (this->GetSavePath(wnd, saPa))this->savePath = saPa;
	else return;
	if (!saPa.IsEffective()) MessageBox(NULL, L"保存路径无效", L"保存错误", MB_YESNO | MB_ICONERROR);
	else this->SaveDrawImage(saPa.GetPath(), wnd.GetImage());
	return;
}
void DrawFile::SaveDrawImage(std::wstring path, IMAGE* image)
{
	saveimage(path.c_str(), image);
}
bool DrawFile::Load(hiex::Window& wnd, Draw::DrawImageUPtr& drIm)
{
	FileInfo FiIn;
	if (!this->GetLoadFileInfo(wnd, FiIn))return false;
	if(!FiIn.IsEffective())MessageBox(NULL, L"加载路径无效", L"加载错误", MB_YESNO | MB_ICONERROR);

	else if(this->LoadDrawImage(FiIn, drIm))return true;
	else MessageBox(NULL, L"加载文件错误", L"加载错误", MB_YESNO | MB_ICONERROR);
	return false;
}
bool DrawFile::Load(std::wstring path, Draw::DrawImageUPtr& drIm)
{
	FileInfo FiIn;
	FiIn.SetPath(path);
	if (!FiIn.IsEffective())MessageBox(NULL, L"加载路径无效", L"加载错误", MB_YESNO | MB_ICONERROR);

	else if (this->LoadDrawImage(FiIn, drIm))return true;
	else MessageBox(NULL, L"加载文件错误", L"加载错误", MB_YESNO | MB_ICONERROR);
	return false;
}
bool DrawFile::LoadDrawImage(FileInfo FiIn, Draw::DrawImageUPtr& drIm)
{
	hiex::Canvas ca;
	Draw::ImageUPtr im(new IMAGE);
	loadimage(im.get(), FiIn.GetPath().c_str());
	if (!(im->getwidth() && im->getheight()))return false;
	drIm->SetImage(im);
	return true;
}

bool DrawFile::GetLoadFileInfo(hiex::Window& wnd, FileInfo& loadPath)
{
	return loadPath.SetPath(this->GetFileInfo());
}

hiex::Window DrawFile::CreateFileWindow(hiex::Window& wnd, std::wstring title)
{
	hiex::Window fileWnd;
	fileWnd.PreSetPos(
		(wnd.GetPos().x + wnd.GetClientWidth() - 400) / 2,
		(wnd.GetPos().y + wnd.GetClientHeight() - 400) / 2
	);
	fileWnd.Create(400, 400, 0, title.c_str());
	EnableSystemMenu(fileWnd.GetHandle(), false);
	EnableResizing(fileWnd.GetHandle(), false);
	return std::move(fileWnd);
}
bool DrawFile::GetSavePath(hiex::Window& wnd, FileInfo& savePath)
{
	FileInfo saPa;
	saPa.SetPath(L"C:\\1.png");
	hiex::Window saveWnd = this->CreateFileWindow(wnd, L"Save");
	hiex::Canvas ca;
	saveWnd.BindCanvas(&ca);

	hiex::SysEdit pathEdit(saveWnd.GetHandle(), 80, 120, 280, 20,saPa.GetPath());
	ca.CenterText(L"保存路径:", { 0, 120, 80, 140 });

	hiex::SysEdit nameEdit(saveWnd.GetHandle(), 80, 160, 280, 20);
	ca.CenterText(L"文件名:", { 0, 160, 80, 180 });
	nameEdit.SetText(saPa.GetNameAndExten());

	hiex::SysComboBox formatComboBox;
	hiex::SysComboBox::PreStyle prSt;
	prSt.editable = false;
	formatComboBox.PreSetStyle(prSt);
	formatComboBox.Create(saveWnd.GetHandle(), 80, 200, 280, 20);
	for (size_t i = 0; i < fileFormatNumber; i++)
	{
		formatComboBox.AddString(fileFormat[i]);
	}
	formatComboBox.SetSel(0);
	ca.CenterText(L"保存格式:", { 0, 200, 80, 220 });

	//确定按钮
	hiex::SysButton determine(saveWnd.GetHandle(), 220, 350, 50, 40, L"确定");
	//取消按钮
	hiex::SysButton cancellation(saveWnd.GetHandle(), 290, 350, 50, 40, L"取消");
	//浏览文件目录按钮
	hiex::SysButton folder(saveWnd.GetHandle(), 360, 120, 20, 20, L"...");
	saveWnd.Redraw();
	while (1) {
		if (cancellation.IsClicked()) {
			saveWnd.Destroy();
			return false;
		}
		if (determine.IsClicked()) {
			if (saPa.IsEffective())
			{
				savePath = saPa;
				saveWnd.Destroy();
				return true;
			}
		}
		if (folder.IsClicked()) {
			folder.Enable(false);
			if (saPa.SetParentPath(this->GetFolderPath()))
				pathEdit.SetText(saPa.GetPath());
			BringWindowToTop(saveWnd.GetHandle());
			folder.Enable(true);
		}
		if (pathEdit.IsEdited()) {
			std::wstring path = pathEdit.GetText();
			if (path.empty())path = saPa.GetPath();
			const std::wstring name = path.substr(path.rfind(L"\\") + 1);
			if (nameEdit.GetText() != name)nameEdit.SetText(name);
			if (saPa.SetPath(path)) {
					formatComboBox.SetSel(saPa.GetExtenSel());
			}
		}
		if (nameEdit.IsEdited()) {
			std::wstring name = nameEdit.GetText();
			if (name.empty())name = saPa.GetNameAndExten();
			const std::wstring path = pathEdit.GetText();
			const std::wstring parentPath = path.substr(0, path.rfind(L"\\"));
			const std::wstring pathName = path.substr(path.rfind(L"\\") + 1);
			saPa.SetNameAndExten(name);
			if (name != pathName)pathEdit.SetText(parentPath + L"\\" + name);
		}
		if (formatComboBox.IsSelChanged())
		{
			saPa.SetExten(fileFormat[formatComboBox.GetSel()]);
			pathEdit.SetText(saPa.GetPath());
		}
		hiex::DelayFPS(120);
	}
}
std::wstring DrawFile::GetFileInfo()
{
	OPENFILENAME ofn = { 0 };
	TCHAR strFileName[MAX_PATH] = { 0 };	//用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);	//结构体大小
	ofn.hwndOwner = NULL;					//拥有着窗口句柄
	//格式 名字+过滤规则
	ofn.lpstrFilter = TEXT("png\0*.png\0jpg\0*.jpg\0bmp\0*.bmp\0\0");	//设置过滤
	ofn.nFilterIndex = 1;	//过滤器索引
	ofn.lpstrFile = strFileName;	//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(strFileName);	//缓冲区长度
	ofn.lpstrInitialDir = NULL;			//初始目录为默认
	ofn.lpstrTitle = TEXT("请选择一个文件"); //窗口标题
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; //文件、目录必须存在，隐藏只读选项
	//打开文件对话框
	if (GetOpenFileName(&ofn)) {
		return strFileName;
	}
	return std::wstring();
}
std::wstring DrawFile::GetFolderPath()
{
	TCHAR path[MAX_PATH] = { 0 };
	TCHAR t[MAX_PATH] = { 0 };
	BROWSEINFO bi;
	bi.hwndOwner = 0;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = path;
	bi.lpszTitle = t;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != NULL)
	{
		SHGetPathFromIDList(pidl, path);
		wchar_t* qsFile = bi.pszDisplayName;
		LPMALLOC pMalloc;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}
		return  qsFile;
	}
	return std::wstring();
}



