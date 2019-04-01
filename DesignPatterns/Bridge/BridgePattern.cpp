#include "BridgePattern.h"
#include <iostream>

namespace bridge_pattern
{
	// can be kept in some other file not visible to the client
	class SystemDialogImpl_PS4 : public SystemDialogImpl
	{
		void render() override {}
		void setAnimData(const AnimData& data) override {}
	};

	class SystemDialogImpl_X1 : public SystemDialogImpl
	{
		void render() override {}
		void setAnimData(const AnimData& data) override {}
	};

	class SystemDialogImpl_NX : public SystemDialogImpl
	{
		void render() override {}
		void setAnimData(const AnimData& data) override {}
	};

	void SystemDialog::setXYWH(float _x, float _y, float _w, float _h)
	{
		x = _x;
		y = _y;
		w = _w;
		h = _h;
	}

	void SystemDialog::render()
	{
		std::cout << "running SystemDialog::render()" << std::endl;
	}
}
