#pragma once

#include "Qode/Qheme.h"
#include "Qode/QuietProcessing.h"
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
  qCtrlTagBaq = 0,
  qCtrlTagSqope,
  qCtrlTagQnodeSys,
  qCtrlTagUndo,
  qCtrlTagQenu,
  qCtrlTagRedo,
  qCtrlTagInfo
};

//*****************************************************************************************************************************************
class Quiet final : public Plugin
{
public:
  //***************************************************************************************************************************************
  Quiet(const InstanceInfo&);

  void OnIdle() override;
  void OnReset() override;
  void OnParamChange(int) override;
  void OnParamChangeUI(int, EParamSource) override;
  void ProcessBlock(sample**, sample**, int) override;
  void ProcessMidiMsg(const IMidiMsg&) override;

  bool SerializeState(IByteChunk&) const override;
  int UnserializeState(const IByteChunk&, int) override;

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
  void turnLookAheadOff(bool);
  bool addAsterisk(std::string&);
  void updateQenuTheme(IGraphics*);
  void updateUndoRedoQuttons(const IGraphics*);
};