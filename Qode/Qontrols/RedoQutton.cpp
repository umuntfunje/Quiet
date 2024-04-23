#include "RedoQutton.h"

//*****************************************************************************************************************************************
RedoQutton::RedoQutton(const IRECT& _bounds, const Qheme& _qheme, IActionFunction _actionFunq)
  : Qontrol(_bounds, _qheme, kNoParameter, _actionFunq)
{
  mDblAsSingleClick = true;
  setupAnimation(0.251f, 0.023f, 0.123f);
}

//*****************************************************************************************************************************************
void RedoQutton::Draw(IGraphics& _qui)
{
  _qui.FillRoundRect(qheme.foreGround.WithOpacity(smoothedValue), mRECT, 3, &mBlend);
  drawRedo(_qui);
  animateQontrol();
}

//*****************************************************************************************************************************************
void RedoQutton::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    qAnimation = QAnimation::qDim;
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void RedoQutton::drawRedo(IGraphics& _qui)
{
  float arrowSize = mRECT.GetLengthOfShortestSide() / 4;
  float newOpacity = map(smoothedValue, 0.023f, 0.123f, 0.73f, 1.0f);
  const IRECT r = mRECT.GetPadded(0, arrowSize * 1.5f, -arrowSize, -arrowSize * 1.5f);

  _qui.FillTriangle(qheme.foreGround.WithOpacity(newOpacity), r.R, r.B, r.R, r.B - arrowSize, r.R - arrowSize, r.B, &mBlend);

  _qui.PathMoveTo(r.R - arrowSize / 2, r.B - arrowSize / 2);
  _qui.PathQuadraticBezierTo(r.R - arrowSize * 2, r.B - arrowSize * 2, r.L + arrowSize, r.B);
  _qui.PathStroke(qheme.foreGround.WithOpacity(newOpacity), 1.5f, IStrokeOptions(), &mBlend);
}