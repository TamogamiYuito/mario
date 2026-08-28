#include "Stage.h"

Item::Item()
	: position(0, 0), velocity(0, 0),
	texture(U"example/resource/キノコ.png"),
	currentTexturePath(U"example/resource/キノコ.png") {}

Item::Item(const Vec2& pos, const Texture& tex)
	: position(pos), velocity(0, 0),
	texture(tex),
	currentTexturePath(U"example/resource/キノコ.png") // 初期化時にパスを保存する
{}


Enemy::Enemy()
	: position(0, 0), velocity(0, 0),
	texture(U"example/resource/クリボー.png"),
	currentTexturePath(U"example/resource/クリボー.png") {}

Enemy::Enemy(const Vec2& pos, const Texture& tex)
	: position(pos), velocity(0, 0),
	texture(tex),
	currentTexturePath(U"example/resource/クリボー.png") // 初期化時にパスを保存する
{}


RectF Item::getBoundingRect(const Vec2& cameraOffset) const
{
	return RectF(position.x - cameraOffset.x, position.y, size); // プレイヤーの当たり判定矩形を返す
}


RectF Enemy::getBoundingRect(const Vec2& cameraOffset) const
{
	return RectF(position.x - cameraOffset.x, position.y, size); // プレイヤーの当たり判定矩形を返す
}



void Item::anime() {
	if (!BoolAnime) return;
	if (type == Mushroom) { newTexturePath = U"example/resource/キノコ.png"; }
	if (type == fire) { newTexturePath = U"example/resource/ファイアーフラワー.png"; }
	velocity.y = -1;

	position += velocity;

	if (position.y - posY < -64 + 15) {
		isMoveR = true;
		enable = true;
		BoolAnime = false;
	}

	// テクスチャを更新
	if (newTexturePath != currentTexturePath) {
		texture = Texture(newTexturePath);
		currentTexturePath = newTexturePath;
	}
}


void Item::update() {
	if (!enable) return;

	if (type == Mushroom) {
		if (BoolAnime)return;
		//落下処理
		const double gravity = 0.3;
		isOnGround = false;
		// 重力を適用
		if (!isOnGround)
		{
			velocity.y += gravity; // 重力加速度
		}
		else
		{
			velocity.y = 0;
		}


		//移動
		if (isMoveR) {
			velocity.x = 1;
		}
		else if (isMoveL) {
			velocity.x = -1;
		}
		else {
			velocity.x = 0;
		}

		//すべてのベクトルの統合
		position += velocity;
	}else if (type == fire) {
	}


	
}


Array<Item> items;



void Enemy::update() {
	if (!enable) return;

	if (type == Kuriboh) {
		//落下処理

		const double gravity = 0.3;
		isOnGround = false;
		// 重力を適用
		if (!isOnGround)
		{
			velocity.y += gravity; // 重力加速度
		}
		else
		{
			velocity.y = 0;
		}


		//移動
		if (isMoveR) {
			velocity.x = 1;
		}
		else if (isMoveL) {
			velocity.x = -1;
		}
		else {
			velocity.x = 0;
		}

		//すべてのベクトルの統合
		position += velocity;
	}
	else if (type == Koopa) {
	}



}

Array<Enemy> enemys;


