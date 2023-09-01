/**
 * @file	SysGroupBox.h
 * @brief	HiSysGUI �ؼ���֧�������
 * @author	huidong
*/

#pragma once

#include "SysControlBase.h"

namespace HiEasyX
{
	/**
	 * @brief <pre>
	 *		ϵͳ�����ؼ�
	 * 
	 *	ע�⣺
	 *		�����ؼ�ʱ��Ҫ���˿ؼ���Ϊ���ؼ��������޷������ؼ�
	 * </pre>
	 * 
	 * @bug
	 *		����򱳾������� Bug��������ʹ��
	*/
	class SysGroupBox : public SysControlBase
	{
	protected:

		void RealCreate(HWND hParent) override;

	public:

		SysGroupBox();

#ifdef UNICODE
		SysGroupBox(HWND hParent, RECT rct, std::wstring strText = L"");
		SysGroupBox(HWND hParent, int x, int y, int w, int h, std::wstring strText = L"");
#else
		SysGroupBox(HWND hParent, RECT rct, std::string strText = "");
		SysGroupBox(HWND hParent, int x, int y, int w, int h, std::string strText = "");
#endif
	};
}