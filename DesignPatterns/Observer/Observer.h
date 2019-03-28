#pragma once

#include <string>

namespace observer_pattern
{
	template<typename T>
	struct Observer
	{
		virtual ~Observer() {}
		virtual void onNotify(const T& message) = 0;
	};
}