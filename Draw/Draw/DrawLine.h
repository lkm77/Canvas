#pragma once
#include "DrawBase.h"
namespace Draw {
	class DrawLine :
		public DrawBase
	{
	public:
		DrawLine();
		~DrawLine();
		bool IsDraw() override;
		void AddPoint(hiex::Canvas& canvas, POINT po);
		void ReDraw(hiex::Canvas& canvas) override;
	private:
		void SaveData(std::wofstream& out) override;
		void LoadData(std::wifstream& in) override;
	private:
		std::vector<POINT> vecPo;

	};
}
