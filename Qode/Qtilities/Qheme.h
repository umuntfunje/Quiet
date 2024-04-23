#pragma once

#include "IGraphicsStructs.h"

//*****************************************************************************************************************************************
struct Qheme
{
  iplug::igraphics::IColor foreGround = iplug::igraphics::IColor::FromColorCode(0x00ecec);
  iplug::igraphics::IColor backGround = iplug::igraphics::IColor::FromColorCode(0x303030);
  const iplug::igraphics::IColor grayGround = iplug::igraphics::IColor::FromColorCode(0x656565);

  enum class QhemeType
  {
    red = 0,
    blue,
    light,
    green,
    yellow
  } type = QhemeType::blue;

  void updateQheme(QhemeType _qhemeType)
  {
    switch (_qhemeType)
    {
    case QhemeType::red:
      type = QhemeType::red;
      foreGround = iplug::igraphics::IColor::FromColorCode(0xbb0808);
      backGround = iplug::igraphics::IColor::FromColorCode(0xadb3bb);
      break;

    case QhemeType::blue:
      type = QhemeType::blue;
      foreGround = iplug::igraphics::IColor::FromColorCode(0x00ecec);
      backGround = iplug::igraphics::IColor::FromColorCode(0x303030);
      break;

    case QhemeType::light:
      type = QhemeType::light;
      foreGround = iplug::igraphics::IColor::FromColorCode(0x212121);
      backGround = iplug::igraphics::IColor::FromColorCode(0xadb3bb);
      break;

    case QhemeType::green:
      type = QhemeType::green;
      foreGround = iplug::igraphics::IColor::FromColorCode(0x00ff51);
      backGround = iplug::igraphics::IColor::FromColorCode(0x303030);
      break;

    case QhemeType::yellow:
      type = QhemeType::yellow;
      foreGround = iplug::igraphics::IColor::FromColorCode(0xfbfb00);
      backGround = iplug::igraphics::IColor::FromColorCode(0x303030);
      break;
    }
  }
};