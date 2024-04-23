#include "Qrid.h"

//*****************************************************************************************************************************************
Qrid::Qrid(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx)
  : IControl(_bounds, _paramIdx)
  , qheme(_qheme)
{
  mIgnoreMouse = true;
}

//*****************************************************************************************************************************************
inline void Qrid::Draw(IGraphics& _qui)
{
  int lines = GetParam()->Int();

  if (lines == 0)
    _qui.DrawLine(qheme.grayGround.WithOpacity(0.21f), mRECT.L, mRECT.MH(), mRECT.R, mRECT.MH());
  else if (lines == 1)
    _qui.DrawLine(qheme.grayGround.WithOpacity(0.21f), mRECT.MW(), mRECT.T, mRECT.MW(), mRECT.B);
  else
    _qui.DrawGrid(qheme.grayGround.WithOpacity(0.21f), mRECT, mRECT.W() / lines, mRECT.H() / lines);
}