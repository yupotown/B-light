#pragma once

#include <Siv3D.hpp>
#include <map>
#include <queue>
#include "BGMInfo.h"

class BGMPlayer {
public:

	BGMPlayer();

	/// <summary>
	/// ���t���[���̏������s���B
	/// </summary>
	void update();

	/// <summary>
	/// BGM ����ǉ�����B
	/// </summary>
	void load(const String &name);

	/// <summary>
	/// �J�ڐ���܂߂� BGM ����ǉ�����B
	/// </summary>
	void loadRecursive(const String &name);

	/// <summary>
	/// �Đ��L���[�� BGM ��ǉ�����B
	/// </summary>
	void play(const String &name);

private:

	// BGM ���
	std::map<String, BGMInfo> infos;

	// �Đ��L���[
	std::queue<String> playQueue;

	// �J�ړr�� BGM �̍Đ���
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
