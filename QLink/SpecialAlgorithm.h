//
// Created by overji on 2024/6/24.
//

#pragma once

inline int specialDiv(int a, int b) {
    // 除法向下取整,例如-0.5取整为-1
    int res = a / b;
    if (a % b != 0 && ((a < 0) ^ (b < 0))) {
        res--;
    }
    return res;

}