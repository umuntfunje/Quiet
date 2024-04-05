#include "Qnob.h"

//*****************************************************************************************************************************************
Qnob::Qnob(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, IActionFunction _actionFunq)
  : Qontrol(_bounds, _qheme, _paramIdx, _actionFunq)
{
  SetValue(mouseDragValue);
  setupAnimation(.251f, .63f);
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
  SetTargetRECT(mRECT.GetScaledAboutCentre(.88f));
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
    double gearing = isFineQontrol(_mod, false) ? qnobGearing * 10. : qnobGearing;

    mouseDragValue += static_cast<double>(_dY / static_cast<double>(-mTargetRECT.H()) / gearing);
    mouseDragValue = Clip(mouseDragValue, .0, 1.);

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
  const double gearing = isFineQontrol(_mod, true) ? .001 : .01;

  SetValue(GetValue() + gearing * _d);
  SetDirty();
}

//*****************************************************************************************************************************************
void Qnob::drawQnob(IGraphics& _qui, const IColor& _color)
{
  float qnobRadius = mTargetRECT.H() / 2.f;
  const float cx = mTargetRECT.MW(), cy = mTargetRECT.MH();

  qnobRadius -= (traqSize / 2.f);

  IRECT qnobHandleBounds = mTargetRECT.GetCentredInside((qnobRadius + .8f) * 2.f);
  const float angle = minAngle + (static_cast<float>(GetValue()) * (maxAngle - minAngle));

  _qui.DrawArc(_color, cx, cy, qnobRadius, angle >= anqorAngle ? anqorAngle : anqorAngle - (anqorAngle - angle),
                                           angle >= anqorAngle ? angle : anqorAngle, &mBlend, traqSize);
  _qui.DrawRadialLine(_color, cx, cy, -135, .0f, (qnobRadius + .4f), &mBlend, traqSize);
  _qui.DrawRadialLine(_color, cx, cy, angle, .0f, (qnobHandleBounds.W() / 2) * .99f, &mBlend, traqSize);
  _qui.FillCircle(_color.WithOpacity(.23f), cx, cy, .5f * traqSize, &mBlend);
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