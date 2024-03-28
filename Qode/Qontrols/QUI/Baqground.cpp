#include "Baqground.h"

//*****************************************************************************************************************************************
Baqground::Baqground(const IRECT& _bounds, const Qheme& _qheme)
  : IControl(_bounds)
  , qheme(_qheme)
{
  mIgnoreMouse = true;
}

//*****************************************************************************************************************************************
void Baqground::Draw(IGraphics& _qui)
{
  _qui.FillRect(qheme.backGround, mRECT);
}