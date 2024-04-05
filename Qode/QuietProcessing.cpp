#include "QuietProcessing.h"

//*****************************************************************************************************************************************
QuietProcessing::QuietProcessing(const Qurve& _qurve)
  : qurve(_qurve)
{
  inputBuffer.resize(maxResolution, 0);
  outputBuffer.resize(maxResolution, 0);
  sidechainBuffer.resize(maxResolution, 0);
}

//*****************************************************************************************************************************************
void QuietProcessing::setDepth(double _depth)
{
  depth = _depth;
}

//*****************************************************************************************************************************************
void QuietProcessing::setRate(double _rate)
{
  rate = _rate;
}

//*****************************************************************************************************************************************
void QuietProcessing::setFrequency(double _frequency)
{
  frequency = _frequency;
}

//*****************************************************************************************************************************************
void QuietProcessing::setPhaseShift(double _phaseShift)
{
  phaseShift = _phaseShift;
}

//*****************************************************************************************************************************************
void QuietProcessing::setTempoSynqd(bool _tempoSynqd)
{
  tempoSynqd = _tempoSynqd;

  // Clear the 'Sqope' buffers
  inputBuffer.clear();
  outputBuffer.clear();
  sidechainBuffer.clear();
  inputBuffer.resize(maxResolution, 0);
  outputBuffer.resize(maxResolution, 0);
  sidechainBuffer.resize(maxResolution, 0);
}

//*****************************************************************************************************************************************
void QuietProcessing::setMIDITriggered(bool _MIDITriggered)
{
  MIDITriggered = _MIDITriggered;
  phase = .0;
}

//*****************************************************************************************************************************************
bool QuietProcessing::isMIDITriggered() const
{
  return MIDITriggered;
}

//*****************************************************************************************************************************************
void QuietProcessing::addMIDIMessage(const IMidiMsg& _msg)
{
  MIDIQ.Add(_msg);
}

//*****************************************************************************************************************************************
void QuietProcessing::setUseVelocity(bool _useVelocity)
{
  useVelocity = _useVelocity;
}

//*****************************************************************************************************************************************
void QuietProcessing::setQurveSmoothing(double _timeInMS)
{
  qurveSmoothing.setSmoothingTime(_timeInMS, sampleRate);
}

//*****************************************************************************************************************************************
void QuietProcessing::setLookAhead(int _latency)
{
  for (int c = 0; c < ioChannels; c++)
    ioLookAhead[c].resize(_latency + 1);

  sidechainLookAhead.resize(_latency + 1);
}

//*****************************************************************************************************************************************
void QuietProcessing::setLookingAhead(bool _lookingAhead)
{
  if (_lookingAhead == lookingAhead)
    return;

  lookingAhead = _lookingAhead;
}

//*****************************************************************************************************************************************
bool QuietProcessing::isLookingAhead() const
{
  return lookingAhead;
}

//*****************************************************************************************************************************************
double& QuietProcessing::getPhase()
{
  return phase;
}

//*****************************************************************************************************************************************
SampleBuffer& QuietProcessing::getIns()
{
  return inputBuffer;
}

//*****************************************************************************************************************************************
SampleBuffer& QuietProcessing::getOuts()
{
  return outputBuffer;
}

//*****************************************************************************************************************************************
SampleBuffer& QuietProcessing::getSc()
{
  return sidechainBuffer;
}

//*****************************************************************************************************************************************
void QuietProcessing::reset(double _sampleRate, int _latency)
{
  sampleRate = _sampleRate;
  depthSmoothing.setSmoothingTime(2.36, _sampleRate);

  for (int c = 0; c < ioChannels; c++)
    ioLookAhead[c].resize(_latency + 1);

  sidechainLookAhead.resize(_latency + 1);
}

