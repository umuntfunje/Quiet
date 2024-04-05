#include "Qabel.h"

//*****************************************************************************************************************************************
Qabel::Qabel(const IRECT& _bounds, const Qheme& _qheme, const char* _qabel)
  : IControl(_bounds)
  , qheme(_qheme)
  , qabel(_qabel)
{
  mIgnoreMouse = true;
}

//*****************************************************************************************************************************************
void Qabel::Draw(IGraphics& _qui)
{
  _qui.DrawText(IText(16.f, EAlign::Center, qheme.foreGround), qabel.Get(), mRECT, &mBlend);
}