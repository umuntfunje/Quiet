#include "Qutton.h"

//*****************************************************************************************************************************************
QuttonA::QuttonA(const IRECT& _bounds, const Qheme& _qheme, const char* _display, IActionFunction _actionFunq)
  : Qontrol(_bounds, _qheme, kNoParameter, _actionFunq)
  , display(_display)
{
  mDblAsSingleClick = true;
  setupAnimation(0.251f, 0.73f);
}

//*****************************************************************************************************************************************
void QuttonA::Draw(IGraphics& _qui)
{
  const IText qText(20, EAlign::Center, qheme.foreGround.WithOpacity(smoothedValue));

  _qui.FillRoundRect(qheme.foreGround.WithOpacity(map(smoothedValue, 0.73f, 1.0f, 0.023f, 0.123f)), mRECT, 3); // hmmmm...
  _qui.DrawText(qText, display, mRECT.GetPadded(0, 1, 0, -1));
  animateQontrol();
}

//*****************************************************************************************************************************************
void QuttonA::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    qAnimation = QAnimation::qDim;
    SetDirty();
  }
}


//*****************************************************************************************************************************************
QuttonB::QuttonB(const IRECT& _bounds, const Qheme& _qheme, const DrawQutton& _quttonFunq, IActionFunction _actionFunq)
  : Qontrol(_bounds, _qheme, kNoParameter, _actionFunq)
  , quttonFunq(_quttonFunq)
{
  mDblAsSingleClick = true;
  setupAnimation(0.251f, 0.88f);
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
    qAnimation = QAnimation::qDim;
    SetDirty();
  }
}