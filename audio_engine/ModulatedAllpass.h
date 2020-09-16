#ifndef MODULATEDALLPASS
#define MODULATEDALLPASS

#include "ModulatedAllpass.h"
#include "FastSin.h"
#include "Utils.h"

namespace CloudSeed
{
	class ModulatedAllpass
	{
	public:
		const int DelayBufferSamples = 19200; // 100ms at 192Khz
		static const int ModulationUpdateRate = 8;

	private:
		float* delayBuffer;
		float* output;
		int bufferSize;
		int index;
		unsigned int samplesProcessed;

		float modPhase;
		int delayA;
		int delayB;
		float gainA;
		float gainB;

	public:

		int SampleDelay;
		float Feedback;
		float ModAmount;
		float ModRate;

		bool InterpolationEnabled;
		bool ModulationEnabled;

		ModulatedAllpass(int bufferSize, int sampleDelay)
		{
			this->InterpolationEnabled = true;
			this->bufferSize = bufferSize;
			delayBuffer = new float[DelayBufferSamples];
			output = new float[bufferSize];
			SampleDelay = sampleDelay;
			index = DelayBufferSamples - 1;
			modPhase = 0.01 + 0.98 * std::rand() / (float)RAND_MAX;
			ModRate = 0.0;
			ModAmount = 0.0;
			Update();
		}

		~ModulatedAllpass()
		{
			delete delayBuffer;
			delete output;
		}


		float* GetOutput()
		{
			return output;
		}

		void ClearBuffers()
		{
			Utils::ZeroBuffer(delayBuffer, DelayBufferSamples);
			Utils::ZeroBuffer(output, bufferSize);
		}

		void Process(const float* input, int sampleCount)
		{
			if (ModulationEnabled)
				ProcessWithMod(input, sampleCount);
			else
				ProcessNoMod(input, sampleCount);
		}


	private:
		void ProcessNoMod(const float* input, int sampleCount)
		{
			auto delayedIndex = index - SampleDelay;
			if (delayedIndex < 0) delayedIndex += DelayBufferSamples;

			for (int i = 0; i < sampleCount; i++)
			{
				auto bufOut = delayBuffer[delayedIndex];
				auto inVal = input[i] + bufOut * Feedback;

				delayBuffer[index] = inVal;
				output[i] = bufOut - inVal * Feedback;

				index++;
				delayedIndex++;
				if (index >= DelayBufferSamples) index -= DelayBufferSamples;
				if (delayedIndex >= DelayBufferSamples) delayedIndex -= DelayBufferSamples;
				samplesProcessed++;
			}
		}

		void ProcessWithMod(const float* input, int sampleCount)
		{
			for (int i = 0; i < sampleCount; i++)
			{
				if (samplesProcessed >= ModulationUpdateRate)
					Update();

				float bufOut;

				if (InterpolationEnabled)
				{
					int idxA = index - delayA;
					int idxB = index - delayB;
					idxA += DelayBufferSamples * (idxA < 0); // modulo
					idxB += DelayBufferSamples * (idxB < 0); // modulo

					bufOut = delayBuffer[idxA] * gainA + delayBuffer[idxB] * gainB;
				}
				else
				{
					int idxA = index - delayA;
					idxA += DelayBufferSamples * (idxA < 0); // modulo
					bufOut = delayBuffer[idxA];
				}

				auto inVal = input[i] + bufOut * Feedback;
				delayBuffer[index] = inVal;
				output[i] = bufOut - inVal * Feedback;

				index++;
				if (index >= DelayBufferSamples) index -= DelayBufferSamples;
				samplesProcessed++;
			}
		}

		float Get(int delay)
		{
			int idx = index - delay;
			if (idx < 0)
				idx += DelayBufferSamples;

			return delayBuffer[idx];
		}

		void Update()
		{
			modPhase += ModRate * ModulationUpdateRate;
			if (modPhase > 1)
				modPhase = std::fmod(modPhase, 1.0);

			auto mod = FastSin::Get(modPhase);

			if (ModAmount >= SampleDelay) // don't modulate to negative value
				ModAmount = SampleDelay - 1;


			auto totalDelay = SampleDelay + ModAmount * mod;
			
			if (totalDelay <= 0) // should no longer be required
				totalDelay = 1;

			delayA = (int)totalDelay;
			delayB = (int)totalDelay + 1;

			auto partial = totalDelay - delayA;

			gainA = 1 - partial;
			gainB = partial;

			samplesProcessed = 0;
		}

	};
}

#endif