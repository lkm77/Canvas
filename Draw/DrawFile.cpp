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
	in >> DrawVersion;//��ʱû��
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



