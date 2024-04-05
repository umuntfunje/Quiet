#include "Qutton.h"

//*****************************************************************************************************************************************
QuttonA::QuttonA(const IRECT& _bounds, const Qheme& _qheme, const char* _ch, IActionFunction _actionFunq, float _textSize)
  : Qontrol(_bounds, _qheme, kNoParameter, _actionFunq)
  , ch(_ch)
  , textSize(_textSize)
{
  mDblAsSingleClick = true;
  setupAnimation(.251f, .73f);
}

//*****************************************************************************************************************************************
void QuttonA::Draw(IGraphics& _qui)
{
  float normGlow = (smoothedValue - .73f) / (1.f - .73f);
  float newGlow = .023f + (normGlow * (.123f - .023f));
  const IText qText(16.f, EAlign::Center, qheme.foreGround);

  _qui.FillRoundRect(qheme.foreGround.WithOpacity(newGlow), mRECT, 3.f); // hmmmm...
  _qui.DrawText(qText.WithSize(textSize).WithFGColor(qText.mFGColor.WithOpacity(smoothedValue)), ch, mRECT.GetPadded(0, 1, 0, -1));
  animateQontrol();
}

//*****************************************************************************************************************************************
void QuttonA::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    qAnimation = qDim;
    SetDirty();
  }
}


//*****************************************************************************************************************************************
QuttonB::QuttonB(const IRECT& _bounds, const Qheme& _qheme, const DrawQutton& _quttonFunq, IActionFunction _actionFunq)
  : Qontrol(_bounds, _qheme, kNoParameter, _actionFunq)
  , quttonFunq(_quttonFunq)
{
  mDblAsSingleClick = true;
  setupAnimation(.251f, .88f);
}

//*****************************************************************************************************************************************
void QuttonB::Draw(IGraphics& _qui)
{
  quttonFunq(_qui, mRECT, qheme.foreGround, smoothedValue);
  animateQontrol();
}

//*****************************************************************************************************************************************
void QuttonB::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    qAnimation = qDim;
    SetDirty();
  }
}