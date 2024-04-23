#include "QuietProcessing.h"

//*****************************************************************************************************************************************
QuietProcessing::QuietProcessing(const Qurve& _qurve)
  : qurve(_qurve)
{
  // These MUST exist as long as the plugin exists!
  inputBuffer = new float[maxResolution]{0};
  outputBuffer = new float[maxResolution]{0};
  sidechainBuffer = new float[maxResolution]{0};
}

//*****************************************************************************************************************************************
QuietProcessing::~QuietProcessing()
{
  // These MUST exist as long as the plugin exists!
  delete[] inputBuffer;
  inputBuffer = nullptr;

  delete[] outputBuffer;
  outputBuffer = nullptr;

  delete[] sidechainBuffer;
  sidechainBuffer = nullptr;
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

  // clean buffers...
  for (int i = 0; i < maxResolution; i++)
  {
    inputBuffer[i] = 0;
    outputBuffer[i] = 0;
    sidechainBuffer[i] = 0;
  }
}

//*****************************************************************************************************************************************
void QuietProcessing::setMIDITriggered(bool _MIDITriggered)
{
  MIDITriggered = _MIDITriggered;
  phase = 0;
}

//*****************************************************************************************************************************************
bool QuietProcessing::isMIDITriggered() const
{
  return MIDITriggered;
}

//*****************************************************************************************************************************************
void QuietProcessing::addMIDIMessage(const iplug::IMidiMsg& _msg)
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
void QuietProcessing::setLookAhead(unsigned int _latency)
{
  if (_latency)
  {
    for (int c = 0; c < ioChannels; c++)
      ioLookAhead[c].resize(_latency + 1);

    sidechainLookAhead.resize(_latency + 1);
  }
  else
  {
    for (int c = 0; c < ioChannels; c++)
      ioLookAhead[c].resize(0);

    sidechainLookAhead.resize(0);
  }
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
float* QuietProcessing::getSc()
{
  return sidechainBuffer;
}

//*****************************************************************************************************************************************
float* QuietProcessing::getIns()
{
  return inputBuffer;
}

//*****************************************************************************************************************************************
float* QuietProcessing::getOuts()
{
  return outputBuffer;
}

//*****************************************************************************************************************************************
double& QuietProcessing::getPhase()
{
  return phase;
}

//*****************************************************************************************************************************************
bool& QuietProcessing::avoidInputs()
{
  return lookingAhead;
}

//*****************************************************************************************************************************************
void QuietProcessing::onReset(double _sampleRate, uint32_t _latency)
{
  sampleRate = _sampleRate;
  depthSmoothing.setSmoothingTime(2.36, sampleRate);
  setLookAhead(_latency);
}

//*****************************************************************************************************************************************
bool QuietProcessing::processBlock(double** _inputs, double** _outputs, int _frames, const iplug::ITimeInfo& _timeInfo, bool _showSidechain, bool _showSqope)
{
  if (_timeInfo.mTransportIsRunning)
  {
    double numBeats = 1 / rate;
    double samplesPerBeat = sampleRate * (60 / (_timeInfo.mTempo == 0 ? 1 : _timeInfo.mTempo));
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
            updateSqopeL(static_cast<float>(_outputs[0][s]), static_cast<float>(sidechainLookAhead.read()));
          }
          else
            updateSqope(static_cast<float>(_inputs[0][s]), static_cast<float>(_outputs[0][s])); // ...could use mono...
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
            updateSqope(static_cast<float>(_inputs[0][s]), static_cast<float>(_outputs[0][s]), static_cast<float>(_inputs[2][s])); // ...or mono?
          else
            updateSqope(static_cast<float>(_inputs[0][s]), static_cast<float>(_outputs[0][s])); // ...could use mono
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

    phase = 0;
    return true;
  }
  else
  {
    for (int s = 0; s < _frames; s++)
      for (int c = 0; c < ioChannels; c++)
        _outputs[c][s] = _inputs[c][s];

    phase = 0;
    return false;
  }
}

//*****************************************************************************************************************************************
void QuietProcessing::processMIDI(int _sampleIndex)
{
  while (!MIDIQ.Empty())
  {
    iplug::IMidiMsg* pMsg = &MIDIQ.Peek();

    if (pMsg->mOffset > _sampleIndex)
      break;

    int status = pMsg->StatusMsg();

    switch (status)
    {
    case iplug::IMidiMsg::kNoteOn :
      if (status == iplug::IMidiMsg::kNoteOn)
      {
        if (tempoSynqd)
          sampleCounter = 0;

        cycle = true;
        phase = 0;

        if (useVelocity)
          noteVelocity = pMsg->Velocity() / 127.0;
      }

      break;
    }

    MIDIQ.Remove();
  }
}

//*****************************************************************************************************************************************
double QuietProcessing::getQurveValueAtPhase(double _phase)
{
  double smoothDepth = depthSmoothing.smoothen(depth);
  double xAxis = wrapAround(_phase - phaseShift, 0, 1);
  double yAxis = static_cast<double>(qurve.getValueAtPhase(static_cast<float>(xAxis)));
  
  if (useVelocity)
    smoothDepth *= noteVelocity;

  return (qurveSmoothing.smoothen(yAxis) * smoothDepth) + (1 - smoothDepth);
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
void QuietProcessing::updateSqope(float _input, float _output)
{
   int discretePhase = static_cast<int>(phase * maxResolution) % maxResolution;

   inputBuffer[discretePhase] = _input;
   outputBuffer[discretePhase] = _output;
}

//*****************************************************************************************************************************************
void QuietProcessing::updateSqopeL(float _output, float _sidechain)
{
   int discretePhase = static_cast<int>(phase * maxResolution) % maxResolution;

   outputBuffer[discretePhase] = _output;
   sidechainBuffer[discretePhase] = _sidechain;
}

//*****************************************************************************************************************************************
void QuietProcessing::updateSqope(float _input, float _output, float _sidechain)
{
  int discretePhase = static_cast<int>(phase * maxResolution) % maxResolution;

  inputBuffer[discretePhase] = _input;
  outputBuffer[discretePhase] = _output;
  sidechainBuffer[discretePhase] = _sidechain;
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
          phase = 1; // hmmmm...
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
        if (phase + frequency >= 1)
        {
          cycle = false;
          phase = 1;
        }
        else
          phase = wrapAround(phase + frequency, 0, 1);
      }
    }
    else
      phase = wrapAround(phase + frequency, 0, 1);
  }
}