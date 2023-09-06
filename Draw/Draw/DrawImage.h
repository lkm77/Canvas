#pragma once
#include "DrawBase.h"

namespace Draw {
	class DrawImage :
		public DrawBase
	{
	public:
		// 通过 DrawBase 继承
		bool IsDraw() override;
		void ReDraw(hiex::Canvas& canvas) override;
		//设置图像
		void SetImage(IMAGE& image);
	private:
		IMAGE image;
	private:
		void SaveData(std::wofstream& out) override;//无用
		void LoadData(std::wifstream& in) override;//无用
	};
}

