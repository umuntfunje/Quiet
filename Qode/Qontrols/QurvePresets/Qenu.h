#pragma once

#include "../../Qurve.h"
#include "../../Qheme.h"

//*****************************************************************************************************************************************
// Edited IDirBrowseControlBase in IControl.h
class Qenu : public QDirBrowseControlBase
{
public:
  //***************************************************************************************************************************************
  Qenu(const IRECT& _bounds, Qurve& _qurve, const Qheme& _qheme, IActionFunction _actionFunq);

  void Draw(IGraphics&) override;

  void OnPopupMenuSelection(IPopupMenu*, int) override;

  void OnMouseOver(float, float, const IMouseMod&) override;
  void OnMouseDown(float, float, const IMouseMod&) override;
  void OnMouseOut() override;

  void setPresetName(const char*, bool _setDirty = false);

  void savePreset();
  void loadNextPreset();
  void loadPreviousPreset();
  void loadPresetAtCurrentIndex(bool _initializing = false);

private:
  //***************************************************************************************************************************************
  Qurve& qurve;
  const Qheme& qheme;
  WDL_String currentPresetName;

  //***************************************************************************************************************************************
  float glow = .023f;
  bool isRising = false;
  Smoothing<float> glowSmoother{glow, .251f, 44100.f, true, glow, .081f};

  //***************************************************************************************************************************************
  void cheqSelectedPreset(int);
};