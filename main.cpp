#include <Novice.h>
#include <cmath>

struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

// x軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {

	Matrix4x4 resultRotateX = { 0.0f };

	resultRotateX.m[0][0] = 1.0f;
	resultRotateX.m[1][1] = std::cosf(radian);
	resultRotateX.m[1][2] = std::sinf(radian);
	resultRotateX.m[2][1] = -std::sinf(radian);
	resultRotateX.m[2][2] = std::cosf(radian);
	resultRotateX.m[3][3] = 1.0f;

	return resultRotateX;
}

// y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {

	Matrix4x4 resultRotateY = { 0.0f };

	resultRotateY.m[0][0] = std::cosf(radian);
	resultRotateY.m[0][2] = -std::sinf(radian);
	resultRotateY.m[1][1] = 1.0f;
	resultRotateY.m[2][0] = std::sinf(radian);
	resultRotateY.m[2][2] = std::cosf(radian);
	resultRotateY.m[3][3] = 1.0f;

	return resultRotateY;
}

// z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {

	Matrix4x4 resultRotateZ = { 0.0f };

	resultRotateZ.m[0][0] = std::cosf(radian);
	resultRotateZ.m[0][1] = std::sinf(radian);
	resultRotateZ.m[1][0] = -std::sinf(radian);
	resultRotateZ.m[1][1] = std::cosf(radian);
	resultRotateZ.m[2][2] = 1.0f;
	resultRotateZ.m[3][3] = 1.0f;

	return resultRotateZ;
}

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 resultMultiply = {};

	for (int x = 0; x < 4; x++) {
		for (int y = 0; y < 4; y++) {
			resultMultiply.m[y][x] = m1.m[y][0] * m2.m[0][x] + m1.m[y][1] * m2.m[1][x] + m1.m[y][2] * m2.m[2][x] + m1.m[y][3] * m2.m[3][x];
		}
	}

	return resultMultiply;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

// 4x4行列の数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x, y, " %s", label);
			Novice::ScreenPrintf(x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}

const char kWindowTitle[] = "LE2B_01_アキモト_ワタル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 rotate{ 0.4f,1.43f,-0.8f };

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		/// 
		///

		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);

		Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, rotateXMatrix, "rotateXMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5, rotateYMatrix, "rotateYMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, rotateZMatrix, "rotateZMatrix");
		MatrixScreenPrintf(0, kRowHeight * 5 * 3, rotateXYZMatrix, "rotateXYZMatrix");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
