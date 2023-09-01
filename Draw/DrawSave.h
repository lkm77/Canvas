#pragma once
#include <Windows.h>
#include <thread>
#include <fstream>
#include <list>
#include <vector>
#include <string>
//用于保存绘图数据
class DrawSave
{
public:
	~DrawSave();
	void Save(std::wstring path, std::vector<std::vector<POINT>>& vePo);
	bool IsSave();
private:
	std::thread saveThread;
	//是否正在保存
	bool isNowSave;
	//std::list<std::thread*> threadList;
};

