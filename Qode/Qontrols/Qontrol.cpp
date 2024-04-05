#include "Qontrol.h"

//*****************************************************************************************************************************************
Qontrol::Qontrol(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq)
  : IControl(_bounds, _paramIdx, _actionFunq)
  , qheme(_qheme)
  , smoothAnimation(0, 0)
{}

//*****************************************************************************************************************************************
void Qontrol::OnMouseOver(float _x, float _y, const IMouseMod& _mod)
{
  qAnimation = qBrighten;

  SetDirty(false);
}

//*****************************************************************************************************************************************
void Qontrol::OnMouseOut()
{
  qAnimation = qDim;

  SetDirty(false);
}

//*****************************************************************************************************************************************
void Qontrol::setupAnimation(float _duationInMS, float _startValue, float _endValue)
{
  smoothedValue = _startValue;
  smoothAnimation = Smoothing<float>(_startValue, _duationInMS, true, _startValue, _endValue);
}

//*****************************************************************************************************************************************
void Qontrol::animateQontrol()
{
  if (qAnimation == qBrighten)
  {
    if (smoothedValue == smoothAnimation.getMaxValue())
      qAnimation = qNothing;
    else
    {
      smoothedValue = smoothAnimation.smoothen(smoothAnimation.getMaxValue());

      SetDirty(false);
    }
  }
  else if (qAnimation == qDim)
  {
    if (smoothedValue == smoothAnimation.getMinValue())
      qAnimation = qDim;
    else
    {
      smoothedValue = smoothAnimation.smoothen(smoothAnimation.getMinValue());

      SetDirty(false);
    }
  }
}