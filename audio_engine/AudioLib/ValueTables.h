#ifndef AUDIOLIB_VALUETABLES
#define AUDIOLIB_VALUETABLES

namespace AudioLib
{
	class ValueTables
	{
	public:
		static const int TableSize = 4001;

		static float Sqrt[TableSize];
		static float Sqrt3[TableSize];
		static float Pow1_5[TableSize];
		static float Pow2[TableSize];
		static float Pow3[TableSize];
		static float Pow4[TableSize];
		static float x2Pow3[TableSize];

		// octave response. value float every step (2,3,4,5 or 6 steps)
		static float Response2Oct[TableSize];
		static float Response3Oct[TableSize];
		static float Response4Oct[TableSize];
		static float Response5Oct[TableSize];
		static float Response6Oct[TableSize];

		// decade response, value multiplies by 10 every step
		static float Response2Dec[TableSize];
		static float Response3Dec[TableSize];
		static float Response4Dec[TableSize];

		static void Init();
		static float Get(float index, float* table);
	};
}

#endif
