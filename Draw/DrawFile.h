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
//���ڱ�������ػ�ͼ����
class DrawFile
{
public:
	~DrawFile();
	//����
	void Save(const std::vector<Draw::DrawBaseUPtr>& veDrBa);
	//���Ϊ
	void SaveAS(const std::vector<Draw::DrawBaseUPtr>& veDrBa);
	//����
	void Load(std::vector<Draw::DrawBaseUPtr>& veDrBa);
private:
	void LoadDrawData(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa);
	void LoadDrawImage(FileInfo FiIn, std::vector<Draw::DrawBaseUPtr>& veDrBa);
	FileInfo GetFileInfo();
	void GetFolderPath();
private:
	std::wstring savePath;
};

