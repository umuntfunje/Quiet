#pragma once

#include "Qurve.h"

//*****************************************************************************************************************************************
class QuietProcessing
{
public:
  //***************************************************************************************************************************************
  QuietProcessing(const Qurve&);

  void setDepth(double);

  void setRate(double);
  void setFrequency(double);
  void setPhaseShift(double);

  void setTempoSynqd(bool);

  void setMIDITriggered(bool);
  bool isMIDITriggered() const;
  void addMIDIMessage(const IMidiMsg&);

  void setUseVelocity(bool);

  void setQurveSmoothing(double);

  void setLookAhead(int);
  void setLookingAhead(bool);
  bool isLookingAhead() const;

  // for updating the sqope...
  double& getPhase();
  sampleBuffer getIns();
  sampleBuffer getOuts();
  sampleBuffer getSc();

  void reset(double, int);
  bool processBlock(sample**, sample**, int, const ITimeInfo&, bool, bool);

private:
  //***************************************************************************************************************************************
  const Qurve& qurve;
  double phase = .0;
  double quietValue = 1.;
  double sampleRate = 44100.;

  //***************************************************************************************************************************************
  double rate = 1.;
  double depth = 1.;
  double frequency = 20.;
  double phaseShift = .0;
  Smoothing<double> depthSmoothing{1., 2.36};

  //***************************************************************************************************************************************
  bool cycle = false;
  int sampleCounter = 0;
  bool tempoSynqd = true;

  //***************************************************************************************************************************************
  IMidiQueue MIDIQ;
  bool useVelocity = false;
  double noteVelocity = 1.;
  bool MIDITriggered = false;

  //***************************************************************************************************************************************
  Smoothing<double> qurveSmoothing{1., .0, sampleRate, true};

  //***************************************************************************************************************************************
  bool hasTail = true;
  int tailCounter = 0;
  bool lookingAhead = false;
  CircularBuffer<sample> lookAhead[outputChannels], scLookAhead;

  //***************************************************************************************************************************************
  std::vector<float> ins, outs, sc;

  //***************************************************************************************************************************************
  void processMIDI(int);
  double getQurveValueAtPhase(double);
  double wrapAround(double, double, double);
  void updateSqope(const sample&, const sample&);
  void updateSqope(const sample&, const sample&, const sample&);
  inline void calculatePhase(int, double, double, double, double);
};