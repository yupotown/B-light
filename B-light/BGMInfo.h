#pragma once

#include <Siv3D.hpp>
#include <vector>

struct BGMInfo {

	BGMInfo();

	BGMInfo(const String &name);

	/// <summary>
	/// ���O(ID)
	/// </summary>
	String name;

	/// <summary>
	/// �Ȗ�
	/// </summary>
	String title;

	/// <summary>
	/// �����f�[�^
	/// </summary>
	Sound sound;

	/// <summary>
	/// ���[�v�͈�
	/// </summary>
	struct Range {
		s3d::int64 begin, end;
	} loop;

	/// <summary>
	/// BGM �J�ڂ̂��߂̏��
	/// </summary>
	struct Trans {

		/// <summary>
		/// �J�ڐ�� BGM �̖��O
		/// </summary>
		String to;

		/// <summary>
		/// �J�ړr���� BGM �̃t�@�C���p�X
		/// </summary>
		String by;

		/// <summary>
		/// BGM ���q���ʒu
		/// </summary>
		struct Point {

			/// <summary>
			/// �J�ڑO�� BGM �̏I���ʒu
			/// </summary>
			s3d::int64 from;

			/// <summary>
			/// �J�ڌ�� BGM �̊J�n�ʒu
			/// </summary>
			s3d::int64 to;

			/// <summary>
			/// �J�ړr���� BGM �̊J�n�ʒu
			/// </summary>
			s3d::int64 a;

			/// <summary>
			/// �J�ړr���� BGM �̏I���ʒu
			/// </summary>
			s3d::int64 b;

		};

		/// <summary>
		/// BGM ���q���ʒu
		/// </summary>
		std::vector<Point> points;

	};

	/// <summary>
	/// BGM �J�ڂ̂��߂̏��
	/// </summary>
	std::vector<Trans> transes;

	/// <summary>
	/// BGM �̖��O����Ή�����t�@�C���p�X�����߂�B
	/// </summary>
	static String toPath(const String &name);

	static Range loadRange(const XMLElement &elem);
	static Trans loadTrans(const XMLElement &elem);
	static Trans::Point loadTransPoint(const XMLElement &elem);

};
