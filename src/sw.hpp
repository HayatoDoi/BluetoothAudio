/*******************************************************
 *
 * sw.hpp
 *
 * ステアリングスイッチの値を読み込むプログラム。
 *
 * Copyright (C) 1997- Hayato Doi. All rights reserved.
 *******************************************************/
#pragma once

/* ステアリングスイッチのキー定義 */
enum STEERING_SW
{
    NONE = 0,
    VOL_UP,
    VOL_DOWN,
    NEXT,
    PREV,
    TEST1, // for debug.
    TEST2, // for debug.
};

/* スイッチ機能のセットアップ */
void sw_setup();

/* ステアリングスイッチの値を読み込む*/
enum STEERING_SW sw_read();
