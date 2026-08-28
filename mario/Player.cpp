#include "Player.h"

Player::Player()
	: position(0, 0), velocity(0, 0),
	texture(U"example/resource/チビマリオ.png"),
	currentTexturePath(U"example/resource/チビマリオ.png") {}

Player::Player(const Vec2& pos, const Texture& tex)
	: position(pos), velocity(0, 0),
	texture(tex),
	currentTexturePath(U"example/resource/チビマリオ.png") // 初期化時にパスを保存する
{}

void Player::Goalupdate() {
	if (GameClear == false)return;
	if (goalTimer == 0) {
		goalTimer = 20;
		if (marioType == SuperMario) {
			newTexturePath = U"example/resource/ゴールスーパーマリオ.png";
		}
		else if (marioType == fireMario) {
			newTexturePath = U"example/resource/ゴールファイアーマリオ.png";
		}
		else {
			newTexturePath = U"example/resource/ゴールチビマリオ.png";
		}
	}

		goalTimer -= Scene::DeltaTime();

		if (goalTimer < 19 ) {
			velocity.y = 1;
			}


		if (goalTimer < 16) {
			if (marioType == SuperMario) {
				newTexturePath = U"example/resource/スーパーマリオ歩き右.png";
			}
			else if (marioType == fireMario) {
				newTexturePath = U"example/resource/ファイアーマリオ歩き右.png";
			}
			else {
				newTexturePath = U"example/resource/チビマリオ歩き右.png";
			}



			velocity.x = 2;

			const double gravity = 0.3;
			// 重力を適用
			if (!isOnGround)
			{
				velocity.y += gravity; // 重力加速度
			}
		}


		position += velocity;
		// テクスチャを更新
		if (newTexturePath != currentTexturePath) {
			texture = Texture(newTexturePath);
			currentTexturePath = newTexturePath;
		}
}

void Player::update()
{
	if (GameClear == true)return;

	if (hitStop)
	{
		newTexturePath = U"example/resource/キノコアニメーション右.png";
		// テクスチャを更新
		if (newTexturePath != currentTexturePath) {
			texture = Texture(newTexturePath);
			currentTexturePath = newTexturePath;
		}
		hitStopTimer -= Scene::DeltaTime();
		if (hitStopTimer <= 0)
		{
			hitStop = false; // ヒットストップ終了
		}
		return; // ヒットストップ中は他の更新をスキップ
	}

	if (marioType == SuperMario && previousMarioType != SuperMario) {
		hitStop = true;
		hitStopTimer = 0.5; // ヒットストップの持続時間を設定（例：0.2秒）
	}

	const double gravity = 0.3;
	// 重力を適用
	if (!isOnGround)
	{
		velocity.y += gravity; // 重力加速度
	}
	else
	{
		velocity.y = 0;
		newTexturePath = U"example/resource/チビマリオ.png";
		


		if (marioType == SuperMario) {
			newTexturePath = U"example/resource/スーパーマリオ.png";
			bigMario = true;
		}
		else if (marioType == fireMario) {
			newTexturePath = U"example/resource/ファイアーマリオ.png";
			bigMario = true;
		}
		else {
			marioType == smallMario;
		}
	}


	if (marioType == SuperMario) {
		marioSize = 2;
		XmarioSize = 1.3333333333333333;
		marioSizeM2 = 1;
	}
	else if (marioType == fireMario) {
		marioSize = 2;
		XmarioSize = 1.3333333333333333;
		marioSizeM2 = 1;
	}
	else {
		marioSize = 1;
		XmarioSize = 1;
		marioSizeM2 = 0;
		bigMario = false;
	}

	


	previousMarioType = marioType;



	if (KeyRight.pressed() && isMoveR) {
		velocity.x += 0.05;
		orientation = false;
		if (isOnGround == true) {
			if (marioType == SuperMario) {
				newTexturePath = U"example/resource/スーパーマリオ歩き右.png";
			}
			else if (marioType == fireMario) {
				newTexturePath = U"example/resource/ファイアーマリオ歩き右.png";
			}
			else {
				newTexturePath = U"example/resource/チビマリオ歩き右.png";
			}
		}
	}

	if (KeyLeft.pressed() && isMoveL) {
		velocity.x += -0.05;
		orientation = true;
		if (marioType == SuperMario) {
			newTexturePath = U"example/resource/スーパーマリオ歩き左.png";
		}
		else if (marioType == fireMario) {
			newTexturePath = U"example/resource/ファイアーマリオ歩き左.png";
		}
		else {
			newTexturePath = U"example/resource/チビマリオ歩き左.png";
		}
	}

	if (KeyRight.pressed() || KeyLeft.pressed()) {

	}
	else if(velocity.x > 0) {
		velocity.x -= 0.05;
	}
	else if (velocity.x < 0) {
		velocity.x += 0.05;
	}
	else {
		velocity.x = 0;
	}

	if (velocity.x > 3) {
		velocity.x = 3;
	}

	if (velocity.x < -3) {
		velocity.x = -3;
	}

	if (KeySpace.down() && isOnGround) {
		velocity.y = -13;
		isOnGround = false; // ジャンプ時には地面に接触していないと判断
	}

	if (velocity.y < -1) {
		if (marioType == SuperMario) {
			newTexturePath = U"example/resource/スーパーマリオジャンプ.png";
		}
		else if (marioType == fireMario) {
			newTexturePath = U"example/resource/ファイアーマリオジャンプ.png";
		}
		else {
			newTexturePath = U"example/resource/マリオジャンプ.png";
		}
	}


	position += velocity;

	// テクスチャを更新
	if (newTexturePath != currentTexturePath) {
		texture = Texture(newTexturePath);
		currentTexturePath = newTexturePath;
	}
}