void Game::update()
{

	for (auto&& item : items) {
		item.anime();
		item.update();
	}

	for (auto&& enemy : enemys) {
		enemy.update();
	}




	float mapC = 350;
	bool isPlayerOnGround = false;
	bool MoveR = true;
	bool MoveL = true;

	if (animeTime >= 0) {
		animeTime -= Scene::DeltaTime();
	}
	else {
		animeX = NULL;
		animeY = NULL;
		animeX1 = NULL;
		animeY1 = NULL;
	}

	// 当たり判定処理
	for (int y = 0; y < map.height(); ++y)
	{
		for (int x = 0; x < map.width(); ++x)
		{
			if (y < 0 || y >= map.height() || x < 0 || x >= map.width())
			{
				continue; // 範囲外アクセスを防ぐ
			}
			int32 chipNo = map[y][x];
			RectF OnTheBlock(x * 64 - cameraOffset.x - mapC + 5, y * 100, 53, 5); // ブロックの矩形
			RectF BlockOnTheLeft(x * 64 - cameraOffset.x - mapC, y * 100 + 5, 5, 100);
			RectF BlockOnTheRight(x * 64 - cameraOffset.x - mapC + 59, y * 100 + 5, 5, 100);

			if (chipNo == 1) // 特定のブロックID 1 に対する処理
			{
				//アイテムの当たり判定
				for (auto&& item : items) {
					if (item.enable == true) {
						if (item.getBoundingRect(cameraOffset).intersects(OnTheBlock))
						{
							// 下方向の衝突
							if (item.position.y + 55 + player.size.y >= OnTheBlock.y && item.position.x + item.size.x >= OnTheBlock.x && item.velocity.y >= 0)
							{
								item.position.y = OnTheBlock.y - item.size.y + 1;
								item.velocity.y = 0;
								item.isOnGround = true;
							}
						}
						if (item.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
						{
							if (item.position.y + player.size.y > BlockOnTheLeft.y)
							{
								item.isMoveR = false;
							}
							else {
								item.isMoveR = true;
							}
						}

						if (item.getBoundingRect(cameraOffset).intersects(BlockOnTheRight))
						{
							if (item.position.y + player.size.y > BlockOnTheLeft.y)
							{
								item.isMoveL = false;
							}
							else {
								item.isMoveL = true;
							}
						}
					}
				}


				//enemyの当たり判定
				for (auto&& enemy : enemys) {
					if (enemy.enable == true) {
						if (enemy.getBoundingRect(cameraOffset).intersects(OnTheBlock))
						{
							// 下方向の衝突
							if (enemy.position.y + 55 + player.size.y >= OnTheBlock.y && enemy.position.x + enemy.size.x >= OnTheBlock.x && enemy.velocity.y >= 0)
							{
								enemy.position.y = OnTheBlock.y - enemy.size.y + 1;
								enemy.velocity.y = 0;
								enemy.isOnGround = true;
							}
						}
						if (enemy.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
						{
							if (enemy.position.y + player.size.y > BlockOnTheLeft.y)
							{
								enemy.isMoveR = false;
							}
							else {
								enemy.isMoveR = true;
							}
						}

						if (enemy.getBoundingRect(cameraOffset).intersects(BlockOnTheRight))
						{
							if (enemy.position.y + player.size.y > BlockOnTheLeft.y)
							{
								enemy.isMoveL = false;
							}
							else {
								enemy.isMoveL = true;
							}
						}
					}
				}


				//プレイヤーの当たり判定
				if (player.GameOver == false) {
					if (player.getBoundingRect(cameraOffset).intersects(OnTheBlock))
					{
						// 下方向の衝突
						if (player.position.y + 55 + player.size.y >= OnTheBlock.y && player.position.x + player.size.x >= OnTheBlock.x && player.velocity.y >= 0)
						{
							player.position.y = OnTheBlock.y - player.size.y + 1;
							player.velocity.y = 0;
							isPlayerOnGround = true;
						}
					}

					if (player.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
					{
						if (player.position.y + player.size.y > BlockOnTheLeft.y)
						{
							MoveR = false;
							if (player.velocity.x > 0) {
								player.velocity.x = 0;
							}
						}
						else {
							MoveR = true;
						}
					}

					if (player.getBoundingRect(cameraOffset).intersects(BlockOnTheRight))
					{
						if (player.position.y + player.size.y > BlockOnTheLeft.y)
						{
							MoveL = false;
							if (player.velocity.x < 0) {
								player.velocity.x = 0;
							}
						}
						else {
							MoveL = true;
						}
					}
				}
			}
		}
	}
	// 当たり判定処理
	for (int y = 0; y < BlockMap.height(); ++y)
	{
		for (int x = 0; x < BlockMap.width(); ++x)
		{
			if (y < 0 || y >= BlockMap.height() || x < 0 || x >= BlockMap.width())
			{
				continue; // 範囲外アクセスを防ぐ
			}

			int32 StageNo = BlockMap[y][x];
			RectF OnTheBlock(x * 64 - cameraOffset.x - mapC + 5, y * 64 - 32, 53, 5); // ブロックの矩形
			RectF BlockOnTheLeft(x * 64 - cameraOffset.x - mapC, y * 64 - 27, 5, 54);
			RectF BlockOnTheRight(x * 64 - cameraOffset.x - mapC + 59, y * 64 - 27, 5, 54);
			RectF UnderTheBlock(x * 64 - cameraOffset.x - mapC + 5, y * 64 + 27, 53, 5);

			RectF OnTheEarthenPipe(x * 64 - cameraOffset.x - mapC + 5, y * 64 - 32, 58 * 2 - 5, 5);
			RectF EarthenPipeOnTheRight(x * 64 - cameraOffset.x - mapC + 59 + 64, y * 64 - 27, 5, 54);

			RectF Goal(x * 64 - cameraOffset.x - mapC + 32, y * 64, 5, 64 * 9);




			if (StageNo == 9) {
				BlockMap[y][x] = 0;
				enemys << Enemy();
				for (auto&& enemy : enemys) {
					if (enemy.enable == false) {
						enemy.enable = true;
						enemy.type = Kuriboh;
						enemy.position = Vec2(x * 64 - mapC, y * 64 - 32);
						enemy.isMoveR = true;
					}
				}
			}



			if (StageNo != 0 && StageNo != 4 && StageNo != 5 && StageNo != 6 && StageNo != 12) // 特定のブロックID 1 に対する処理
			{
				//アイテムの当たり判定
				for (auto&& item : items) {
					if (item.enable == true) {
						if (item.getBoundingRect(cameraOffset).intersects(OnTheBlock))
						{
							// 下方向の衝突
							if (item.position.y + 55 + item.size.y >= OnTheBlock.y && item.position.x + item.size.x >= OnTheBlock.x && item.velocity.y >= 0)
							{
								item.position.y = OnTheBlock.y - item.size.y + 1;
								item.velocity.y = 0;
								item.isOnGround = true;
							}
						}

						if (item.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
						{
							if (item.position.y + item.size.y > BlockOnTheLeft.y)
							{
								item.isMoveR = false;
							}
							else {
								item.isMoveR = true;
							}
						}

						if (item.getBoundingRect(cameraOffset).intersects(BlockOnTheRight))
						{
							if (item.position.y + item.size.y > BlockOnTheLeft.y)
							{
								item.isMoveL = false;
							}
							else {
								item.isMoveL = true;
							}
						}
						if (item.getBoundingRect(cameraOffset).intersects(UnderTheBlock) && item.velocity.y <= 0)
						{
							item.velocity.y = 0;
						}
					}
				}


				//enemyの当たり判定
				for (auto&& enemy : enemys) {
					if (enemy.enable == true) {
						if (enemy.getBoundingRect(cameraOffset).intersects(OnTheBlock))
						{
							// 下方向の衝突
							if (enemy.position.y + 55 + enemy.size.y >= OnTheBlock.y && enemy.position.x + enemy.size.x >= OnTheBlock.x && enemy.velocity.y >= 0)
							{
								enemy.position.y = OnTheBlock.y - enemy.size.y + 1;
								enemy.velocity.y = 0;
								enemy.isOnGround = true;
							}
						}

						if (enemy.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
						{
							if (enemy.position.y + enemy.size.y > BlockOnTheLeft.y)
							{
								enemy.isMoveR = false;
								enemy.isMoveL = true;
							}
							else {
								enemy.isMoveR = true;
								enemy.isMoveL = false;
							}
						}

						if (enemy.getBoundingRect(cameraOffset).intersects(BlockOnTheRight))
						{
							if (enemy.position.y + enemy.size.y > BlockOnTheLeft.y)
							{
								enemy.isMoveR = true;
								enemy.isMoveL = false;
							}
							else {
								enemy.isMoveL = true;
								enemy.isMoveR = false;
							}
						}
						if (enemy.getBoundingRect(cameraOffset).intersects(UnderTheBlock) && enemy.velocity.y <= 0)
						{
							enemy.velocity.y = 0;
						}
					}
				}


				//プレイヤーの当たり判定
				if (player.GameOver == false) {
					if (player.getBoundingRect(cameraOffset).intersects(OnTheBlock))
					{
						// 下方向の衝突
						if (player.position.y + 55 + player.size.y >= OnTheBlock.y && player.position.x + player.size.x >= OnTheBlock.x && player.velocity.y >= 0)
						{
							player.position.y = OnTheBlock.y - player.size.y + 1;
							player.velocity.y = 0;
							isPlayerOnGround = true;
						}
					}

					if (player.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
					{
						if (player.position.y + player.size.y > BlockOnTheLeft.y)
						{
							MoveR = false;
							if (player.velocity.x > 0) {
								player.velocity.x = 0;
							}
						}
						else {
							MoveR = true;
						}
					}

					if (player.getBoundingRect(cameraOffset).intersects(BlockOnTheRight))
					{
						if (player.position.y + player.size.y > BlockOnTheLeft.y)
						{
							MoveL = false;
							if (player.velocity.x < 0) {
								player.velocity.x = 0;
							}
						}
						else {
							MoveL = true;
						}
					}
					if (player.getBoundingRect(cameraOffset).intersects(UnderTheBlock) && player.velocity.y <= 0)
					{
						player.velocity.y = 0;

						if (player.bigMario == true && StageNo == 3) {
							BlockMap[y][x] = 0;
						}


						if (StageNo == 2 || StageNo == 3 || StageNo == 8) {
							if (animeX == NULL && animeY == NULL) {
								animeX = x;
								animeY = y;
								animeTime = 0.2;
							}
							else if (animeX != NULL && animeY != NULL) {
								animeX1 = x;
								animeY1 = y;
							}
						}
					}
				}


			}
			else if (StageNo == 4 || StageNo == 5)
			{
				//アイテムの当たり判定
				for (auto&& item : items) {
					if (item.getBoundingRect(cameraOffset).intersects(OnTheEarthenPipe))
					{
						// 下方向の衝突
						if (item.position.y + 55 + item.size.y >= OnTheEarthenPipe.y && item.position.x + item.size.x >= OnTheEarthenPipe.x && item.velocity.y >= 0)
						{
							item.position.y = OnTheEarthenPipe.y - item.size.y + 1;
							item.velocity.y = 0;
							item.isOnGround = true;
						}
					}

					if (item.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
					{
						if (item.position.y + item.size.y > BlockOnTheLeft.y)
						{
							item.isMoveR = false;
							item.isMoveL = true;
						}
						else {
							item.isMoveR = true;
							item.isMoveL = false;
						}
					}

					if (item.getBoundingRect(cameraOffset).intersects(EarthenPipeOnTheRight))
					{
						if (item.position.y + item.size.y > EarthenPipeOnTheRight.y)
						{
							item.isMoveR = true;
							item.isMoveL = false;
						}
						else {
							item.isMoveL = true;
							item.isMoveR = false;
						}
					}
				}


				//enemyの当たり判定
				for (auto&& enemy : enemys) {
					if (enemy.getBoundingRect(cameraOffset).intersects(OnTheEarthenPipe))
					{
						// 下方向の衝突
						if (enemy.position.y + 55 + enemy.size.y >= OnTheEarthenPipe.y && enemy.position.x + enemy.size.x >= OnTheEarthenPipe.x && enemy.velocity.y >= 0)
						{
							enemy.position.y = OnTheEarthenPipe.y - enemy.size.y + 1;
							enemy.velocity.y = 0;
							enemy.isOnGround = true;
						}
					}

					if (enemy.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
					{
						if (enemy.position.y + enemy.size.y > BlockOnTheLeft.y)
						{
							enemy.isMoveR = false;
							enemy.isMoveL = true;
						}
						else {
							enemy.isMoveR = true;
							enemy.isMoveL = false;
						}
					}

					if (enemy.getBoundingRect(cameraOffset).intersects(EarthenPipeOnTheRight))
					{
						if (enemy.position.y + enemy.size.y > EarthenPipeOnTheRight.y)
						{
							enemy.isMoveR = true;
							enemy.isMoveL = false;
						}
						else {
							enemy.isMoveL = true;
							enemy.isMoveR = false;
						}
					}
				}


				//プレイヤーの当たり判定
				if (player.GameOver == false) {
					if (player.getBoundingRect(cameraOffset).intersects(OnTheEarthenPipe))
					{
						// 下方向の衝突
						if (player.position.y + 55 + player.size.y >= OnTheEarthenPipe.y && player.position.x + player.size.x >= OnTheEarthenPipe.x && player.velocity.y >= 0)
						{
							player.position.y = OnTheEarthenPipe.y - player.size.y + 1;
							player.velocity.y = 0;
							isPlayerOnGround = true;
						}
					}

					if (player.getBoundingRect(cameraOffset).intersects(BlockOnTheLeft))
					{
						if (player.position.y + player.size.y > BlockOnTheLeft.y)
						{
							MoveR = false;
							if (player.velocity.x > 0) {
								player.velocity.x = 0;
							}
						}
						else {
							MoveR = true;
						}
					}

					if (player.getBoundingRect(cameraOffset).intersects(EarthenPipeOnTheRight))
					{
						if (player.position.y + player.size.y > EarthenPipeOnTheRight.y)
						{
							MoveL = false;
							if (player.velocity.x < 0) {
								player.velocity.x = 0;
							}
						}
						else {
							MoveL = true;
						}
					}
				}
			}
			else if (StageNo == 6) {
				if (player.getBoundingRect(cameraOffset).intersects(Goal)) {
					if (player.GameClear == false) {
						player.velocity = { 0, 0 };
					}
					player.GameClear = true;
				}
			}
			else if (StageNo == 12) {
				if (player.getBoundingRect(cameraOffset).intersects(BlockOnTheRight)) {
					player.enable = false;
				}
			}
		}
	}


	//アイテムとの当たり判定
	for (auto&& item : items) {
		if (player.GameOver == true)break;
		if (player.getBoundingRect(cameraOffset).intersects(item.getBoundingRect(cameraOffset)) && item.enable == true) {
			if (item.type == Mushroom) {
				player.marioType = SuperMario;
			}
			if (item.type == fire) {
				player.marioType = fireMario;
			}
			item.enable = false;
		}
	}

	for (auto&& enemy : enemys) {
		if (player.GameOver == true)break;
		if (player.getBoundingRect(cameraOffset).intersects(enemy.getBoundingRect(cameraOffset)) && enemy.enable == true) {
			if (player.position.y < enemy.position.y - enemy.size.y + 15) {
				enemy.BoolAnime = true;
				enemy.enable = false;
				player.velocity.y = -10;
			}
			else {
				if (player.marioType == fireMario && DMcooltime < 0) {
					player.marioType = SuperMario;
					DMcooltime = 2;
				}
				else if (player.marioType == SuperMario && DMcooltime < 0) {
					player.marioType = smallMario;
					DMcooltime = 2;
				}
				else if (DMcooltime < 0){
					player.GameOver = true;
				}
			}
		}
	}

	if (DMcooltime >= 0) {
		DMcooltime -= Scene::DeltaTime();
	}

	player.isOnGround = isPlayerOnGround;
	player.isMoveL = MoveL;
	player.isMoveR = MoveR;



	// プレイヤーの位置を制限して、カメラの左端より左に行けないようにする
	if (player.position.x < cameraOffset.x)
	{
		player.position.x = cameraOffset.x;
	}

	// プレイヤーがカメラの中央を越えたらカメラを移動する
	if (player.position.x - cameraOffset.x > 450 && player.GameClear == false)
	{
		cameraOffset = player.position - Vec2{ 450, 0 };
	}
	// プレイヤーの更新処理
	player.update();
	player.Goalupdate();
}



void Item::draw(Vec2& cameraOffset) {
	if (!BoolAnime && !enable) return;

	if (type == fire) {
		divisions = 4;
	}
	else {
		divisions = 1;
	}

	const int32 segmentWidth = texture.width() / divisions;
	const int32 segmentHeight = texture.height();
	const int32 frameRate = 10;

	// 現在のフレーム数から、現在表示すべきセグメントを計算
	int32 currentFrame = static_cast<int32>((Scene::Time() * frameRate)) % divisions;
	Rect currentSegment(currentFrame * segmentWidth, 0, segmentWidth, segmentHeight);

	texture(currentSegment).draw(position.x - cameraOffset.x, position.y);
}


void Enemy::draw(Vec2& cameraOffset) {
	if (enable == false && BoolAnime == false)return;

		divisions = 3;

	const int32 segmentWidth = texture.width() / divisions;
	const int32 segmentHeight = texture.height();
	const int32 frameRate = 10;

	// 現在のフレーム数から、現在表示すべきセグメントを計算
	int32 currentFrame = static_cast<int32>((Scene::Time() * frameRate)) % divisions;

	if (currentFrame >= 2) {
		currentFrame = 0;
	}

	if (BoolAnime == true) {
		currentFrame = 2;
		dieTime -= Scene::DeltaTime();

		if (dieTime < 0) {
			BoolAnime = false;
		}
	}


	Rect currentSegment(currentFrame * segmentWidth, 0, segmentWidth, segmentHeight);

	texture(currentSegment).draw(position.x - cameraOffset.x, position.y);
}

void Game::draw()
{
	float mapC = 350;
	int k = 0;
	for (int y = 0; y < map.height(); ++y)
	{
		for (int x = 0; x < map.width(); ++x)
		{
			if (y < 0 || y >= map.height() || x < 0 || x >= map.width())
			{
				continue; // 範囲外アクセスを防ぐ
			}
			int32 chipNo = map[y][x];

			// キーが存在するかどうかを確認する
			if (chipDB.contains(chipNo))
			{
				Texture texture = chipDB.at(chipNo).texture;
				if (!chipDB.at(chipNo).texture.isEmpty())
				{
					if (x != 0 && x % 16 == 0 && y == 0)
					{
						k++;
						texture.draw(1024 * k - cameraOffset.x - mapC, y * 100);
					}
					else
					{
						texture.draw(x * 64 - cameraOffset.x - mapC, y * 100);
						/*RectF OnTheBlock(x * 64 - cameraOffset.x - mapC + 5, y * 100, 53, 5); // ブロックの矩形
						RectF BlockOnTheLeft(x * 64 - cameraOffset.x - mapC, y * 100 + 5, 5, 100);
						RectF BlockOnTheRight(x * 64 - cameraOffset.x - mapC + 59, y * 100 + 5, 5, 100);
						OnTheBlock.draw(ColorF(0, 1, 0, 0.5f));
						BlockOnTheLeft.draw(ColorF(0, 0, 1, 0.5f));
						BlockOnTheRight.draw(ColorF(0, 0, 1, 0.5f));*/
					}
				}
			}
		}
	}


	for (auto&& item : items) {
		item.draw(cameraOffset);
	}

	for (auto&& enemy : enemys) {
		enemy.draw(cameraOffset);
	}
	


	// 現在のフレーム数から、現在表示すべきセグメントを計算
	const int32 currentFrame = static_cast<int32>((Scene::Time() * frameRate)) % 5;
	// 現在のセグメントの描画範囲を計算
	Rect currentSegment(currentFrame * segmentWidth, 0, segmentWidth, segmentHeight);



	int32 Yanime;
	float zure;
	float scal;

	for (int y = 0; y < BlockMap.height(); ++y)
	{
		for (int x = 0; x < BlockMap.width(); ++x)
		{
			if (y < 0 || y >= BlockMap.height() || x < 0 || x >= BlockMap.width())
			{
				continue; // 範囲外アクセスを防ぐ
			}
			int32 StageNo = BlockMap[y][x];

			// キーが存在するかどうかを確認する
			if (StageDB.contains(StageNo))
			{
				Texture texture = StageDB.at(StageNo).texture;




				if (animeX == x && animeY == y || animeX1 == x && animeY1 == y) {
					if (animeTime > 0.175) {
						Yanime = 7.5;
						scal = 1.05;
						zure = 1.6;
					}
					else if (animeTime > 0.15) {
						Yanime = 15;
						scal = 1.1;
						zure = 3.2;
					}
					else if (animeTime > 0.1) {
						Yanime = 30;
						scal = 1.2;
						zure = 6.4;
					}
					else if (animeTime > 0.05) {
						Yanime = 15;
						scal = 1.1;
						zure = 3.2;

						if (StageNo == 2 || StageNo == 8) {
							BlockMap[y][x] = 7;
							if (StageNo == 8) {
									items << Item();

								for (auto&& item : items) {
									if (item.enable == false) {
										item.BoolAnime = true;
										item.posY = y * 64 - 32 - Yanime;
										item.position = Vec2(x * 64 - mapC, y * 64 - 32 - Yanime);
										item.isMoveR = false;
										item.isMoveL = false;
										item.velocity.x = 0;
										item.velocity.y = 0;

										if (player.marioType == SuperMario && StageNo == 8 ||
											player.marioType == fireMario && StageNo == 8) {
											item.type = fire;
										}
										else {
											item.type = Mushroom;
										}
										break;
									}
								}
							}
						}
					}
					else if (animeTime > 0.025) {
						Yanime = 7.5;
						scal = 1.05;
						zure = 1.6;
					}
				}
				else {
					Yanime = 0;
					scal = 1;
					zure = 0;
				}

				if (!StageDB.at(StageNo).texture.isEmpty() && StageNo != 2 && StageNo != 6 && StageNo != 8)
				{
					texture.scaled(scal).draw(x * 64 - cameraOffset.x - mapC - zure, y * 64 - 32 - Yanime);
					/*RectF OnTheBlock(x * 64 - cameraOffset.x - mapC + 5, y * 64 - 32, 53, 5); // ブロックの矩形
					RectF BlockOnTheLeft(x * 64 - cameraOffset.x - mapC, y * 64 - 27, 5, 54);
					RectF BlockOnTheRight(x * 64 - cameraOffset.x - mapC + 59, y * 64 - 27, 5, 54);
					RectF UnderTheBlock(x * 64 - cameraOffset.x - mapC + 5, y * 64 + 27, 53, 5);
					OnTheBlock.draw(ColorF(0, 1, 0, 0.5f));
					BlockOnTheLeft.draw(ColorF(0, 0, 1, 0.5f));
					BlockOnTheRight.draw(ColorF(0, 0, 1, 0.5f));
					UnderTheBlock.draw(ColorF(1, 0, 0, 0.5f));*/
				}
				else if (StageNo == 2 || StageNo == 8) {
					//セグメントを表示
					texture(currentSegment).scaled(scal).draw(x * 64 - cameraOffset.x - mapC - zure, y * 64 - 32 - Yanime);
				}
				else if (StageNo == 6) {
					//セグメントを表示
					texture.scaled(scal).draw(x * 64 - cameraOffset.x - mapC - zure - 32, y * 64 - 32 - Yanime);
				}
			}
		}
	}
	// プレイヤーの描画
	player.draw(cameraOffset);
}

Vec2 Game::cameraPosition()
{
	return Vec2(cameraOffset); // プレイヤーの当たり判定矩形を返す
}
