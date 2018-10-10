#pragma once
//共通パラメーター
#define WINDOW_WIDTH		800	//ウィンドウ横サイズ
#define WINDOW_HEIGHT		400	//ウィンドウ縦サイズ
#define LOOK_RADIAN			30//視点方向角度
#define MIN_CAMERA_ANGLE	15//最小カメラアングル
#define MAX_CAMERA_ANGLE	75//最大カメラアングル
#define FPS					60//秒間更新数

//迷路パラメーター
#define MAP_MAX_X			10//マップXサイズ
#define MAP_MAX_Y			10//マップYサイズ
#define MAP_MAX_Z			10//マップZサイズ
#define LOAD_LOOP			10//道作成ループ回数
#define LOAD_LENGTH			10.0f//道の直進数
#define TILE_JUDGE			0.4f//床階段の割合

//ステージパラメーター
#define STAGE_ENEMY			2//敵の数
#define STAGE_ITEM			2//アイテムの数
#define STAGE_TIME			60//制限時間
#define STAGE_NO			3//ステージ数
#define STAGE_TIME_UP		5//次ステージ制限時間アップ数
#define LOAD_LOOP_UP		5//次ステージ道作成ループ数アップ数
#define STAGE_ENEMY_UP		2//次ステージ敵の数アップ数
#define STAGE_ITEM_UP		1//次ステージアイテムの数アップ数