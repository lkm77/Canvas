#include "DrawFile.h"
void FileInfo::SetFileType()
{
	if (this->exten == L"DrawData")this->fiTy = DrawData;
	else this->fiTy = Image;
}
DrawFile::~DrawFile()
{
}
void DrawFile::Save(const std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	if (this->savePath == L"")this->GetFolderPath();
	std::wofstream out(this->savePath + L"\\sava.DrawData");
	out << DrawVersion << std::endl;
	for (auto& drBa : veDrBa)
	{
		drBa->Save(out);
		out << std::endl;
	}
}
void DrawFile::SaveAS(const std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	this->GetFolderPath();
	Save(veDrBa);
}
void DrawFile::Load(std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	FileInfo FiIn = this->GetFileInfo();
	if (FiIn.fiTy == DrawData)this->LoadDrawData(FiIn, veDrBa);
	else if (FiIn.fiTy = Image)this->LoadDrawImage(FiIn, veDrBa);
}
void DrawFile::LoadDrawData(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
	std::wifstream in(FiIn.path);
	std::wstring DrawVersion;
	in >> DrawVersion;//暂时没用
	int DrTy = 0;
	while (in>> DrTy)
	{
		Draw::DrawBaseUPtr DrBa = Draw::DrawFactory(Draw::DrawType(DrTy));
		DrBa->Load(in);
		veDrBa.push_back(std::move(DrBa));
	}

}
void DrawFile::LoadDrawImage(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa)
{
}
FileInfo DrawFile::GetFileInfo()
{
	OPENFILENAME ofn = { 0 };
	TCHAR strFileName[MAX_PATH] = { 0 };	//用于接收文件名
	ofn.lStructSize = sizeof(OPENFILENAME);	//结构体大小
	ofn.hwndOwner = NULL;					//拥有着窗口句柄
	//格式 名字+过滤规则
	ofn.lpstrFilter = TEXT("DrawData\0*.DrawData\0jpg\0*.jpg\0bmp\0*.bmp\0png\0*.png\0\0");	//设置过滤
	ofn.nFilterIndex = 1;	//过滤器索引
	ofn.lpstrFile = strFileName;	//接收返回的文件名，注意第一个字符需要为NULL
	ofn.nMaxFile = sizeof(strFileName);	//缓冲区长度
	ofn.lpstrInitialDir = NULL;			//初始目录为默认
	ofn.lpstrTitle = TEXT("请选择一个文件"); //窗口标题
	ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY; //文件、目录必须存在，隐藏只读选项
	//打开文件对话框
	if (GetOpenFileName(&ofn)) {
		std::wstring filePath = strFileName;
		int start = filePath.find_last_of('\\');
		int end = filePath.find_last_of('.');
		std::wstring fileName = filePath.substr(start + 1, end - start - 1);
		std::wstring exten = filePath.substr(end+1, filePath.length() - end);
		FileInfo FiIn;
		FiIn.name = fileName;
		FiIn.path = filePath;
		FiIn.exten = exten;
		FiIn.SetFileType();
		return FiIn;
	}
}
void DrawFile::GetFolderPath()
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
		this->savePath = qsFile;
	}
}



