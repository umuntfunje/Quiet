#include "Qoggle.h"

//*****************************************************************************************************************************************
Qoggle::Qoggle(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const char* _on, const char* _off, bool _asPopupQenu, char _side)
  : Qontrol(_bounds, _qheme, _paramIdx)
  , on(_on)
  , off(_off)
  , side(_side)
{
  mDblAsSingleClick = true;

  if (_asPopupQenu)
    mDisablePrompt = false;

  setupAnimation(0.251f, 0.73f);
}

//*****************************************************************************************************************************************
void Qoggle::Draw(IGraphics& _qui)
{
  if (mDisablePrompt)
    drawRegularQoggle(_qui);
  else
    drawQenuQoggle(_qui);

  animateQontrol();
}

//*****************************************************************************************************************************************
void Qoggle::OnPopupMenuSelection(IPopupMenu* _selectedQenu, int _valIdx)
{
  menuOn = false;

  if (_selectedQenu && _valIdx > kNoValIdx && GetParamIdx(_valIdx) > kNoParameter && !mDisablePrompt)
    SetValueFromUserInput(GetParam()->ToNormalized(static_cast<double>(_selectedQenu->GetChosenItemIdx())), _valIdx);
}

//*****************************************************************************************************************************************
void Qoggle::OnMouseDown(float, float, const IMouseMod& _mod)
{
  if (_mod.L)
  {
    if (mDisablePrompt)
    {
      SetValue(!GetValue());
      SetDirty();
    }
    else
    {
      menuOn = true;
      qAnimation = QAnimation::qDim;

      if (side == 'L')
        PromptUserInput(mRECT.GetPadded(56, 60, -56, -60));
      else
        PromptUserInput(mRECT.GetPadded(-44, 60, 44, -60));
    }
  }
}

//*****************************************************************************************************************************************
bool Qoggle::isOn() const
{
  return GetValue() >= 0.5;
}

//*****************************************************************************************************************************************
void Qoggle::drawQenuQoggle(IGraphics& _qui)
{
  const IText qText(16, EAlign::Center, qheme.foreGround.WithOpacity(smoothedValue));

  _qui.FillRoundRect(qheme.foreGround.WithOpacity(map(smoothedValue, 0.73f, 1.0f, 0.023f, 0.123f)), mRECT.GetPadded(-1), 1.7f, &mBlend);
  _qui.DrawText(qText, off, mRECT.GetPadded(-1), &mBlend);

  if (menuOn)
  {
    if (side == 'L')
      _qui.DrawLine(qheme.foreGround.WithOpacity(0.73f), mRECT.L, mRECT.T, mRECT.L, mRECT.B, &mBlend);
    else
      _qui.DrawLine(qheme.foreGround.WithOpacity(0.73f), mRECT.R, mRECT.T, mRECT.R, mRECT.B, &mBlend);
  }
}

//*****************************************************************************************************************************************
void Qoggle::drawRegularQoggle(IGraphics& _qui)
{
  const IText qText(16, EAlign::Center, qheme.foreGround.WithOpacity(smoothedValue));

  if (isOn())
  {
    _qui.FillRoundRect(qheme.foreGround.WithOpacity(smoothedValue * 0.73f), mRECT.GetPadded(-2), 1.7f, &mBlend);
    _qui.DrawText(qText.WithFGColor(qheme.backGround.WithContrast(-0.12f)), on, mRECT.GetPadded(-1), &mBlend);
  }
  else
  {
    _qui.FillRoundRect(qheme.foreGround.WithOpacity(map(smoothedValue, 0.73f, 1.0f, 0.023f, 0.123f)), mRECT.GetPadded(-1), 1.7f, &mBlend);
    _qui.DrawText(qText, off, mRECT.GetPadded(-1), &mBlend);
  }
}