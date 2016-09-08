#include "BGMInfo.h"

#include <algorithm>

BGMInfo::BGMInfo()
{
}

BGMInfo::BGMInfo(const String & name)
{
	const String path = toPath(name);
	XMLReader reader(path);

	this->name = name;

	for (XMLElement e = reader.root().firstChild(); e; e = e.nextSibling()) {
		if (e.name() == L"file") sound = Sound(FileSystem::ParentPath(path) + L"/" + e.text());
		else if (e.name() == L"loop") loop = loadRange(e);
		else if (e.name() == L"trans") transes.push_back(loadTrans(e));
	}

	sound.setLoopBySample(true, loop.begin, loop.end);
}

String BGMInfo::toPath(const String & name)
{
	return L"data/bgm/" + name + L".xml";
}

BGMInfo::Range BGMInfo::loadRange(const XMLElement & elem)
{
	Range res{ 0, 0 };

	for (XMLElement e = elem.firstChild(); e; e = e.nextSibling()) {
		if (e.name() == L"begin") res.begin = Parse<int64>(e.text());
		else if (e.name() == L"end") res.end = Parse<int64>(e.text());
	}

	return res;
}

BGMInfo::Trans BGMInfo::loadTrans(const XMLElement & elem)
{
	Trans res;

	for (XMLElement e = elem.firstChild(); e; e = e.nextSibling()) {
		if (e.name() == L"to") res.to = e.text();
		else if (e.name() == L"by") res.by = e.text();
		else if (e.name() == L"point") res.points.push_back(loadTransPoint(e));
	}

	std::sort(res.points.begin(), res.points.end(), [&](const Trans::Point &a, const Trans::Point &b) { return a.from < b.from; });

	return res;
}

BGMInfo::Trans::Point BGMInfo::loadTransPoint(const XMLElement &elem)
{
	Trans::Point res{ 0, 0, 0, 0 };

	for (XMLElement e = elem.firstChild(); e; e = e.nextSibling()) {
		if (e.name() == L"from") res.from = Parse<int64>(e.text());
		else if (e.name() == L"to") res.to = Parse<int64>(e.text());
		else if (e.name() == L"a") res.a = Parse<int64>(e.text());
		else if (e.name() == L"b") res.b = Parse<int64>(e.text());
	}

	return res;
}