RectF Player::getBoundingRect(const Vec2& cameraOffset) const
{
	return RectF(position.x - cameraOffset.x, position.y - (size.y * marioSizeM2), size.x * XmarioSize, size.y * marioSize); // プレイヤーの当たり判定矩形を返す
}


int32 divisions = 5;
bool jump = false;

void Player::draw(Vec2& cameraOffset)
{
	if (enable == false)return;

	int32 frameRate = 10;


	if (currentTexturePath == U"example/resource/チビマリオ歩き右.png" || currentTexturePath == U"example/resource/チビマリオ歩き左.png" ||
		currentTexturePath == U"example/resource/スーパーマリオ歩き右.png" || currentTexturePath == U"example/resource/スーパーマリオ歩き左.png" ||
		currentTexturePath == U"example/resource/ファイアーマリオ歩き右.png" || currentTexturePath == U"example/resource/ファイアーマリオ歩き左.png"){
		divisions = 5;
		jump = false;
		frameRate = 10;
	}
	else if (currentTexturePath == U"example/resource/マリオジャンプ.png" ||
		currentTexturePath == U"example/resource/スーパーマリオジャンプ.png" ||
		currentTexturePath == U"example/resource/ファイアーマリオジャンプ.png") {
		divisions = 2;
		jump = true;
		frameRate = 10;
	}
	else if (currentTexturePath == U"example/resource/ゴールチビマリオ.png" ||
		currentTexturePath == U"example/resource/ゴールスーパーマリオ.png" ||
		currentTexturePath == U"example/resource/ゴールファイアーマリオ.png")
	{
		divisions = 3;
	}
	else {
		divisions = 1;
		jump = false;
		frameRate = 10;
	}


	if (currentTexturePath == U"example/resource/キノコアニメーション右.png") {
		divisions = 3;
		frameRate = 13;
	}

	const int32 segmentWidth = texture.width() / divisions;
	const int32 segmentHeight = texture.height();

	// 現在のフレーム数から、現在表示すべきセグメントを計算
	int32 currentFrame = static_cast<int32>((Scene::Time() * frameRate)) % divisions;
	if (currentFrame >= 3) {
		currentFrame = 1;
	}

	if (isOnGround == true) {
		if (velocity.x < 0 && KeyRight.pressed()) {
			currentFrame = 4;
		}
		else if (velocity.x > 0 && KeyLeft.pressed()) {
			currentFrame = 4;
		}
	}
	

	if (jump == true && orientation == false) {
		currentFrame = 1;
	}else if(jump == true && orientation == true) {
		currentFrame = 0;
	}

	if (GameClear == true && goalTimer >= 16) {
		currentFrame = 0;
		if (goalTimer < 19) {
			currentFrame = 1;
		}

		if (goalTimer < 16.5) {
			if (zurasi == false)
			{
				zurasi = true;
				position.x += 40;
			}
			currentFrame = 2;
		}
	}
	// 現在のセグメントの描画範囲を計算
	Rect currentSegment(currentFrame * segmentWidth, 0, segmentWidth, segmentHeight);

	texture(currentSegment).mirrored(orientation && currentTexturePath == U"example/resource/チビマリオ.png" ||
	orientation&& newTexturePath == U"example/resource/スーパーマリオ.png" ||
	orientation && newTexturePath == U"example/resource/ファイアーマリオ.png" ||
	orientation && newTexturePath == U"example/resource/キノコアニメーション右.png").draw(position.x - cameraOffset.x, position.y - (size.y * marioSizeM2));
}
