#pragma once

#include "Qommon.h"

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
  float smoothedValue = 0;

  //***************************************************************************************************************************************
  enum class QAnimation
  {
    qNothing = 0,
    qBrighten,
    qDim,
  } qAnimation = QAnimation::qNothing;

  //***************************************************************************************************************************************
  void animateQontrol();
  void setupAnimation(float _duationInMS, float _minValue = 0, float _maxValue = 1);

private:
  //***************************************************************************************************************************************
  Smoothing<float> smoothAnimation{0, 0};
};