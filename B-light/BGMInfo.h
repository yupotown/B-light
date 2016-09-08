#pragma once

#include <Siv3D.hpp>
#include <vector>

struct BGMInfo {

	BGMInfo();

	BGMInfo(const String &name);

	/// <summary>
	/// 名前(ID)
	/// </summary>
	String name;

	/// <summary>
	/// 曲名
	/// </summary>
	String title;

	/// <summary>
	/// 音声データ
	/// </summary>
	Sound sound;

	/// <summary>
	/// ループ範囲
	/// </summary>
	struct Range {
		s3d::int64 begin, end;
	} loop;

	/// <summary>
	/// BGM 遷移のための情報
	/// </summary>
	struct Trans {

		/// <summary>
		/// 遷移先の BGM の名前
		/// </summary>
		String to;

		/// <summary>
		/// 遷移途中の BGM のファイルパス
		/// </summary>
		String by;

		/// <summary>
		/// BGM を繋ぐ位置
		/// </summary>
		struct Point {

			/// <summary>
			/// 遷移前の BGM の終了位置
			/// </summary>
			s3d::int64 from;

			/// <summary>
			/// 遷移後の BGM の開始位置
			/// </summary>
			s3d::int64 to;

			/// <summary>
			/// 遷移途中の BGM の開始位置
			/// </summary>
			s3d::int64 a;

			/// <summary>
			/// 遷移途中の BGM の終了位置
			/// </summary>
			s3d::int64 b;

		};

		/// <summary>
		/// BGM を繋ぐ位置
		/// </summary>
		std::vector<Point> points;

	};

	/// <summary>
	/// BGM 遷移のための情報
	/// </summary>
	std::vector<Trans> transes;

	/// <summary>
	/// BGM の名前から対応するファイルパスを求める。
	/// </summary>
	static String toPath(const String &name);

	static Range loadRange(const XMLElement &elem);
	static Trans loadTrans(const XMLElement &elem);
	static Trans::Point loadTransPoint(const XMLElement &elem);

};
