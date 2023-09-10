#pragma once
#include "DrawBase.h"

namespace Draw {
	typedef std::unique_ptr <IMAGE> ImageUPtr;
	class DrawImage :
		public DrawBase
	{
	public:
		// ͨ�� DrawBase �̳�
		bool IsDraw() override;
		void ReDraw(hiex::Canvas& canvas) override;
		//����ͼ��
		void SetImage(ImageUPtr& image);
		void RemoveImage();
	private:
		ImageUPtr image;
	};
	typedef std::unique_ptr <DrawImage> DrawImageUPtr;
}

