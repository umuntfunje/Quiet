#pragma once

#include "Qheme.h"

//*****************************************************************************************************************************************
// Animated Qontrols...
class Qontrol : public IControl
{
public:
  //***************************************************************************************************************************************
  Qontrol(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx = kNoParameter, IActionFunction _actionFunq = nullptr);

  void OnMouseOver(float, float, const IMouseMod&) override;
  void OnMouseOut() override;

protected:
  //***************************************************************************************************************************************
  const Qheme& qheme;
  float smoothedValue = .0f;

  //***************************************************************************************************************************************
  enum QAnimation
  {
    qDim = 0,
    qBrighten,
    qNothing
  } qAnimation = qNothing;

  //***************************************************************************************************************************************
  void setupAnimation(float _duationInMS, float _minValue = .0f, float _maxValue = 1.f);
  void animateQontrol();

private:
  //***************************************************************************************************************************************
  Smoothing<float> smoothAnimation;
};