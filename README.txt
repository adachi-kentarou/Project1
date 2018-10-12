作品名　RANDOM MAZE
制作者　安達健太郎

開発環境
OS win10
エディタ visual studio 2017
言語 C++11

インストールソリューションパッケージ
glm
nupengl.core
nupengl.core.redist

使用SDK
CRI ADX2 LE SDK

ビルド方法
ソリューション構成　Debug
ソリューションプラットフォーム　x86
新規でビルドする際は
プロジェクト直下の
cri\pc\libs\x86￥cri_ware_pcx86_le.dllファイルと
Resディレクトリーを
Debugディレクトリーにコピーする必要があります
（カスタムビルドステップで自動でコピーするようにしていますがコピーに失敗した場合は
手動でコピーしてください）
visual studioがインストールされていない環境でも実行できるようにする場合Debugディレクトリーに
msvcp140d.dll
ucrtbased.dll
vcruntime140d.dll
msvcp140.dll
vccorlib140.dll
vcruntime140.dll
をインターネット上から入手しコピーする必要があります。

ゲーム実行方法
プロジェクトディレクトリ直下Debug/Project1.exeを実行

ゲーム操作説明
マウスクリック　ゲーム開始、プレイヤー前方攻撃
マウスホイール　カメラズームイン、アウト
マウス移動　視点変更
A、Dキー　左右方向転換
W、Sキー　前後移動

ゲームルール
制限時間内にステージ上にいる敵を全て倒せばステージクリア
ステージが進むごとに敵、アイテム、迷路の距離が増える
全ステージをクリアーするとゲームクリア
敵に隣接すると攻撃される
HPが0になるか制限時間が過ぎるとゲームオーバー
ライフアイテムを取るとHP回復
タイムアイテムを取ると制限時間延長

制作期間
約3か月

工夫した点
●定数ファイルを作成しゲームの設定を管理するようにした
Constants.h

●VBO,VAO,IBOを作成し処理の効率化を図った

●ゲームエンジンを作成しVBO,VAO,IBO,メインループ処理、描画処理を管理するようにした
Src/GameEngine.h
Src/GameEngine.cpp

●画面に表示させるクラスの基底クラスになる、
移動回転縮小同クラスの親子関係を管理するNodeクラスを作成
Src/Node.h
Src/Node.cpp

●自動生成で地形を作るアルゴリズムを作成した
Src/LoadState.h
Src/LoadState.cpp
	void CreateMap()

●Nodeクラスを継承したポリゴン表示クラスを作成
Entity.h
Entity.cpp

●Nodeクラスとポリゴン表示クラスをキャラクターのパーツとしてまとめたキャラクタークラスと
アイテム表示クラスを作成
Charactor.h
Charactor.cpp

●キャラクタークラスを継承した敵キャラプレイヤーキャラクラスを作成
EnemyCharactor.h
EnemyCharactor.cpp
PlayerCharactor.h
PlayerCharactor.cpp

●アイテム表示クラスを継承しライフアイテムクラスとタイムアイテムクラスを作成
Item.h
Item.cpp

●マウス座標から表示カメラ角度を計算するようにした
MainGameState.cpp
197行目