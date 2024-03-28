#include "Qutton.h"

//*****************************************************************************************************************************************
QuttonA::QuttonA(const IRECT& _bounds, const Qheme& _qheme, const char* _ch, IActionFunction _actionFunq, float _textSize)
  : IControl(_bounds, _actionFunq)
  , ch(_ch)
  , qheme(_qheme)
  , textSize(_textSize)
{
  mDblAsSingleClick = true;
}

//*****************************************************************************************************************************************
void QuttonA::Draw(IGraphics& _qui)
{
  float smoothGlow = glowSmoother.smoothe(glow);
  float normGlow = (smoothGlow - .73f) / (1.f - .73f);
  float newGlow = .023f + (normGlow * (.123f - .023f));
  const IText qText(16.f, EAlign::Center, qheme.foreGround);

  _qui.FillRoundRect(qheme.foreGround.WithOpacity(newGlow), mRECT, 3.f); // hmmmm...
  _qui.DrawText(qText.WithSize(textSize).WithFGColor(qText.mFGColor.WithOpacity(smoothGlow)), ch, mRECT.GetPadded(0, 1, 0, -1));

  if (isRising && smoothGlow < 1.f)
    SetDirty(false);
  else if (!isRising && smoothGlow > .73f)
    SetDirty(false);
}

//*****************************************************************************************************************************************
void QuttonA::OnMouseOver(float, float, const IMouseMod&)
{
  if (!isRising)
  {
    isRising = true;
    glow = 1.f;
    SetDirty(false);
  }
}

//*****************************************************************************************************************************************
void QuttonA::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    isRising = false;
    glow = .73f;
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void QuttonA::OnMouseOut()
{
  if (isRising)
  {
    isRising = false;
    glow = .73f;
    SetDirty(false);
  }
}


//*****************************************************************************************************************************************
QuttonB::QuttonB(const IRECT& _bounds, const Qheme& _qheme, const DrawQutton& _quttonFunq, IActionFunction _actionFunq)
  : IControl(_bounds, _actionFunq)
  , qheme(_qheme)
  , quttonFunq(_quttonFunq)
{
  mDblAsSingleClick = true;
}

//*****************************************************************************************************************************************
void QuttonB::Draw(IGraphics& _qui)
{
  float smoothGrow = growSmoother.smoothe(grow);

  quttonFunq(_qui, mRECT, qheme.foreGround, smoothGrow);

  if (isRising && smoothGrow < 1.f)
    SetDirty(false);
  else if (!isRising && smoothGrow > .88f)
    SetDirty(false);
}

//*****************************************************************************************************************************************
void QuttonB::OnMouseOver(float, float, const IMouseMod&)
{
  if (!isRising)
  {
    isRising = true;
    grow = 1.f;
    SetDirty(false);
  }
}

//*****************************************************************************************************************************************
void QuttonB::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    isRising = false;
    grow = .73f;
    SetDirty();
  }
}

//*****************************************************************************************************************************************
void QuttonB::OnMouseOut()
{
  if (isRising)
  {
    isRising = false;
    grow = .88f;
    SetDirty(false);
  }
}