//*****************************************************************************************************************************************
bool QuietProcessing::processBlock(sample** _inputs, sample** _outputs, int _frames, const ITimeInfo& _timeInfo, bool _showSidechain, bool _showSqope)
{
  if (_timeInfo.mTransportIsRunning)
  {
    double numBeats = 1. / rate;
    double samplesPerBeat = sampleRate * (60. / (_timeInfo.mTempo == .0 ? 1. : _timeInfo.mTempo));
    double samplesPerCycle = samplesPerBeat * numBeats;
  
    if (!hasTail)
      hasTail = true;
  
    if (lookingAhead)
      for (int s = 0; s < _frames; s++)
      {
        calculatePhase(s, numBeats, samplesPerCycle, samplesPerBeat, _timeInfo.mPPQPos);
        quietValue = getQurveValueAtPhase(phase);

        for (int c = 0; c < ioChannels; c++)
        {
          ioLookAhead[c].write(_inputs[c][s]);
          _outputs[c][s] = ioLookAhead[c].read() * quietValue;
        }

        if (MIDITriggered && !cycle)  // ...fix: some weird shit on the GUI when done with cycle...
          continue;
  
        if (_showSqope)
        {
          if (_showSidechain)
          {
            sidechainLookAhead.write(_inputs[2][s]);
            updateSqope(_inputs[0][s], _outputs[0][s], sidechainLookAhead.read()); // ...could use mono...
          }
          else
            updateSqope(_inputs[0][s], _outputs[0][s]); // ...could use mono...
        }
      }
    else
      for (int s = 0; s < _frames; s++)
      {
        calculatePhase(s, numBeats, samplesPerCycle, samplesPerBeat, _timeInfo.mPPQPos);
        quietValue = getQurveValueAtPhase(phase);
  
        for (int c = 0; c < ioChannels; c++)
          _outputs[c][s] = _inputs[c][s] * quietValue;

        if (MIDITriggered && !cycle)  // ...fix: some weird shit on the GUI when done with cycle...
          continue;

        if (_showSqope)
        {
          if (_showSidechain)
            updateSqope(_inputs[0][s], _outputs[0][s], _inputs[2][s]); // ...could use mono...
          else
            updateSqope(_inputs[0][s], _outputs[0][s]); // ...could use mono...
        }
      }
  
    if (MIDITriggered)
      MIDIQ.Flush(_frames);
  
    return true;
  }
  else if (hasTail)
  {
    if (lookingAhead)
    {
      for (int s = 0; s < _frames; s++)
        for (int c = 0; c < ioChannels; c++)
        {
          ioLookAhead[c].write(_inputs[c][s]);
          _outputs[c][s] = ioLookAhead[c].read() * quietValue;
        }
  
      tailCounter += _frames;
  
      if (tailCounter >= ioLookAhead[0].getSize() + _frames)
      {
        hasTail = false;
        tailCounter = 0;
  
        for (int c = 0; c < ioChannels; c++)
          ioLookAhead[c].clean();
      }
    }
    else
    {
      for (int s = 0; s < _frames; s++)
        for (int c = 0; c < ioChannels; c++)
          _outputs[c][s] = _inputs[c][s] * quietValue;
  
      hasTail = false;
    }

    phase = .0;
    return true;
  }
  else
  {
    for (int s = 0; s < _frames; s++)
      for (int c = 0; c < ioChannels; c++)
        _outputs[c][s] = _inputs[c][s];

    phase = .0;
    return false;
  }
}

//*****************************************************************************************************************************************
void QuietProcessing::processMIDI(int _sampleIndex)
{
  while (!MIDIQ.Empty())
  {
    IMidiMsg* pMsg = &MIDIQ.Peek();

    if (pMsg->mOffset > _sampleIndex)
      break;

    int status = pMsg->StatusMsg();

    switch (status)
    {
    case IMidiMsg::kNoteOn:
      if (status == IMidiMsg::kNoteOn)
      {
        if (tempoSynqd)
          sampleCounter = 0;

        cycle = true;
        phase = .0;

        if (useVelocity)
          noteVelocity = pMsg->Velocity() / static_cast<double>(127);
      }

      break;
    }

    MIDIQ.Remove();
  }
}

//*****************************************************************************************************************************************
double QuietProcessing::getQurveValueAtPhase(double _phase)
{
  double xAxis = wrapAround(_phase - phaseShift, .0, 1.);
  double yAxis = static_cast<double>(qurve.getValueAtPhase(static_cast<float>(xAxis)));
  double smoothDepth = useVelocity && MIDITriggered ? depthSmoothing.smoothen(depth * noteVelocity) : depthSmoothing.smoothen(depth);
  
  return (qurveSmoothing.smoothen(yAxis) * smoothDepth) + (1. - smoothDepth);
}

//*****************************************************************************************************************************************
double QuietProcessing::wrapAround(double _value, double _min, double _max)
{
  while (_value > _max)
    _value -= _max;

  while (_value < _min)
    _value += (_max - _min);

  return _value;
}

//*****************************************************************************************************************************************
void QuietProcessing::updateSqope(const sample& _input, const sample& _output)
{
   int discretePhase = static_cast<int>(phase * maxResolution) % maxResolution;

   inputBuffer[discretePhase] = static_cast<float>(_input);
   outputBuffer[discretePhase] = static_cast<float>(_output);
}

//*****************************************************************************************************************************************
void QuietProcessing::updateSqope(const sample& _input, const sample& _output, const sample& _sidechain)
{
  int discretePhase = static_cast<int>(phase * maxResolution) % maxResolution;

  inputBuffer[discretePhase] = static_cast<float>(_input);
  outputBuffer[discretePhase] = static_cast<float>(_output);
  sidechainBuffer[discretePhase] = static_cast<float>(_sidechain);
}

//*****************************************************************************************************************************************
inline void QuietProcessing::calculatePhase(int _sampleIndex, double _numBeats, double _samplesPerCycle, double _samplesPerBeat, double _PPQPos)
{
  if (tempoSynqd)
  {
    if (MIDITriggered)
    {
      processMIDI(_sampleIndex);

      if (cycle)
      {
        phase = static_cast<double>(sampleCounter) / _samplesPerCycle;
        sampleCounter++;

        if (_samplesPerCycle < sampleCounter)
        {
          cycle = false;
          phase = 1.; // hmmmm...
        }
      }
    }
    else
      phase = std::fmod(_PPQPos + (static_cast<double>(_sampleIndex) / _samplesPerBeat), _numBeats) / _numBeats;
  }
  else
  {
    if (MIDITriggered)
    {
      processMIDI(_sampleIndex);

      if (cycle)
      {
        if (phase + frequency >= 1.)
        {
          cycle = false;
          phase = 1.;
        }
        else
          phase = wrapAround(phase + frequency, .0, 1.);
      }
    }
    else
      phase = wrapAround(phase + frequency, .0, 1.);
  }
}