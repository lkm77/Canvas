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
		else if (this->vecPo.back() == po)return;
		else po2 = this->vecPo.back();
		canvas.GP_Line(po.x, po.y, po2.x, po2.y, true, this->GetLineColor());
		this->vecPo.push_back(po);
	}
	void DrawLine::ReDraw(hiex::Canvas& canvas)
	{
		for (size_t i = 1; i < this->vecPo.size(); i++)
		{
			canvas.GP_Line(vecPo[i - 1].x, vecPo[i - 1].y, vecPo[i].x, vecPo[i].y, true, this->GetLineColor());
		}
	}
}