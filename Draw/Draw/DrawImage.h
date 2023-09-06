#pragma once
#include "DrawBase.h"

namespace Draw {
	class DrawImage :
		public DrawBase
	{
	public:
		// ͨ�� DrawBase �̳�
		bool IsDraw() override;
		void ReDraw(hiex::Canvas& canvas) override;
		//����ͼ��
		void SetImage(IMAGE& image);
	private:
		IMAGE image;
	private:
		void SaveData(std::wofstream& out) override;//����
		void LoadData(std::wifstream& in) override;//����
	};
}

