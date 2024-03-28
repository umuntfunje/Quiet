#include "UndoQutton.h"

//*****************************************************************************************************************************************
UndoQutton::UndoQutton(const IRECT& _bounds, const Qheme& _qheme, IActionFunction _actionFunq)
  : IControl(_bounds, _actionFunq)
  , qheme(_qheme)
{
  mDblAsSingleClick = true;
}

//*****************************************************************************************************************************************
void UndoQutton::Draw(IGraphics& _qui)
{
  float smoothGlow = glowSmoother.smoothe(glow);

  _qui.FillRoundRect(qheme.foreGround.WithOpacity(smoothGlow), mRECT, 3.f, &mBlend);
  drawUndo(_qui, smoothGlow);

  if (isRising && smoothGlow < .123f)
    SetDirty(false);
  else if (!isRising && smoothGlow > .023f)
    SetDirty(false);
}

//*****************************************************************************************************************************************
void UndoQutton::OnMouseOver(float, float, const IMouseMod&)
{
  if (!isRising)
  {
    isRising = true;
    glow = .123f;
    SetDirty(false);
  }
}

//*****************************************************************************************************************************************
void UndoQutton::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    isRising = false;
    glow = .023f;
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void UndoQutton::OnMouseOut()
{
  if (isRising)
  {
    isRising = false;
    glow = .023f;
    SetDirty(false);
  }
}

//*****************************************************************************************************************************************
void UndoQutton::drawUndo(IGraphics& _qui, float _smoothGlow)
{
  float arrowSize = mRECT.GetLengthOfShortestSide() / 4;
  const IRECT r = mRECT.GetPadded(-arrowSize, arrowSize * 1.5f, 0, -arrowSize * 1.5f);
  float normGlow = (_smoothGlow - .023f) / (.123f - .023f);
  float newGlow = .73f + (normGlow * (1.f - .73f));

  _qui.FillTriangle(qheme.foreGround.WithOpacity(newGlow), r.L, r.B, r.L, r.B - arrowSize, r.L + arrowSize, r.B, &mBlend);

  _qui.PathMoveTo(r.L + arrowSize / 2, r.B - arrowSize / 2);
  _qui.PathQuadraticBezierTo(r.L + arrowSize * 2, r.B - arrowSize * 2, r.R - arrowSize, r.B);
  _qui.PathStroke(qheme.foreGround.WithOpacity(newGlow), 1, IStrokeOptions(), &mBlend);
}