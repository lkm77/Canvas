#include "DrawLine.h"
namespace Draw {
	DrawLine::DrawLine()
	{
		this->SetDrawType(Line);
	}
	DrawLine::~DrawLine()
	{
	}
	bool DrawLine::IsDraw()
	{
		return !this->vecPo.empty();
	}
	void DrawLine::AddPoint(hiex::Canvas& canvas, POINT po)
	{

		POINT po2;
		if (this->vecPo.empty())po2 = po;
		else po2 = this->vecPo.back();
		canvas.Line(po, po2, true, this->GetLineColor());
		this->vecPo.push_back(po);
	}
	void DrawLine::ReDraw(hiex::Canvas& canvas)
	{
		canvas.Polyline(this->vecPo.data(), this->vecPo.size(), true, this->GetLineColor());
	}
	void DrawLine::SaveData(std::wofstream& out)
	{
		out << vecPo.size() << " ";
		for (auto& po : this->vecPo)
		{
			out << po.x << " " << po.y << " ";
		}
	}
	void DrawLine::LoadData(std::wifstream& in)
	{
		size_t size;
		in >> size;
		POINT po{};
		for (size_t i = 0; i < size; i++)
		{
			in >> po.x >> po.y;
			this->vecPo.push_back(po);
		}
	}
}