#include "BGMPlayer.h"

#include <algorithm>

BGMPlayer::BGMPlayer()
	: trans(false),
	fixOff(735 * 6),
	off(0),
	fade(0.017s)
{
}

void BGMPlayer::update()
{
	currentPos = current.sound.samplesPlayed() - off;

	if (currentPos > current.sound.loopEndSample()) {
		const int64 loopLen = current.sound.loopEndSample() - current.sound.loopBeginSample();
		off += loopLen;
		currentPos -= loopLen;
	}

	if (!playQueue.empty()) {

		if (!trans) {
			auto posIt = upperBound(currentPos + fixOff);
			auto prevIt = upperBound(prevPos + fixOff);
			if (posIt != prevIt) {
				transPoint = (*prevIt);
				trans = true;
				middle.setPosSample(transPoint.a + (currentPos - transPoint.from));
				middle.play(fade);
				current.sound.stop(fade);
			}
		}
		else {
			const int64 transCount = middle.samplesPlayed();
			if (transCount > transPoint.b - transPoint.a) {
				trans = false;
				current = next;
				current.sound.setPosSample(transPoint.to + (transCount - (transPoint.b - transPoint.a)) - fixOff);
				current.sound.play(fade);
				middle.stop(fade);

				playQueue.pop();
				if (!playQueue.empty()) prepareTrans();
			}
		}

	}

	prevPos = currentPos;
}

void BGMPlayer::load(const String & name)
{
	BGMInfo info(name);
	infos[name] = info;
}

void BGMPlayer::loadRecursive(const String & name)
{
	std::queue<String> q;
	q.push(name);
	while (!q.empty()) {
		String t = q.front(); q.pop();
		if (infos.count(t) != 0) continue;
		infos[t] = BGMInfo(t);
		for (auto trance : infos[t].transes) q.push(trance.to);
	}
}

void BGMPlayer::play(const String & name)
{
	// 何も再生していないならすぐに再生を始める
	if (!current.sound.isPlaying() && !middle.isPlaying()) {
		current = BGMInfo(name);
		current.sound.play();
		return;
	}

	// 再生中なら再生キューに追加
	playQueue.push(name);

	// 元々のキューが空の状態だったら遷移の準備をする
	if (playQueue.size() == 1) prepareTrans();
}

void BGMPlayer::prepareTrans() {
	String nextName = playQueue.front();

	if (nextName == current.name) {
		playQueue.pop();
		return;
	}

	for (std::size_t i = 0; i < current.transes.size(); ++i) {
		if (current.transes[i].to == nextName) {
			transInfo = current.transes[i];
			middle = Sound(L"data/bgm/" + transInfo.by);
			next = infos[transInfo.to];
			break;
		}
	}
}

std::vector<BGMInfo::Trans::Point>::iterator BGMPlayer::upperBound(int64 pos)
{
	return std::upper_bound(
		transInfo.points.begin(), transInfo.points.end(),
		BGMInfo::Trans::Point{ pos, 0, 0, 0 },
		[&](const BGMInfo::Trans::Point &a, const BGMInfo::Trans::Point &b) {
			return a.from < b.from;
		}
	);
}
