// 数学処理用の関数

#include "pch.h"
#include "framework.h"
#include "MathLibrary.h"

// 目的  : 2点から直線の角度を計算
// 入力1 : 位置情報1
// 入力2 : 位置情報2
// 出力  : 直線の角度
float GetLineAngle(float* p1, float* p2)
{
	return (p2[1]-p1[1])/(p2[0] - p1[0]);
}

// 目的  : 入力された角度と垂直になる直線の角度を求める
// 入力  : 直線の角度
// 出力  : 入力された角度と垂直になる直線の角度