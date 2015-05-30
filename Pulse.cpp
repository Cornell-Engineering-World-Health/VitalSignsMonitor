#include "Arduino.h"
#include "Pulse.h"

Pulse::Pulse(int pin, int pin2){
  _pin = pin;
  _pin2 = pin2;
  NumSample = 300;
  sampleRate = 10;
}

void Pulse::ClearArrays(){
  Serial.println("Clearing Array");
    for(int i = 0; i < NumSample; i++){
      DC_Value[i] = 0;
    }
    
    for(int i = 0; i < NumSample; i++){
      DC_Value2[i] = 0;
    }
    
    for(int i = 0; i < 10; i++){
      RisingEdges[i] = 0;
    }
    
    for(int i = 0; i < 10; i++){
      RisingEdges2[i] = 0;
    }
}

void Pulse::ReadSample(){
  Serial.println("Start reading samples");
  index = 0;
  while(index < NumSample){
    delay(sampleRate);
    DC_Value[index] = digitalRead(_pin);
    DC_Value2[index] = digitalRead(_pin2); //for the second pulse
    index++;
  }
}

void Pulse::GetRisingEdge(int samples[], int risingEdge[]){
  Serial.println("Getting rising edge");
  int value;
  int nextVal;
  int edgeIndex = 0;
  for(int i = 1; i < NumSample; i++){
    value = samples[i - 1];
    nextVal = samples[i];
    if(value == 0 && nextVal == 1){
      risingEdge[edgeIndex] = i;
      edgeIndex++;
    }
  }
}

int Pulse::GetNumElements(int edges[]){
    for(int i = 0; i < 10; i++){
      if(edges[i] == 0){
        return i;
      }
    }
}

float Pulse::ComputeHR(int edges[]){
  //count the number of edges first
  int numEdges = GetNumElements(edges);
  Serial.print("Number of edges = ");
  Serial.println(numEdges);
  
  for(int i = 1; i < numEdges; i++){
     float diff = (edges[i] - edges[i - 1])*sampleRate; //in ms
     float diffmin = diff/1000/60; //in min
     float HR = 1/diffmin;
     HeartRates[i - 1] = HR;
     Serial.print("HR: ");
     Serial.println(HR);
  }
  
  int numHR = numEdges - 1; // number of HR calculated will be one less than #rising edge 
  float total = 0;
  for(int i = 0; i < numHR; i++){
    total = total + HeartRates[i];
  }
  
  avgHR = total/numHR;
  Serial.print("Avg HR: ");
  Serial.println(avgHR);
  
  return avgHR;
}

float Pulse::ComputePTT(){
  int numEdges = GetNumElements(RisingEdges);
  int numEdges2 = GetNumElements(RisingEdges2);
  int numEdge = min(numEdges,numEdges2);
  Serial.print("Number of edges for PTT = ");
  Serial.println(numEdge);
  
  float total = 0;
  for(int i = 0; i < numEdge; i++){
    float ptt = abs(RisingEdges[i] - RisingEdges2[i]);
    total = total + ptt;
  }
  avgPTT = total/numEdge;
  Serial.print("Average PTT = ");
  Serial.println(avgPTT);
}

float Pulse::GetPulse(){
  Serial.println("GetPulse() in .cpp");
  ClearArrays();
  ReadSample();
  GetRisingEdge(DC_Value, RisingEdges);
  
  return ComputeHR(RisingEdges);;
}

float Pulse::GetPTT(){
  ClearArrays();
  ReadSample();
  GetRisingEdge(DC_Value, RisingEdges);
  GetRisingEdge(DC_Value2, RisingEdges2);
  
  return ComputePTT();
}
