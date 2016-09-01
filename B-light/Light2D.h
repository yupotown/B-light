#pragma once

#include <opencv2/opencv.hpp>
#include <Siv3D.hpp>
#include <vector>
#include "LightSource.h"

struct Light2D {

	Light2D(int screenW, int screenH, int imgRate);

	/// <summary>
	/// 1色のテクスチャを更新する。
	/// </summary>
	void update(PColor col);

	/// <summary>
	/// 画面の座標を計算用空間の座標に変換する。
	/// </summary>
	int toImgX(int x) const;

	/// <summary>
	/// 画面の座標を計算用空間の座標に変換する。
	/// </summary>
	int toImgY(int y) const;

	/// <summary>
	/// 画面の座標を計算用空間の座標に変換する。
	/// </summary>
	void toImgPos(int x, int y, int &outX, int &outY) const;

	/// <summary>
	/// 計算用空間の座標から対応する画面の範囲の左上の座標を計算する。
	/// </summary>
	int toScreenX(int x) const;

	/// <summary>
	/// 計算用空間の座標から対応する画面の範囲の左上の座標を計算する。
	/// </summary>
	int toScreenY(int y) const;

	/// <summary>
	/// 計算用空間の座標から対応する画面の範囲の左上の座標を計算する。
	/// </summary>
	void toScreenPos(int x, int y, int &outX, int &outY) const;

	/// <summary>
	/// 画面の幅
	/// </summary>
	int scw;

	/// <summary>
	/// 画面の高さ
	/// </summary>
	int sch;

	/// <summary>
	/// 計算用画像の幅
	/// </summary>
	int imgw;

	/// <summary>
	/// 計算用画像の高さ
	/// </summary>
	int imgh;

	/// <summary>
	/// 画面の解像度に対する光の計算用画像の解像度の比率の逆数
	/// </summary>
	int rate;

	/// <summary>
	/// 最終的な描画用テクスチャ
	/// </summary>
	DynamicTexture tex[3];

	/// <summary>
	/// 計算用画像
	/// </summary>
	cv::Mat matLt[3];

	/// <summary>
	/// 計算用画像からテクスチャに変換するための Siv3D 画像
	/// </summary>
	Image img[3];

	/// <summary>
	/// 空間の光透過率
	/// </summary>
	cv::Mat air;

	/// <summary>
	/// 鏡
	/// </summary>
	cv::Mat maskMir;

	/// <summary>
	/// 鏡の法線ベクトル
	/// </summary>
	cv::Mat vecMir;

	/// <summary>
	/// 光源
	/// </summary>
	std::vector<LightSource> srcs;

};
