#pragma once

#include "Qtilities.h"

//*****************************************************************************************************************************************
struct Qheme
{
  void updateQheme(int _theme)
  {
    switch (_theme)
    {
    case 0: // default (blue)
      foreGround = IColor::FromColorCode(0x00ecec);
      backGround = IColor::FromColorCode(0x292929);
      break;
    case 1: // green
      foreGround = IColor::FromColorCode(0x00ff51);
      backGround = IColor::FromColorCode(0x292929);
      break;
    case 2: // yellow
      foreGround = IColor::FromColorCode(0xfbfb00);
      backGround = IColor::FromColorCode(0x292929);
      break;
    case 3: // light
      foreGround = IColor::FromColorCode(0x212121);
      backGround = IColor::FromColorCode(0xc2c8d0);
      break;
    case 4: // dark
      foreGround = IColor::FromColorCode(0xffffff);
      backGround = IColor::FromColorCode(0x212121);
      break;
    }
  }

  IColor foreGround = IColor::FromColorCode(0x00ecec);
  IColor backGround = IColor::FromColorCode(0x292929);
  const IColor qrey = IColor::FromColorCode(0x656565);
};