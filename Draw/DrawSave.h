#pragma once
#include <Windows.h>
#include <thread>
#include <fstream>
#include <list>
#include <vector>
#include <string>
//���ڱ����ͼ����
class DrawSave
{
public:
	~DrawSave();
	void Save(std::wstring path, std::vector<std::vector<POINT>>& vePo);
	bool IsSave();
private:
	std::thread saveThread;
	//�Ƿ����ڱ���
	bool isNowSave;
	//std::list<std::thread*> threadList;
};

