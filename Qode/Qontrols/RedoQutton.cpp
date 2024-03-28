#include "RedoQutton.h"

//*****************************************************************************************************************************************
RedoQutton::RedoQutton(const IRECT& _bounds, const Qheme& _qheme, IActionFunction _actionFunq)
  : IControl(_bounds, _actionFunq)
  , qheme(_qheme)
{
  mDblAsSingleClick = true;
}

//*****************************************************************************************************************************************
void RedoQutton::Draw(IGraphics& _qui)
{
  float smoothGlow = glowSmoother.smoothe(glow);

  _qui.FillRoundRect(qheme.foreGround.WithOpacity(smoothGlow), mRECT, 3.f, &mBlend);
  drawRedo(_qui, smoothGlow);

  if (isRising && smoothGlow < .123f)
    SetDirty(false);
  else if (!isRising && smoothGlow > .023f)
    SetDirty(false);
}

//*****************************************************************************************************************************************
void RedoQutton::OnMouseOver(float, float, const IMouseMod&)
{
  if (!isRising)
  {
    isRising = true;
    glow = .123f;
    SetDirty(false);
  }
}

//*****************************************************************************************************************************************
void RedoQutton::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    isRising = false;
    glow = .023f;
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void RedoQutton::OnMouseOut()
{
  if (isRising)
  {
    isRising = false;
    glow = .023f;
    SetDirty(false);
  }
}

//*****************************************************************************************************************************************
void RedoQutton::drawRedo(IGraphics& _qui, float _smoothGlow)
{
  float arrowSize = mRECT.GetLengthOfShortestSide() / 4;
  const IRECT r = mRECT.GetPadded(0, arrowSize * 1.5f, -arrowSize, -arrowSize * 1.5f);
  float normGlow = (_smoothGlow - .023f) / (.123f - .023f);
  float newGlow = .73f + (normGlow * (1.f - .73f));

  _qui.FillTriangle(qheme.foreGround.WithOpacity(newGlow), r.R, r.B, r.R, r.B - arrowSize, r.R - arrowSize, r.B, &mBlend);

  _qui.PathMoveTo(r.R - arrowSize / 2, r.B - arrowSize / 2);
  _qui.PathQuadraticBezierTo(r.R - arrowSize * 2, r.B - arrowSize * 2, r.L + arrowSize, r.B);
  _qui.PathStroke(qheme.foreGround.WithOpacity(newGlow), 1, IStrokeOptions(), &mBlend);
}