#include "Qnob.h"

//*****************************************************************************************************************************************
Qnob::Qnob(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq)
  : Qontrol(_bounds, _qheme, _paramIdx, _actionFunq)
{
  SetValue(mouseDragValue);
  setupAnimation(0.251f, 0.63f);
}

//*****************************************************************************************************************************************
void Qnob::Draw(IGraphics& _qui)
{
  drawQnob(_qui, qheme.foreGround.WithOpacity(smoothedValue));
  animateQontrol();
}

//*****************************************************************************************************************************************
void Qnob::OnResize()
{
  SetTargetRECT(mRECT.GetScaledAboutCentre(0.88f));
  SetDirty(false);
}

//*****************************************************************************************************************************************
void Qnob::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    mouseDragValue = GetValue();
    GetUI()->HideMouseCursor(true, true);
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void Qnob::OnMouseDrag(float, float, float, float _dY, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    float gearing = isFineQontrol(_mod, false) ? qnobGearing * 10 : qnobGearing;

    mouseDragValue += static_cast<double>(_dY / -mTargetRECT.H() / gearing);
    mouseDragValue = Clip(mouseDragValue, 0.0, 1.0);

    SetValue(mouseDragValue);
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void Qnob::OnMouseUp(float, float, const IMouseMod&)
{
  GetUI()->HideMouseCursor(false);
}

//*****************************************************************************************************************************************
void Qnob::OnMouseWheel(float, float, const IMouseMod& _mod, float _d)
{
  float gearing = isFineQontrol(_mod, true) ? 0.001f : 0.01f;

  SetValue(GetValue() + static_cast<double>(gearing * _d));
  SetDirty();
}

//*****************************************************************************************************************************************
void Qnob::drawQnob(IGraphics& _qui, const IColor& _color)
{

  const float minAngle = -135, maxAngle = 135;
  float cx = mTargetRECT.MW(), cy = mTargetRECT.MH();
  float qnobRadius = mTargetRECT.H() / 2 - strokeWidth / 2;
  float angle = minAngle + (static_cast<float>(GetValue()) * (maxAngle - minAngle));

  _qui.DrawArc(qheme.grayGround.WithOpacity(0.63f * smoothedValue), cx, cy, qnobRadius, minAngle, maxAngle, &mBlend, 2 * strokeWidth / 3);
  _qui.DrawArc(_color, cx, cy, qnobRadius, angle >= minAngle ? minAngle : minAngle - (minAngle - angle),
                                           angle >= minAngle ? angle : minAngle, &mBlend, strokeWidth);
  _qui.DrawRadialLine(_color, cx, cy, angle, 0, qnobRadius + strokeWidth / 3, &mBlend, strokeWidth);
  _qui.FillCircle(_color.WithOpacity(0.23f), cx, cy, strokeWidth / 2, &mBlend);
}

//*****************************************************************************************************************************************
bool Qnob::isFineQontrol(const IMouseMod& _mod, bool _wheel) const
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