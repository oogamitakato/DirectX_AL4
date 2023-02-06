#include "GameScene.h"
#include "Collision.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(spriteBG);
	safe_delete(sprite1);
	safe_delete(sprite2);
	safe_delete(object3d);
	safe_delete(model);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);

	this->dxCommon = dxCommon;
	this->input = input;

	// デバッグテキスト用テクスチャ読み込み
	Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png");
	// デバッグテキスト初期化
	debugText.Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	Sprite::LoadTexture(1, L"Resources/background.png");
	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/texture.png");

	//テクスチャ2番のスプライトを生成
	sprite1 = Sprite::Create(2, { 0,0 });
	sprite2 = Sprite::Create(2, { 500,500 },{1,0,0,1},{0,0},false,true);

	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });

	//モデル読み込み
	model = Model::LoadFromOBJ("sphere");
	
	// 3Dオブジェクト生成
	object3d = Object3d::Create();
	object3d->Initialize();
	object3d->SetModel(model);
	object3d->SetScale({
		sphere.radius,
		sphere.radius,
		sphere.radius,
		});
	object3d->Update();

	//球の初期値を設定
	sphere.center = XMVectorSet(0, 2, 0, 1);
	sphere.radius = 1.0f;

	//平面の初期値を設定
	plane.normal = XMVectorSet(0, 1, 0, 0);
	plane.distance = 0.0f;

	//三角形の初期化を設定
	triangle.p0 = XMVectorSet(-1.0f, 0, -1.0f, 1);
	triangle.p1 = XMVectorSet(-1.0f, 0, +1.0f, 1);
	triangle.p2 = XMVectorSet(+1.0f, 0, -1.0f, 1);
	triangle.normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0);
}

void GameScene::Update()
{

	//スプライト移動
	if (input->PushKey(DIK_SPACE)) {
		//現在の座標を取得
		XMFLOAT2 position = sprite1->GetPosition();
		//移動後の座標を計算
		position.x += 1.0f;
		//座標の変更を反映
		sprite1->SetPosition(position);
	}


	//球移動
	XMVECTOR moveY = XMVectorSet(0, 0.1f, 0, 0);
	XMVECTOR moveX = XMVectorSet(0.1f, 0, 0, 0);
	if (input->PushKey(DIK_W)) { sphere.center += moveY; }
	if (input->PushKey(DIK_S)) { sphere.center -= moveY; }
	if (input->PushKey(DIK_D)) { sphere.center += moveX; }
	if (input->PushKey(DIK_A)) { sphere.center -= moveX; }

	//球と平面の当たり判定
	XMVECTOR inter;
	bool hit = Collision::CheckSphere2Triangle(sphere, triangle, &inter);
	if (hit) {
		debugText.Print("Hit", 50, 200, 1.0f);
	}

	//stringstreamで変数の値を埋め込んで整形する
	std::ostringstream spherestr;
	spherestr << "Sphere:("
		<< std::fixed << std::setprecision(2)
		<< sphere.center.m128_f32[0] << ","
		<< sphere.center.m128_f32[1] << ","
		<< sphere.center.m128_f32[2] << ")";

	debugText.Print(spherestr.str(), 50, 180, 1.0f);

	// 座標の変更を反映
	object3d->SetPosition({
			sphere.center.m128_f32[0],
			sphere.center.m128_f32[1],
			sphere.center.m128_f32[2],
		});

	object3d->Update();
}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Object3d::PreDraw(cmdList);

	// 3Dオブクジェクトの描画
	object3d->Draw();

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Object3d::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	//sprite1->Draw();
	//sprite2->Draw();

	// デバッグテキストの描画
	debugText.DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}
