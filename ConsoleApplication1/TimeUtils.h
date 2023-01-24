#pragma once

#include <chrono>

using Clock = std::chrono::high_resolution_clock;

long toMs(std::chrono::nanoseconds t)
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
}
