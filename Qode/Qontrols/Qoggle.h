#pragma once

#include "Qontrol.h"

//*****************************************************************************************************************************************
class Qoggle : public Qontrol
{
public:
  //***************************************************************************************************************************************
  Qoggle(const IRECT& _bounds, const Qheme& _qheme, int _paramIdx, const char* _on, const char* _off, bool _asPopupQenu = false, char _side = 'L');

  void Draw(IGraphics& _qui) override;
  void OnPopupMenuSelection(IPopupMenu* _selectedQenu, int _valIdx) override;

  void OnMouseDown(float _x, float _y, const IMouseMod& _mod) override;

private:
  //***************************************************************************************************************************************
  const char side;
  bool menuOn = false;
  const char *on, *off;

  //***************************************************************************************************************************************
  bool isOn() const;
  void drawQenuQoggle(IGraphics& _qui);
  void drawRegularQoggle(IGraphics& _qui);
};