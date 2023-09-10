#pragma once
#include "DrawBase.h"

namespace Draw {
	typedef std::unique_ptr <IMAGE> ImageUPtr;
	class DrawImage :
		public DrawBase
	{
	public:
		// Õ®π˝ DrawBase ºÃ≥–
		bool IsDraw() override;
		void ReDraw(hiex::Canvas& canvas) override;
		//…Ë÷√ÕºœÒ
		void SetImage(ImageUPtr& image);
		void RemoveImage();
	private:
		ImageUPtr image;
	};
	typedef std::unique_ptr <DrawImage> DrawImageUPtr;
}

