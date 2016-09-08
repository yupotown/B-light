#pragma once

#include <Siv3D.hpp>
#include <map>
#include <queue>
#include "BGMInfo.h"

class BGMPlayer {
public:

	BGMPlayer();

	/// <summary>
	/// 毎フレームの処理を行う。
	/// </summary>
	void update();

	/// <summary>
	/// BGM 情報を追加する。
	/// </summary>
	void load(const String &name);

	/// <summary>
	/// 遷移先も含めて BGM 情報を追加する。
	/// </summary>
	void loadRecursive(const String &name);

	/// <summary>
	/// 再生キューに BGM を追加する。
	/// </summary>
	void play(const String &name);

private:

	// BGM 情報
	std::map<String, BGMInfo> infos;

	// 再生キュー
	std::queue<String> playQueue;

	// 遷移途中 BGM の再生中
	bool trans;

	s3d::int64 fixOff, currentPos, prevPos, off;
	SecondsF fade;
	BGMInfo current, next;
	Sound middle;
	BGMInfo::Trans transInfo;
	BGMInfo::Trans::Point transPoint;

	void prepareTrans();
	std::vector<BGMInfo::Trans::Point>::iterator upperBound(s3d::int64 pos);

};
