#pragma once
#include <Windows.h>
#include <fstream>
#include <string>
#include <Shlobj.h>
#include "DrawDef.h"
#include "DrawFactory.h"
enum FileType {
	Image, Folder, DrawData,other
};
struct FileInfo {
	std::wstring name;
	std::wstring path;
	std::wstring exten;
	FileType fiTy = other;
	void SetFileType();
};
//用于保存与加载绘图数据
class DrawFile
{
public:
	~DrawFile();
	//保存
	void Save(const std::vector<Draw::DrawBaseUPtr>& veDrBa);
	//另存为
	void SaveAS(const std::vector<Draw::DrawBaseUPtr>& veDrBa);
	//加载
	void Load(std::vector<Draw::DrawBaseUPtr>& veDrBa);
private:
	void LoadDrawData(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa);
	void LoadDrawImage(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa);
	FileInfo GetFileInfo();
	void GetFolderPath();
private:
	std::wstring savePath;
};

