#pragma once

// ステアリングスイッチのキー
enum STEERING_SW
{
    UNKNOWN = -1,
    NONE = 0,
    VOL_UP,
    VOL_DOWN,
    NEXT,
    PREV,
    TEST1, // for debug.
    TEST2, // for debug.
};

enum STEERING_SW read_sw();
