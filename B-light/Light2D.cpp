#include "Light2D.h"

Light2D::Light2D(int screenW, int screenH, int imgRate)
{
	scw = screenW;
	sch = screenH;
	rate = imgRate;
	imgw = (screenW - 1) / rate + 1;
	imgh = (screenH - 1) / rate + 1;
	for (int i = 0; i < 3; ++i) {
		new(tex + i) DynamicTexture(imgw, imgh);
		new(matLt + i) cv::Mat(imgh, imgw, CV_8UC1);
		new(img + i) Image(imgw, imgh, Color(0, 0, 0, 255));
	}
	new(&air) cv::Mat(imgh, imgw, CV_8UC1, cv::Scalar(255));
	new(&maskMir) cv::Mat(imgh, imgw, CV_8UC1, cv::Scalar(0));
	new(&vecMir) cv::Mat(imgh, imgw, CV_32FC2, cv::Scalar(0, 0));
}

void Light2D::update(PColor col)
{
	// リセット
	matLt[col] = cv::Scalar(0);

	// 光源ごと
	for (std::size_t i = 0; i < srcs.size(); ++i) {
		const LightSource &src = srcs[i];

		// OFF 状態の光源や色の違う光源は処理しない
		if (!src.on || src.col != col) continue;

		// 寿命を計算用空間のものに変換
		const int life = (src.life - 1) / rate + 1;

		// 範囲を度数法の表現に変換して2で割る
		const int rng = int(src.range / PiF * 180 + 1e-6) / 2;

		// 減衰係数を計算用空間のものに変換
		const float att = Pow(src.att, rate);

		// 1度ごと
		for (int j = -rng; j <= rng; ++j) {

			// 向き
			float angle = src.angle + PiF / 180 * j;
			float dx = Cos(angle), dy = Sin(angle);

			// 位置を計算用空間のものに変換
			float x = src.pos.x / rate, y = src.pos.y / rate;

			// 強さ
			float st = src.st;

			// 直前の位置の整数座標
			int pnx = int(x) - 1, pny = int(y) - 1;

			// 光が尽きるまで
			for (int k = 0; k < life && st > 1e-2; ++k) {

				// 整数座標
				const int nx = int(x), ny = int(y);

				// 画面外に出たら終了
				if (nx < 0 || ny < 0 || nx >= imgw || ny >= imgh) break;

				// 位置を更新
				x += dx; y += dy;

				// 整数座標が変わっていなければ何もしない
				if (nx == pnx && ny == pny) continue;

				// 計算用画像を更新
				int temp = matLt[col].data[ny * imgw + nx] + st * 255;
				if (temp > 255) temp = 255;
				matLt[col].data[ny * imgw + nx] = temp;

				// 強さを更新
				st *= att * air.data[ny * imgw + nx] / 255;

				// 鏡
				if (maskMir.data[ny * imgw + nx] != 0) {

					// 強さを更新
					st *= maskMir.data[ny * imgw + nx] / 255.0f;

					// 法線
					const float * const vd = reinterpret_cast<const float *>(vecMir.data);
					const float vx = vd[(ny * imgw + nx) * 2], vy = vd[(ny * imgw + nx) * 2 + 1];

					// 鏡の向いている方向から光が来ていたら反射
					const float c = dx * vx + dy * vy, s = dx * vy - dy * vx;
					if (c < 0) {
						const float temp = (s * s - c * c) * dx + 2 * s * c * dy;
						dy = -2 * s * c * dx + (s * s - c * c) * dy;
						dx = temp;
					}
				}

				// 直前の位置を更新
				pnx = nx; pny = ny;

			}

		}
	}

	// ぼかす
	cv::GaussianBlur(matLt[col], matLt[col], cv::Size(60 / rate / 2 * 2 + 1, 60 / rate / 2 * 2 + 1), 0);

	// テクスチャに変換するために Siv3D 画像化
	const uint8 *it1 = matLt[col].data;
	const uint8 * const end = matLt[col].data + imgw * imgh;
	Color *it2 = img[col][0];
	if (col == 0) for (; it1 != end; ++it1, ++it2) it2->r = *it1;
	if (col == 1) for (; it1 != end; ++it1, ++it2) it2->g = *it1;
	if (col == 2) for (; it1 != end; ++it1, ++it2) it2->b = *it1;

	// テクスチャに変換
	tex[col].fill(img[col]);
}

int Light2D::toImgX(int x) const
{
	return x / rate;
}

int Light2D::toImgY(int y) const
{
	return y / rate;
}

void Light2D::toImgPos(int x, int y, int & outX, int & outY) const
{
	outX = toImgX(x);
	outY = toImgY(y);
}

int Light2D::toScreenX(int x) const
{
	return x * rate;
}

int Light2D::toScreenY(int y) const
{
	return y * rate;
}

void Light2D::toScreenPos(int x, int y, int & outX, int & outY) const
{
	outX = toScreenX(x);
	outY = toScreenY(y);
}
