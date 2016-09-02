
#include "Light2D.h"
#include <Siv3D.hpp>
#include "LightSource.h"

struct Wall {
	Line line;
	bool floor;
};

void Main()
{
	Graphics::SetBackground(Color(0, 0, 0));

	// 光
	Light2D light(640, 480, 4);

	// 光源
	light.srcs = std::vector<LightSource>{
		LightSource(B, { 0, 0 }, 0, PiF / 60, 1.0f, 0.999f, 1000), // 操作する光源
		LightSource(R, { 320, 10 }, PiF / 2, PiF / 3, 0.8f, 0.998f, 600),
		LightSource(G, { 120, 10 }, PiF / 4, PiF / 3, 0.8f, 0.998f, 600),
		LightSource(B, { 520, 10 }, PiF * 3 / 4, PiF / 3, 0.8f, 0.998f, 600),
		LightSource(R, { 320, 240 }, 0.0f, PiF / 12, 1.0f, 0.999f, 600),
	};
	LightSource &flashlight = light.srcs[0];

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

	// 鏡
	for (auto it = walls.begin(); it != walls.end(); ++it) {
		int x1, y1, x2, y2;
		light.toImgPos(int(it->line.begin.x), int(it->line.begin.y), x1, y1);
		light.toImgPos(int(it->line.end.x), int(it->line.end.y), x2, y2);
		cv::Point p1(x1, y1), p2(x2, y2);
		const float vx = float(y2 - y1), vy = float(x1 - x2);
		const float n = Sqrt(vx * vx + vy * vy);
		cv::line(light.maskMir, p1, p2, cv::Scalar(255), 2, 8);
		cv::line(light.vecMir, p1, p2, cv::Scalar(vx / n, vy / n), 2, 8);
	}

	// 主人公の位置と大きさ
	Circle me(330, 400, 9);

	// 速度、加速度、最高速度
	Vec2 v(0, 0);
	double minvx = -3, maxvx = 3, keyacc = 0.15, jumpa0 = 3;

	// 着地しているか、横に移動しているか
	bool onFloor = false, walking = false;
	std::list<Wall>::iterator floorOn;

	// 背景画像
	Image imgBk(L"data/img/empty.png");
	Texture texBk(imgBk);

	while (System::Update())
	{
		// マウス
		const Point mouse = Mouse::Pos();

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

		// フラッシュライト
		flashlight.pos = me.center.asPoint();
		flashlight.angle = Atan2(mouse.y - float(me.center.y), mouse.x - float(me.center.x));
		flashlight.st = Random(0.85f, 0.99f);

		// 光
		light.update(R);
		light.update(G);
		light.update(B);
		light.srcs[4].angle += PiF / 180;

		// 背景の描画
		texBk.draw();

		// 光の描画
		Graphics2D::SetBlendState(BlendState::Additive);
		for (int i = 0; i < 3; ++i) {
			light.tex[i].resize(640, 480).draw();
		}
		Graphics2D::SetBlendState(BlendState::Default);

		// 床や壁の描画
		for (auto flr : walls) {
			flr.line.draw(Color(255, 255, 255, int(128 * Abs(Cos(light.srcs[4].angle)))));
		}

		// 主人公の描画
		me.draw(Palette::Red);
	}
}
