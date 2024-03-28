#include "Qoggle.h"

//*****************************************************************************************************************************************
Qoggle::Qoggle(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const char* _on, const char* _off, bool _asPopupQenu, char _side)
  : IControl(_bounds, _paramIdx)
  , on(_on)
  , off(_off)
  , side(_side)
  , qheme(_qheme)
{
  mDblAsSingleClick = true;

  if (_asPopupQenu)
    mDisablePrompt = false;
}

Qoggle::Qoggle(const IRECT& _bounds, const Qheme& _qheme, bool _defaultValue, const char* _on, const char* _off, IActionFunction _actionFunq)
  : IControl(_bounds, _actionFunq)
  , on(_on)
  , off(_off)
  , side('\0')
  , qheme(_qheme)
{
  mDblAsSingleClick = true;
  SetValue(_defaultValue);
}

//*****************************************************************************************************************************************
void Qoggle::Draw(IGraphics& _qui)
{
  float smoothGlow = glowSmoother.smoothe(glow);
  const IText qText(16.f, EAlign::Center, qheme.foreGround);

  // Laziness...
  if (mDisablePrompt) // Not Menu
  {
    if (isOn())
    {
      _qui.FillRoundRect(qheme.foreGround.WithOpacity(smoothGlow * .73f), mRECT.GetPadded(-2), 1.7f, &mBlend);
      _qui.DrawText(qText.WithFGColor(qheme.backGround.WithContrast(-.12f)), on, mRECT.GetPadded(-1), &mBlend);
    }
    else
    {
      _qui.FillRoundRect(qheme.qrey.WithOpacity(smoothGlow * .123f), mRECT.GetPadded(-1), 1.7f, &mBlend);
      _qui.DrawText(qText.WithFGColor(qText.mFGColor.WithOpacity(smoothGlow)), off, mRECT.GetPadded(-1), &mBlend);
    }
  }
  else // Menu
  {
    if (isOn(true))
    {
      _qui.FillRoundRect(qheme.foreGround.WithOpacity(smoothGlow * .73f), mRECT.GetPadded(-2), 1.7f, &mBlend);
      _qui.DrawText(qText.WithFGColor(qheme.backGround.WithContrast(-.12f)), on, mRECT.GetPadded(-1), &mBlend);
    }
    else
    {
      _qui.FillRoundRect(qheme.qrey.WithOpacity(smoothGlow * .123f), mRECT.GetPadded(-1), 1.7f, &mBlend);
      _qui.DrawText(qText.WithFGColor(qText.mFGColor.WithOpacity(smoothGlow)), off, mRECT.GetPadded(-1), &mBlend);
    }

    if (menuOn)
    {
      if (side == 'L')
        _qui.DrawLine(qheme.foreGround.WithOpacity(.73f), mRECT.L, mRECT.T, mRECT.L, mRECT.B, &mBlend);
      else
        _qui.DrawLine(qheme.foreGround.WithOpacity(.73f), mRECT.R, mRECT.T, mRECT.R, mRECT.B, &mBlend);
    }
  }

  if (isRising && smoothGlow < 1.f)
    SetDirty(false);
  else if (!isRising && smoothGlow > .73f)
    SetDirty(false);
}

//*****************************************************************************************************************************************
void Qoggle::OnPopupMenuSelection(IPopupMenu* pSelectedMenu, int valIdx)
{
  menuOn = false;

  if (pSelectedMenu && valIdx > kNoValIdx && GetParamIdx(valIdx) > kNoParameter && !mDisablePrompt)
    SetValueFromUserInput(GetParam()->ToNormalized(static_cast<double>(pSelectedMenu->GetChosenItemIdx())), valIdx);
}

//*****************************************************************************************************************************************
void Qoggle::OnMouseOver(float, float, const IMouseMod&)
{
  if (!isRising)
  {
    isRising = true;
    glow = 1.f;
    SetDirty(false);
  }
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

      if (side == 'L')
        PromptUserInput(mRECT.GetPadded(56, 60, -56, -60));
      else
        PromptUserInput(mRECT.GetPadded(-44, 60, 44, -60));
    }
  }
}

//*****************************************************************************************************************************************
void Qoggle::OnMouseOut()
{
  if (isRising)
  {
    isRising = false;
    glow = .73f;
    SetDirty(false);
  }
}

//*****************************************************************************************************************************************
bool Qoggle::isOn(bool _isMenu) const
{
  if (_isMenu)
    return static_cast<int>(5 * GetValue()) > 0;
  else
    return GetValue() >= .5;
}