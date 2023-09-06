#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include <Shlobj.h>
#include <regex>
#include "DrawDef.h"
#include "DrawFactory.h"
enum FileType {
	Image, Folder, DrawData,other
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
const int fileFormatNumber = 4;
const std::wstring fileFormat[fileFormatNumber] = {
		L"DrawData",
		L"jpg",
		L"png",
		L"bmp"
};
//用于保存与加载绘图数据
class DrawFile
{
public:
	DrawFile();
	~DrawFile();
	//保存
	void Save(hiex::Window& wnd, const std::vector<Draw::DrawBaseUPtr>& veDrBa, FileInfo* savePath=nullptr);
	//另存为
	void SaveAS(hiex::Window& wnd, const std::vector<Draw::DrawBaseUPtr>& veDrBa);
	//加载
	void Load(hiex::Window& wnd, std::vector<Draw::DrawBaseUPtr>& veDrBa);
private:
	hiex::Window CreateFileWindow(hiex::Window& wnd, std::wstring title);
	void SaveDrawData(std::wofstream& out, const std::vector<Draw::DrawBaseUPtr>& veDrBa);
	void SaveDrawImage(std::wstring path, IMAGE* image);
	void LoadDrawData(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa);
	void LoadDrawImage(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa);

	bool GetLoadFileInfo(hiex::Window& wnd, FileInfo& loadPath);
	bool GetSavePath(hiex::Window& wnd, FileInfo& savePath);
	std::wstring GetFileInfo();
	std::wstring GetFolderPath();
private:
	FileInfo savePath;
	//hiex::Window saveWnd;
	//hiex::Window loadWnd;
};

