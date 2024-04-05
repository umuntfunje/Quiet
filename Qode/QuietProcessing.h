#pragma once

#include "Qurve.h"

//*****************************************************************************************************************************************
class QuietProcessing
{
public:
  //***************************************************************************************************************************************
  QuietProcessing(const Qurve& _qurve);

  void setDepth(double _depth);

  void setRate(double _rate);
  void setFrequency(double _frequency);
  void setPhaseShift(double _phaseShift);

  void setTempoSynqd(bool _tempoSycd);

  void setMIDITriggered(bool _MIDITriggered);
  bool isMIDITriggered() const;
  void addMIDIMessage(const IMidiMsg& _msg);

  void setUseVelocity(bool _useVelocity);

  void setQurveSmoothing(double _timeInMS);

  void setLookAhead(int _latency);
  void setLookingAhead(bool _lookingAhead);
  bool isLookingAhead() const;

  // for updating the 'Sqope'...
  double& getPhase();
  SampleBuffer& getIns();
  SampleBuffer& getOuts();
  SampleBuffer& getSc();

  void reset(double _sampleRate, int _latency);
  bool processBlock(sample** _inputs, sample** _outputs, int _frames, const ITimeInfo& _timeInfo, bool _showSidechain, bool _showSqope);

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
  Smoothing<double> qurveSmoothing{1., .0, true};

  //***************************************************************************************************************************************
  bool hasTail = true;
  int tailCounter = 0;
  bool lookingAhead = false;
  CircularBuffer<sample> ioLookAhead[ioChannels], sidechainLookAhead;

  //***************************************************************************************************************************************
  SampleBuffer inputBuffer, outputBuffer, sidechainBuffer;

  //***************************************************************************************************************************************
  void processMIDI(int _sampleIndex);
  double getQurveValueAtPhase(double _phase);
  double wrapAround(double _value, double _min, double _max);
  void updateSqope(const sample& _input, const sample& _output);
  void updateSqope(const sample& _input, const sample& _output, const sample& _sidechain);
  inline void calculatePhase(int _sampleIndex, double _numBeats, double _samplesPerCycle, double _samplesPerBeat, double _PPQPos);
};