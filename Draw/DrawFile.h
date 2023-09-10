#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include <Shlobj.h>
#include <regex>
#include "DrawDef.h"
#include "DrawFactory.h"
enum FileType {
	Image, Folder, other
};
class FileInfo {
public:
	bool IsEffective();
	std::wstring GetPath();
	std::wstring GetName();
	std::wstring GetNameAndExten();
	std::wstring GetExten();
	int GetExtenSel();
	FileType GetFileType();
	bool SetNameAndExten(std::wstring name);
	bool SetPath(std::wstring path);
	bool SetParentPath(std::wstring parentPath);
	bool SetExten(std::wstring exten);
private:
	std::wstring name;
	std::wstring parentPath;
	std::wstring exten;
	int extenSel = -1;
};
//支持的文件格式
const int fileFormatNumber = 3;
const std::wstring fileFormat[fileFormatNumber] = {
		L"png",
		L"jpg",
		L"bmp"
};
//用于保存与加载绘图数据
class DrawFile
{
public:
	DrawFile();
	~DrawFile();
	//保存
	void Save(hiex::Window& wnd, bool isSaveAS = false);
	//加载
	bool Load(hiex::Window& wnd, Draw::DrawImageUPtr& drIm);
	bool Load(std::wstring path, Draw::DrawImageUPtr& drIm);
private:
	hiex::Window CreateFileWindow(hiex::Window& wnd, std::wstring title);
	void SaveDrawImage(std::wstring path, IMAGE* image);
	bool LoadDrawImage(FileInfo FiIn, Draw::DrawImageUPtr& drIm);

	bool GetLoadFileInfo(hiex::Window& wnd, FileInfo& loadPath);
	bool GetSavePath(hiex::Window& wnd, FileInfo& savePath);
	std::wstring GetFileInfo();
	std::wstring GetFolderPath();
private:
	FileInfo savePath;
};

