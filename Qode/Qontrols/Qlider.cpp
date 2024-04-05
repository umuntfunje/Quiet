#include "Qlider.h"

//*****************************************************************************************************************************************
Qlider::Qlider(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq)
  : Qontrol(_bounds, _qheme, _paramIdx, _actionFunq)
{
  setupAnimation(.252f, .67f);
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
  float x = (.06f * mRECT.W()) + mRECT.L, y = (.17f * mRECT.H()) + mRECT.T;
  float w = (.94f * mRECT.W()) - (.06f * mRECT.W()), h = (.83f * mRECT.H()) - (.17f * mRECT.H());

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
    double gearing = isFineQontrol(_mod, false) ? qliderGearing * 10. : qliderGearing;

    mouseDragValue += static_cast<double>(_dX / static_cast<double>(mTargetRECT.W()) / gearing);
    mouseDragValue = Clip(mouseDragValue, .0, 1.);

    double v = mouseDragValue;

    if (pParam && pParam->GetStepped() && pParam->GetStep() > 0)
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
  const double gearing = isFineQontrol(_mod, true) ? .001 : .01;
  double newValue = .0;
  const double oldValue = GetValue();
  const IParam* pParam = GetParam();

  if (pParam && pParam->GetStepped() && pParam->GetStep() > 0)
  {
    if (_d != .0f)
    {
      const double step = pParam->GetStep();
      double v = pParam->FromNormalized(oldValue);
      v += _d > 0 ? step : -step;
      newValue = pParam->ToNormalized(v);
    }
    else
      newValue = oldValue;
  }
  else
    newValue = oldValue + gearing * _d;

  SetValue(newValue);
  SetDirty();
}

//*****************************************************************************************************************************************
void Qlider::drawQlider(IGraphics& _qui, const IColor& _color)
{
  float v = static_cast<float>(GetValue());
  IRECT filledTraq = mTargetRECT.FracRect(EDirection::Horizontal, v < .018f ? .018f : v);   // ...the Qliders dont look pretty...

  _qui.DrawRoundRect(_color, filledTraq, .73f, &mBlend, thiQness);
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