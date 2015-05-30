#ifndef Pulse_h
#define Pulse_h

#include "Arduino.h"

class Pulse
{
  	public:
  		Pulse(int pin, int pin2);
  		void ClearArrays();
                void ReadSample();
                void GetRisingEdge(int[],int[]);
                int GetNumElements(int[]);
                float ComputeHR(int[]);
                float ComputePTT();
                float GetPulse();
                float GetPTT();
  	private:
  		int DC_Value[300];
                int DC_Value2[300];
                int RisingEdges[10];
                int RisingEdges2[10];
                float HeartRates[10];
                int index;
                int NumSample;
                int sampleRate;
                float avgHR;
                float avgPTT;
                int _pin;
                int _pin2;
};

#endif
