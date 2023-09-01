#include "DrawSave.h"
void savePo(bool& isNowSave, std::wstring path, std::vector<std::vector<POINT>> vePo) {
	std::ofstream out(path);
	for (auto& ti : vePo) {
		for (auto& po : ti) {
			out << po.x << " " << po.y << " ";
		}
	}
	isNowSave = false;
}
DrawSave::~DrawSave()
{
	if (this->saveThread.joinable())this->saveThread.join();
	this->isNowSave = false;
}
void DrawSave::Save(std::wstring path, std::vector<std::vector<POINT>>& vePo)
{
	this->isNowSave = true;
	std::thread th(savePo, std::ref(this->isNowSave), path, vePo);
	this->saveThread.swap(th);
}

bool DrawSave::IsSave()
{
	return this->isNowSave;
}
