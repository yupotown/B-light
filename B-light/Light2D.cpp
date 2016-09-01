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
	// ���Z�b�g
	matLt[col] = cv::Scalar(0);

	// ��������
	for (std::size_t i = 0; i < srcs.size(); ++i) {
		const LightSource &src = srcs[i];

		// OFF ��Ԃ̌�����F�̈Ⴄ�����͏������Ȃ�
		if (!src.on || src.col != col) continue;

		// �������v�Z�p��Ԃ̂��̂ɕϊ�
		const int life = (src.life - 1) / rate + 1;

		// �͈͂�x���@�̕\���ɕϊ�����2�Ŋ���
		const int rng = int(src.range / PiF * 180 + 1e-6) / 2;

		// �����W�����v�Z�p��Ԃ̂��̂ɕϊ�
		const float att = Pow(src.att, rate);

		// 1�x����
		for (int j = -rng; j <= rng; ++j) {

			// ����
			float angle = src.angle + PiF / 180 * j;
			float dx = Cos(angle), dy = Sin(angle);

			// �ʒu���v�Z�p��Ԃ̂��̂ɕϊ�
			float x = src.pos.x / rate, y = src.pos.y / rate;

			// ����
			float st = src.st;

			// ���O�̈ʒu�̐������W
			int pnx = int(x) - 1, pny = int(y) - 1;

			// �����s����܂�
			for (int k = 0; k < life && st > 1e-2; ++k) {

				// �������W
				const int nx = int(x), ny = int(y);

				// ��ʊO�ɏo����I��
				if (nx < 0 || ny < 0 || nx >= imgw || ny >= imgh) break;

				// �ʒu���X�V
				x += dx; y += dy;

				// �������W���ς���Ă��Ȃ���Ή������Ȃ�
				if (nx == pnx && ny == pny) continue;

				// �v�Z�p�摜���X�V
				int temp = matLt[col].data[ny * imgw + nx] + st * 255;
				if (temp > 255) temp = 255;
				matLt[col].data[ny * imgw + nx] = temp;

				// �������X�V
				st *= att * air.data[ny * imgw + nx] / 255;

				// ��
				if (maskMir.data[ny * imgw + nx] != 0) {

					// �������X�V
					st *= maskMir.data[ny * imgw + nx] / 255.0f;

					// �@��
					const float * const vd = reinterpret_cast<const float *>(vecMir.data);
					const float vx = vd[(ny * imgw + nx) * 2], vy = vd[(ny * imgw + nx) * 2 + 1];

					// ���̌����Ă����������������Ă����甽��
					const float c = dx * vx + dy * vy, s = dx * vy - dy * vx;
					if (c < 0) {
						const float temp = (s * s - c * c) * dx + 2 * s * c * dy;
						dy = -2 * s * c * dx + (s * s - c * c) * dy;
						dx = temp;
					}
				}

				// ���O�̈ʒu���X�V
				pnx = nx; pny = ny;

			}

		}
	}

	// �ڂ���
	cv::GaussianBlur(matLt[col], matLt[col], cv::Size(60 / rate / 2 * 2 + 1, 60 / rate / 2 * 2 + 1), 0);

	// �e�N�X�`���ɕϊ����邽�߂� Siv3D �摜��
	const uint8 *it1 = matLt[col].data;
	const uint8 * const end = matLt[col].data + imgw * imgh;
	Color *it2 = img[col][0];
	if (col == 0) for (; it1 != end; ++it1, ++it2) it2->r = *it1;
	if (col == 1) for (; it1 != end; ++it1, ++it2) it2->g = *it1;
	if (col == 2) for (; it1 != end; ++it1, ++it2) it2->b = *it1;

	// �e�N�X�`���ɕϊ�
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
