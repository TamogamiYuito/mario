#pragma once
#include <Siv3D.hpp> // OpenSiv3D v0.6.6


enum MarioType
{
	smallMario,
	SuperMario,
	fireMario,
	// 他のアイテムタイプを追加可能
};

class Player
{

	
	String currentTexturePath; // 現在のテクスチャパスを保持
	String newTexturePath;
	MarioType previousMarioType = smallMario; // 前回のマリオのタイプを追跡するための変数


	bool orientation = false;//右向き

public:
	MarioType marioType;
    Vec2 position;
    Vec2 velocity;
    Texture texture;
    Vec2 size = Vec2(48, 64); // プレイヤーのサイズ
	int32 marioSize = 1;
	float XmarioSize = 1;
	int32 marioSizeM2 = 0;
    bool isOnGround = false;  // プレイヤーが地面に接触しているかどうか
    bool isMoveR = false;
    bool isMoveL = false;
	bool bigMario = false;

	bool enable = true;

	bool hitStop = false;          // ヒットストップがアクティブかどうかのフラグ
	double hitStopDuration = 0.0;  // ヒットストップの持続時間（秒）
	double hitStopTimer = 0.0;     // ヒットストップ時間を追跡するタイマー

	bool GameClear = false;
	bool GameOver = false;

	float goalTimer = 0;

	bool zurasi = false;

    Player(); // デフォルトコンストラクタ
    Player(const Vec2& pos, const Texture& tex);

    void update();
	void Goalupdate();
    void drawhit(const Vec2& cameraOffset);
    void draw(Vec2& cameraOffset);
	void anime();
    RectF getBoundingRect(const Vec2& cameraOffset) const;
};
