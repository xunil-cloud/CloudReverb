#pragma once

#include <cmath>

namespace CloudSeed
{
	class FastSin
	{
	private:
		static const int DataSize = 32768;
		static double data[DataSize];

	public:
		static void ZeroBuffer(double* buffer, int len);
		static void Init()
		{
			for (int i = 0; i < DataSize; i++)
			{
				data[i] = std::sin(2 * M_PI * i / (double)DataSize);
			}
		}

		static double Get(double index)
		{
			return data[(int)(index * 32767.99999)];
		}
	};
}

