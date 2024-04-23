#pragma once

#include "Qommon.h"
#include "DSP/QuietProcessing.h"
#include "IPlug_include_in_plug_hdr.h"

const int qNumPresets = 1;

enum QParams
{
  qDepth = 0,
  qRate,
  qFrequency,
  qSnap,
  qPhaseShift,
  qSync,
  qMIDI,
  qSmoothing,
  qWaveform,
  qTheme,
  qLookahead,
  qVelocity,
  qNumParams
};

enum QontrolTags
{
  QtrlTagBaq = 0,
  QtrlTagSqope,
  QtrlTagQnodeSys,
  QtrlTagUndo,
  QtrlTagQenu,
  QtrlTagRedo,
  QtrlTagInfo
};

//*****************************************************************************************************************************************
class Quiet final : public Plugin
{
public:
  //***************************************************************************************************************************************
  Quiet(const InstanceInfo&);

  void OnIdle() override;
  void OnReset() override;
  void OnParamChange(int _paramIdx) override;
  void OnParamChangeUI(int _paramIdx, EParamSource _eps) override;
  void ProcessBlock(sample** _inputs, sample** _outputs, int _frames) override;
  void ProcessMidiMsg(const IMidiMsg& _msg) override;

  bool SerializeState(IByteChunk& _chunk) const override;
  int UnserializeState(const IByteChunk& _chunk, int _startPosition) override;

private:
  //***************************************************************************************************************************************
  Qurve qurve;
  QuietProcessing qp{qurve};

  //***************************************************************************************************************************************
  Qheme qheme;
  bool transporting = false;
  bool showSidechain = false;

  //***************************************************************************************************************************************
  void updateLookAhead();
  void turnLookAheadOff(bool _alreadyOff);
  bool addAsterisk(std::string& _presetName);
  void updateQenuTheme(IGraphics* _qui);
  void updateUndoRedoQuttons(const IGraphics* _qui);
};