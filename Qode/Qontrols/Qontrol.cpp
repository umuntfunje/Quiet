#include "Qontrol.h"

//*****************************************************************************************************************************************
Qontrol::Qontrol(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq)
  : IControl(_bounds, _paramIdx, _actionFunq)
  , qheme(_qheme)
{}

//*****************************************************************************************************************************************
void Qontrol::OnMouseOver(float _x, float _y, const IMouseMod& _mod)
{
  qAnimation = QAnimation::qBrighten;
  SetDirty(false);
}

//*****************************************************************************************************************************************
void Qontrol::OnMouseOut()
{
  qAnimation = QAnimation::qDim;
  SetDirty(false);
}

//*****************************************************************************************************************************************
void Qontrol::animateQontrol()
{
  switch (qAnimation)
  {
  case QAnimation::qNothing:
    break;

  case QAnimation::qBrighten:
    if (smoothedValue == smoothAnimation.getMaxValue())
      qAnimation = QAnimation::qNothing;
    else
    {
      smoothedValue = smoothAnimation.smoothen(smoothAnimation.getMaxValue());
      SetDirty(false);
    }
    break;

  case QAnimation::qDim:
    if (smoothedValue == smoothAnimation.getMinValue())
      qAnimation = QAnimation::qNothing;
    else
    {
      smoothedValue = smoothAnimation.smoothen(smoothAnimation.getMinValue());
      SetDirty(false);
    }
    break;
  }
}

//*****************************************************************************************************************************************
void Qontrol::setupAnimation(float _duationInMS, float _startValue, float _endValue)
{
  smoothedValue = _startValue;
  smoothAnimation = Smoothing<float>(_startValue, _duationInMS, true, _startValue, _endValue);
}