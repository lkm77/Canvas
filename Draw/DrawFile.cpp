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
	if (this->exten == L"DrawData")return DrawData;
	else return Image;
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
void DrawFile::Save(hiex::Window& wnd, const std::vector<Draw::DrawBaseUPtr>& veDrBa, FileInfo* savePath)
{
	FileInfo saPa;
	if (savePath)saPa = *savePath;
	else if (this->savePath.IsEffective())saPa = this->savePath;
	else
	{

		if (this->GetSavePath(wnd, saPa));
		else return;
	}
	if (!saPa.IsEffective()) {
		MessageBox(NULL, L"����·��Ϊ��", L"����·������", MB_YESNO | MB_ICONERROR);
		return;
	}
	if (saPa.GetFileType() == Image)
	{
		this->SaveDrawImage(saPa.GetPath(), wnd.GetImage());
		return;
	}
	std::wofstream out(saPa.GetPath());
	out << DrawVersion << std::endl;
	this->SaveDrawData(out, veDrBa);
}
void DrawFile::SaveAS(hiex::Window& wnd, const std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	FileInfo saPa;
	if(!this->GetSavePath(wnd, saPa))return;
	Save(wnd, veDrBa, &saPa);
}
void DrawFile::SaveDrawData(std::wofstream& out, const std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	for (auto& drBa : veDrBa)
	{
		drBa->Save(out);
		out << std::endl;
	}
}
void DrawFile::SaveDrawImage(std::wstring path, IMAGE* image)
{
	saveimage(path.c_str(), image);
}
void DrawFile::Load(hiex::Window& wnd, std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	FileInfo FiIn;
	this->GetLoadFileInfo(wnd, FiIn);
	if (FiIn.GetFileType() == DrawData)this->LoadDrawData(FiIn, veDrBa);
	else if (FiIn.GetFileType() == Image)this->LoadDrawImage(FiIn, veDrBa);
}
void DrawFile::LoadDrawData(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	std::wifstream in(FiIn.GetPath());
	std::wstring DrawVersion;
	in >> DrawVersion;//��ʱû��
	int DrTy = 0;
	while (in >> DrTy)
	{
		Draw::DrawBaseUPtr DrBa = Draw::DrawFactory(Draw::DrawType(DrTy));
		DrBa->Load(in);
		veDrBa.push_back(std::move(DrBa));
	}

}
void DrawFile::LoadDrawImage(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	hiex::Canvas ca;
	IMAGE im = ca.Load_Image_Alpha(
		FiIn.GetPath().c_str(),
		0, 0,
		false,
		0, 0,
		(BYTE)255U,
		true, true
	);
	Draw::DrawBaseUPtr DrBa = Draw::DrawFactory(Draw::Image);
	Draw::DrawImage* DrIm = (Draw::DrawImage*)DrBa.get();
	DrIm->SetImage(im);
	veDrBa.push_back(std::move(DrBa));
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
	saPa.SetPath(L"C:\\1.DrawData");
	hiex::Window saveWnd = this->CreateFileWindow(wnd, L"Save");
	hiex::Canvas ca;
	saveWnd.BindCanvas(&ca);

	hiex::SysEdit pathEdit(saveWnd.GetHandle(), 80, 120, 280, 20,saPa.GetPath());
	ca.CenterText(L"����·��:", { 0, 120, 80, 140 });

	hiex::SysEdit nameEdit(saveWnd.GetHandle(), 80, 160, 280, 20);
	ca.CenterText(L"�ļ���:", { 0, 160, 80, 180 });
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
	ca.CenterText(L"�����ʽ:", { 0, 200, 80, 220 });

	//ȷ����ť
	hiex::SysButton determine(saveWnd.GetHandle(), 220, 350, 50, 40, L"ȷ��");
	//ȡ����ť
	hiex::SysButton cancellation(saveWnd.GetHandle(), 290, 350, 50, 40, L"ȡ��");
	//����ļ�Ŀ¼��ť
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
			if (saPa.SetParentPath(this->GetFolderPath()))
				pathEdit.SetText(saPa.GetPath());
			BringWindowToTop(saveWnd.GetHandle());
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
	TCHAR strFileName[MAX_PATH] = { 0 };	//���ڽ����ļ���
	ofn.lStructSize = sizeof(OPENFILENAME);	//�ṹ���С
	ofn.hwndOwner = NULL;					//ӵ���Ŵ��ھ��
	//��ʽ ����+���˹���
	ofn.lpstrFilter = TEXT("DrawData\0*.DrawData\0jpg\0*.jpg\0bmp\0*.bmp\0png\0*.png\0\0");	//���ù���
	ofn.nFilterIndex = 1;	//����������
	ofn.lpstrFile = strFileName;	//���շ��ص��ļ�����ע���һ���ַ���ҪΪNULL
	ofn.nMaxFile = sizeof(strFileName);	//����������
	ofn.lpstrInitialDir = NULL;			//��ʼĿ¼ΪĬ��
	ofn.lpstrTitle = TEXT("��ѡ��һ���ļ�"); //���ڱ���
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; //�ļ���Ŀ¼������ڣ�����ֻ��ѡ��
	//���ļ��Ի���
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



