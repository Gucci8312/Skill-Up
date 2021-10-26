// 数学処理用の関数

// フォーマット
// 目的  : ------
// 入力  : ------
// 出力  : ------

#include "pch.h"
#include "framework.h"
#include "MathLibrary.h"


// 目的  : 2点から直線の角度を計算
// 入力1 : 位置情報1
// 入力2 : 位置情報2
// 出力  : 直線の角度
float GetLineAngle(float* p1, float* p2)
{
	return (p2[1] - p1[1]) / (p2[0] - p1[0]);
}


// 目的  : 入力された角度と垂直になる直線の角度を求める
// 入力  : 直線の角度
// 出力  : 入力された角度と垂直になる直線の角度
float GetReverseVerticalAngle(float Slope)
{
	return -1 / Slope;
}


// 目的  : 2つの直線が垂直かどうかを調べる
// 入力1 : 直線の傾き
// 入力2 : 直線の傾き
// 出力  : 入力された傾きの直線が垂直ならtrueを返す
bool CheckVertical(float Slope1, float Slope2)
{
	if (Slope1 * Slope2 == -1)
	{
		return true;
	}
	return false;
}

// todo
// 目的  : 入力されたスカラーから直線での移動座標を取得
// 入力1 : 座標を計算するための値
// 出力  : 2次元での位置座標
