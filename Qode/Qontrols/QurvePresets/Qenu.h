#pragma once

#include "Qommon.h"
#include "DSP/Qurve.h"

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
  enum class QAnimation
  {
    qNothing = 0,
    qBrighten,
    qDim,
  } qAnimation = QAnimation::qNothing;

  //***************************************************************************************************************************************
  float smoothedValue = 0;
  Smoothing<float> smoothAnimation{0, 0};

  //***************************************************************************************************************************************
  void animateQontrol();
  void cheqSelectedPreset(int _index);
  void setupAnimation(float _duationInMS, float _minValue = 0, float _maxValue = 1);
};