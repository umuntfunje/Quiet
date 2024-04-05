#pragma once

#include "Qheme.h"
#include "Qurve.h"

//*****************************************************************************************************************************************
// Edited IDirBrowseControlBase in IControl.h
class Qenu : public QDirBrowseControlBase
{
public:
  //***************************************************************************************************************************************
  Qenu(const IRECT& _bounds, Qurve& _qurve, const Qheme& _qheme, IActionFunction _actionFunq);

  void Draw(IGraphics& _qui) override;

  void OnPopupMenuSelection(IPopupMenu* _selectedQenu, int _valIdx) override;

  void OnMouseOver(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;
  void OnMouseOut() override;

  void setPresetName(const char* _presetName, bool _setDirty = false);

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
  Smoothing<float> glowSmoother{glow, .251f, true, glow, .081f};

  //***************************************************************************************************************************************
  void cheqSelectedPreset(int _index);
};