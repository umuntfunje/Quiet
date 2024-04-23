#include "Qlider.h"

//*****************************************************************************************************************************************
Qlider::Qlider(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq)
  : Qontrol(_bounds, _qheme, _paramIdx, _actionFunq)
{
  setupAnimation(0.252f, 0.67f);
}

//*****************************************************************************************************************************************
void Qlider::Draw(IGraphics& _qui)
{
  drawQlider(_qui, qheme.foreGround.WithOpacity(smoothedValue));
  animateQontrol();  
}

//*****************************************************************************************************************************************
void Qlider::OnResize()
{
  float _fraction = 0.173f;
  float _ratio = mRECT.H() / mRECT.W();
  float x = mRECT.L + _fraction * _ratio * mRECT.W();
  float y = mRECT.T + _fraction * mRECT.H();
  float w = (1 - 2 * _fraction * _ratio) * mRECT.W();
  float h = (1 - 2 * _fraction) * mRECT.H();

  SetTargetRECT(IRECT::MakeXYWH(x, y, w, h));
  
  SetDirty(false);
}

//*****************************************************************************************************************************************
void Qlider::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    mouseDragValue = GetValue();
    GetUI()->HideMouseCursor(true, true);
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void Qlider::OnMouseDrag(float, float, float _dX, float _dY, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    const IParam* pParam = GetParam();
    float gearing = isFineQontrol(_mod, false) ? qliderGearing * 10 : qliderGearing;

    mouseDragValue += static_cast<double>(_dX / mTargetRECT.W() / gearing);
    mouseDragValue = Clip(mouseDragValue, 0.0, 1.0);

    double v = mouseDragValue;

    if (pParam->GetStepped())
      v = pParam->ConstrainNormalized(mouseDragValue);

    SetValue(v);
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void Qlider::OnMouseUp(float, float, const IMouseMod&)
{
  GetUI()->HideMouseCursor(false);
}

//*****************************************************************************************************************************************
void Qlider::OnMouseWheel(float, float, const IMouseMod& _mod, float _d)
{
  double newValue = 0;
  double oldValue = GetValue();
  const IParam* pParam = GetParam();
  float gearing = isFineQontrol(_mod, true) ? 0.001f : 0.01f;

  if (pParam->GetStepped())
  {
    if (_d != 0)
    {
      double step = pParam->GetStep();
      double v = pParam->FromNormalized(oldValue);
      v += _d > 0 ? step : -step;
      newValue = pParam->ToNormalized(v);
    }
    else
      newValue = oldValue;
  }
  else
    newValue = oldValue + static_cast<double>(gearing * _d);

  SetValue(newValue);
  SetDirty();
}

//*****************************************************************************************************************************************
void Qlider::drawQlider(IGraphics& _qui, const IColor& _color)
{
  float v = static_cast<float>(GetValue());
  IRECT filledTraq = mTargetRECT.FracRect(EDirection::Horizontal, v < 0.0171f ? 0.0171f : v);   // ...the Qliders should look "pretty"

  _qui.DrawRoundRect(qheme.grayGround.WithOpacity(0.63f * smoothedValue), mTargetRECT, 0.73f, &mBlend, 2 * strokeWidth / 3);
  _qui.DrawRoundRect(_color, filledTraq, 0.73f, &mBlend, strokeWidth);
}

//*****************************************************************************************************************************************
bool Qlider::isFineQontrol(const IMouseMod& _mod, bool _wheel) const
{
#ifdef PROTOOLS
  #ifdef OS_WIN
  return _mod.C;
  #else
  return _wheel ? _mod.C : _mod.R;
  #endif
#else
  return (_mod.C || _mod.S);
#endif
}