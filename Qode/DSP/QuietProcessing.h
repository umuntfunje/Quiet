#pragma once

#include "Qurve.h"
#include "IPlugStructs.h"
#include "Qtilities/Qonstants.h"
#include "Qtilities/Smoothing.h"
#include "Qtilities/CircularBuffer.h"

//*****************************************************************************************************************************************
class QuietProcessing
{
public:
  //***************************************************************************************************************************************
  QuietProcessing(const Qurve& _qurve);
  ~QuietProcessing();

  void setDepth(double _depth);

  void setRate(double _rate);
  void setFrequency(double _frequency);
  void setPhaseShift(double _phaseShift);

  void setTempoSynqd(bool _tempoSycd);

  void setMIDITriggered(bool _MIDITriggered);
  bool isMIDITriggered() const;
  void addMIDIMessage(const iplug::IMidiMsg& _msg);

  void setUseVelocity(bool _useVelocity);

  void setQurveSmoothing(double _timeInMS);

  void setLookAhead(unsigned int _latency);
  void setLookingAhead(bool _lookingAhead);
  bool isLookingAhead() const;

  // for updating the 'Sqope'...
  float* getSc();
  float* getIns();
  float* getOuts();
  double& getPhase();
  bool& avoidInputs();

  void onReset(double _sampleRate, uint32_t _latency);
  bool processBlock(double** _inputs, double** _outputs, int _frames, const iplug::ITimeInfo& _timeInfo, bool _showSidechain, bool _showSqope);

private:
  //***************************************************************************************************************************************
  const Qurve& qurve;
  double phase = 0;
  double quietValue = 1;
  double sampleRate = 44100;

  //***************************************************************************************************************************************
  double rate = 1;
  double depth = 1;
  double frequency = 20;
  double phaseShift = 0;
  Smoothing<double> depthSmoothing{1, 2.36};

  //***************************************************************************************************************************************
  bool cycle = false;
  int sampleCounter = 0;
  bool tempoSynqd = true;

  //***************************************************************************************************************************************
  iplug::IMidiQueue MIDIQ;
  double noteVelocity = 1;
  bool useVelocity = false;
  bool MIDITriggered = false;

  //***************************************************************************************************************************************
  Smoothing<double> qurveSmoothing{1, 0, true};

  //***************************************************************************************************************************************
  bool hasTail = true;
  int tailCounter = 0;
  bool lookingAhead = false;
  CircularBuffer<double> ioLookAhead[ioChannels], sidechainLookAhead;

  //***************************************************************************************************************************************
  float *inputBuffer = nullptr, *outputBuffer = nullptr, *sidechainBuffer = nullptr;  // Sqope buffer

  //***************************************************************************************************************************************
  void processMIDI(int _sampleIndex);
  double getQurveValueAtPhase(double _phase);
  double wrapAround(double _value, double _min, double _max);
  void updateSqope(float _input, float _output);
  void updateSqopeL(float _output, float _sidechain); // avoid weird shift on GUI (from inputs)
  void updateSqope(float _input, float _output, float _sidechain);
  void calculatePhase(int _sampleIndex, double _numBeats, double _samplesPerCycle, double _samplesPerBeat, double _PPQPos);
};