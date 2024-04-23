#include "Qubble.h"

//*****************************************************************************************************************************************
Qubble::Qubble(const Qheme& _qheme)
  : IBubbleControl(IText(16, EAlign::Center, _qheme.foreGround), _qheme.foreGround.WithOpacity(0), _qheme.foreGround)
  , qheme(_qheme)
{
  mIgnoreMouse = true;
}

//*****************************************************************************************************************************************
void Qubble::Draw(IGraphics& _qui)
{
  mText = IText(16, EAlign::Center, qheme.foreGround);
  mStrokeColor = mText.mFGColor;
  DrawContent(_qui, mBubbleBounds);
}

//*****************************************************************************************************************************************
void Qubble::showQubble(IControl* _qaller, const IRECT& _bounds, const char* _display)
{
  mStr.Set(_display);
  mBubbleBounds = _bounds;
  SetRECT(mRECT.Union(mBubbleBounds));

  if (mState == kCollapsed)
  {
    Hide(false);
    mState = kExpanding;
    SetDirty();
  }

  if (_qaller != mCaller)
  {
    mRECT = mBubbleBounds;
    GetUI()->SetAllControlsDirty();
  }

  mCaller = _qaller;
}