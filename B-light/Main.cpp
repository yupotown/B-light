﻿
# include <Siv3D.hpp>

struct Wall {
	Line line;
	bool floor;
};

void Main()
{
	Graphics::SetBackground(Color(0, 0, 0));

	// 重力加速度
	Vec2 gravity(0, 0.1);

	// 摩擦、床で立ち止まっている時の摩擦
	double fric = 0.98, fricFloor = 0.8;

	// 床や壁
	std::list<Wall> walls{
		{ Line(0, 460, 640, 460), true },
		{ Line(320, 460, 400, 430), true },
		{ Line(450, 430, 400, 430), false },
		{ Line(450, 460, 450, 430), false },
		{ Line(100, 360, 200, 460), true },
		{ Line(50, 360, 100, 360), true },
		{ Line(0, 200, 50, 360), false },
		{ Line(600, 460, 640, 320), false },
	};

	// 主人公の位置と大きさ
	Circle me(330, 400, 9);

	// 速度、加速度、最高速度
	Vec2 v(0, 0);
	double minvx = -3, maxvx = 3, keyacc = 0.15, jumpa0 = 3;

	// 着地しているか、横に移動しているか
	bool onFloor = false, walking = false;
	std::list<Wall>::iterator floorOn;

	while (System::Update())
	{
		// 乗っている床の向き
		const Vec2 fov = onFloor ? floorOn->line.vector().normalized() : Vec2(1, 0);

		// 左右移動
		walking = false;
		if (v.x >= minvx + keyacc && Input::KeyA.pressed) {
			v -= keyacc * fov;
			walking = true;
		}
		if (v.x <= maxvx - keyacc && Input::KeyD.pressed) {
			v += keyacc * fov;
			walking = true;
		}

		// ジャンプ
		if (onFloor && Input::KeyW.clicked) {
			v += jumpa0 * Vec2(fov.y, -fov.x);
		}

		// 重力加速度
		v += gravity;

		// 移動
		me.center += v;

		// 床や壁との当たり判定
		onFloor = false;
		for (auto it = walls.begin(); it != walls.end(); ++it) {
			const Line &line = it->line;
			const Vec2 fv = line.vector(), fmv = me.center - line.begin;
			if (fv.cross(v) > 0 && fv.cross(fmv) < 0 && line.intersects(me)) {
				if (it->floor) {
					onFloor = true;
					floorOn = it;
				}
				const Vec2 temp1 = line.begin + (fmv.dot(fv) / fv.lengthSq()) * fv;
				const Vec2 temp2 = me.center - temp1;
				me.center = temp1 + (me.r / temp2.length()) * temp2;
				v = v.dot(fv) / fv.lengthSq() * fv * (it->floor ? (!walking ? fricFloor : fric) : 1);
			}
		}

		// 床や壁の描画
		for (auto flr : walls) {
			flr.line.draw(Palette::White);
		}

		// 主人公の描画
		me.draw(Palette::Red);
	}
}
