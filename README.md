# 超合体！雪ダル魔神サンダークロス  

プランナー２名 / プログラマー ２名 / CGデザイナー 4名 / 声優 3名　　

計11名で制作した、アクションゲームです。

制作期間 2024/11/29～2025/03/24　　

**概要説明**  
動いていれば死なない2人協力アクションゲーム!
敵に攻撃をする際は合体して攻め込もう!
大きいほど合体時間は長くなるが、被弾リスクも大きい!　

# 開発環境
[DXライブラリ](https://dxlib.xsrv.jp/)  
[nlomann-json](https://github.com/nlohmann/json)  

# プロジェクト構成  
<details>

<summary>プロジェクト構成</summary>


```
.
|── AppFrame(ゲームを作成する際に必要となる機能や便利な機能をまとめたライブラリ)
|   |
|   └── source(ソースファイル)
|       |
|       |── Application(実行に必要となる機能や、汎用性が高い機能をまとめたファイルを格納)
|       |   |
|       |   |── ApplicationBase(DxLibの初期化や全体のループを管理するクラス)
|       |   |
|       |   |── UtilMacro(よく使う機能をマクロとしてまとめたもの)
|       |   |
|       |   └── WinMain(ゲーム全体の処理を行うもの)
|       |
|       |── EffectServer(エフェクトや影関連をまとめたもの)
|       |   |
|       |   |── EffectController(EffectManagerを継承して作られたエフェクトの管理クラス)
|       |   |
|       |   |── EffectManager(画面効果用の基底クラス)
|       |   |
|       |   |── PlayerEmphasisEffect(プレイヤーの描画を別個に行い、ほかのモデルを貫通して描画する)
|       |   |
|       |   └── ShadowMapSpriteComponent(落ち影を描画する)
|       |
|       |── ModelServer(3Dモデル関連をまとめたもの)
|       |   |
|       |   └── ModelServer(3Dモデル関連のクラスを管理するクラス)
|       |
|       |── ModeServer(ゲームを作成する際に必要なフレームワークをまとめたもの)
|       |   |
|       |   └── ModeServer(3Dモデル関連のクラスを管理するクラス)
|       |
|       |── ModeServer(ゲームを作成する際に必要なフレームワークをまとめたもの)
|       |   |
|       |   |── UI(UIアクターやそのコンポーネントをまとめたもの)
|       |   |   |
|       |   |   |── UIChipClass(UIに使用するアクタクラス)
|       |   |   |
|       |   |   |── UIChipEffectComponent(UIChipClassに適用する動作をまとめたファイル)
|       |   |   |
|       |   |   └── UIChipSpriteComponent(UIの描画を行うコンポーネント)
|       |   |   
|       |   |── ActorClass(オブジェクトの基本となる基底クラス)
|       |   |
|       |   |── Component(アクタクラスに持たせる動作の基底クラス)
|       |   |
|       |   |── EffectSpriteComponent(Effekseerで作成したエフェクトの描画用コンポーネント)
|       |   |
|       |   |── HitCollisionComponent(攻撃などの当たり判定用コリジョンのコンポーネント)
|       |   |
|       |   |── ModeBase(ゲームのシーンを作成する際の基底クラス)
|       |   |
|       |   |── ModelComponent(3Dモデルの管理、描画を行うコンポーネント)
|       |   |
|       |   |── ModeServer(ゲームのシーンを作成する際の基底クラス)
|       |   |
|       |   |── MoveCollisionComponent(地形との当たり判定用コリジョンのコンポーネント)
|       |   |
|       |   |── MoveComponent(移動を行うコンポーネント)
|       |   |
|       |   └── SpriteComponent(描画関連の基底クラス)
|       |
|       |── SoundServer(音声関連のクラスをまとめたフォルダ)
|       |   |
|       |   |── SoundServer(SEやBGM、ボイスなどを管理するクラス)
|       |   |
|       |   |── SourceVoiceItem(XAudio2におけるSourceVoiceのラッパークラス)
|       |   |
|       |   |── SourceVoiceItemEffect(SourceVoiceItemに音響処理等を追加できるクラス)
|       |   |
|       |   └── WAVRead(WAVファイルを読み込む関数)
|       |
|       └── appframe.h(AppFrame内のファイルのヘッダーを一つにまとめたもの)
|
|── DxLib(ライブラリ用ディレクトリ)
|
|── Game(ゲーム本体の Project)
|   |
|   |── Game
|   |   |
|   |   └── source(ソースファイル)
|   |        |
|   |        |── include
|   |        |   |
|   |        |   └── nlohmann(nlohmann-jsonのインクルードファイル)
|   |        |
|   |        |── ApplicationGlobal(共有したい値を格納しておくクラス)
|   |        |
|   |        |── ApplicationMain(ApplicationBaseを継承し、実際に処理を行うクラス)
|   |        |
|   |        |── BGMActor(インゲームのBGMを管理するアクタクラス)
|   |        |
|   |        |── BossActor(ボスの制御を行うアクタクラス)
|   |        |
|   |        |── BossAttackActor(ボスの攻撃のアクタクラス)
|   |        |
|   |        |── BreakableBoxActor(破壊できるマップオブジェクトのアクタクラス)
|   |        |
|   |        |── BulletComponent(敵が発射する弾のアクタクラス)
|   |        |
|   |        |── CameraActor(インゲームのカメラを制御するアクタクラス)
|   |        |
|   |        |── ChangeSnowBallActor(返信途中の演出用の雪玉アクタクラス)
|   |        |
|   |        |── DaikanpaActor(サンダークロスの技、ダイカン波のアクタクラス)
|   |        |
|   |        |── EBoxComponent(四角敵の制御を行うコンポーネント)
|   |        |
|   |        |── ECornComponent(三角敵の制御を行うコンポーネント)
|   |        |
|   |        |── EnemyActor(敵の制御を行うクラス)
|   |        |
|   |        |── EnemyComponent(敵の制御を行う基底コンポーネントクラス)
|   |        |
|   |        |── EnemyCreator(敵の生成を行うクラス)
|   |        |
|   |        |── EnemySpawnerActor(敵をフィールドにスポーンさせるアクタクラス)
|   |        |
|   |        |── EnemyStruct(敵に使用する構造体をまとめたもの)
|   |        |
|   |        |── ExplosionActor(それぞれのカメラクラスで共通で使用する変数がある基底クラス)
|   |        |
|   |        |── GimmickWallActor(当たり判定処理の管理を行うCollisionManagerクラスに情報を登録するためのクラス)
|   |        |
|   |        |── GoalItemActor(ObjectBaseから派生したキャラクターのもととなる基底クラス)
|   |        |
|   |        |── GroupAttackActor(FrameDataからのコマンドを受け取り、それに応じたエフェクトやSEを再生するクラス)
|   |        |
|   |        |── ioJson(当たり判定処理の管理を行うクラス)
|   |        |
|   |        |── ioJsonText(一番弱いクライスターのパターン1(ガラス)のクラス 突進攻撃をします)
|   |        |
|   |        |── ItemActor(クライスターのパターン2(岩)のクラス 頭突き攻撃をします。)
|   |        |
|   |        |── LaserActor(一番強いクライスターのパターン3(鉄)のクラス 回転しながら追尾攻撃をします。)
|   |        |
|   |        |── LavaActor(クライスターの屋根部分を描画するためのクラス)
|   |        |
|   |        |── ModeEnemyCon(エフェクトを再生するための基底クラス)
|   |        |
|   |        |── ModeGame(全てのエフェクトを管理するためのクラス)
|   |        |
|   |        |── ModeGameGoal(エフェクシアを再生するための基本的なクラス)
|   |        |
|   |        |── ModeGameOver(位置情報のあるアドレスを受け取り移動するエフェクシアを再生するクラス)
|   |        |
|   |        |── ModeGameUI(回転攻撃のエフェクトを再生するクラス)
|   |        |
|   |        |── ModelComponent(エネミーの基本行動や共通の変数が書かれた基底クラス)
|   |        |
|   |        |── ModeLoading(エネミーの生成、管理を行うクラス)
|   |        |
|   |        |── ModeMenu(エネミーで使用する構造体を定義するヘッダーです。)
|   |        |
|   |        |── ModeOption(ゲームステージある床(タイル)を管理するクラス)
|   |        |
|   |        |── ModeScenario(プレイヤーが決められた範囲外に出るとフォグが発生するクラス)
|   |        |
|   |        |── ModeStage(ファイルから読み込んだキャラクターのモーションに合わせて行う処理)
|   |        |
|   |        |── ModeStageSelect(プレイヤーが触れればクリアになるオブジェクトクラス)
|   |        |
|   |        |── ModeStory(プレイヤーが触れたらＨＰを回復するオブジェクトクラス)
|   |        |
|   |        |── ModeTestStage( プレイヤーが持つ鉄球・鎖の制御・描画を行うクラス)
|   |        |
|   |        |── ModeTitle(ファイルからデータを読み込みライトを設定するクラス)
|   |        |
|   |        |── ModeTitleMenu(loading時の鎖を制御・描画を行うクラス)
|   |        |
|   |        |── OutlineComponent(loading時のキャラクターのを制御・描画を行うクラス)
|   |        |
|   |        |── PlayerActor(ボスバトルモードの制御を行うクラス)
|   |        |
|   |        |── PlayerCursorComponent(クリア時にリザルトと次のステージへの遷移を管理するクラス)
|   |        |
|   |        |── PlayerMoveCollisionComponent(ステージ遷移時にフェードイン、フェードアウトを行うためのクラス)
|   |        |
|   |        |── PlayerMoveComponent(ステージ遷移時にフェードインしてからフェードアウトを行うためのクラス)
|   |        |
|   |        |── PunchActor(ボスバトル以外のステージを管理するクラス)
|   |        |
|   |        |── SlashActor(ゲームオーバー時に遷移し、リトライかタイトルに戻るかを選択するモード)
|   |        |
|   |        |── ModeInstructions(ポーズ画面から遷移できる、操作説明画面を表示するクラス)
|   |        |
|   |        |── ModelColor(モデルの白点滅処理を行うクラス)
|   |        |
|   |        |── ModeLoading(loading時のキャラクターや鉄球などの処理・描画を行うクラス)
|   |        |
|   |        |── ModeMovie(ゲームクリア時にエンディングムービーを再生するクラス)
|   |        |
|   |        |── ModePause(音量設定や振動設定、操作説明画面、ゲーム終了を選択するポーズ画面のクラス)
|   |        |
|   |        |── ModeRotationCamera(ステージ開始時のカメラの回転処理を行うクラス)
|   |        |
|   |        |── ModeScenario(シナリオを再生するクラス)
|   |        |
|   |        |── ModeTitle(タイトル画面の処理を行うクラス)
|   |        |
|   |        |── ModeTutorial(ステージの最初にチュートリアル画面を表示するクラス)
|   |        |
|   |        |── ModeZoomCamera(ゲート表示時にカメラを移動させるクラス)
|   |        |
|   |        |── MotionList(各キャラクターのモーションリストを管理するクラス)
|   |        |
|   |        |── myJson(jsonファイルを読み込むクラス)
|   |        |
|   |        |── ObjectBase(当たり判定処理を行う全てのオブジェクトの基底クラス)
|   |        |
|   |        |── PlaneEffectBase( 板ポリゴンを表示するエフェクトの基底クラス)
|   |        |
|   |        |── Player(プレイヤーキャラクターの制御・描画を行うクラス)
|   |        |
|   |        |── RotationCamera(カメラを回転させる処理をするクラス)
|   |        |
|   |        |── ScreenVibration(画面上でのカメラが見ている映像の中心座標をずらす処理行うクラス)
|   |        |
|   |        |── SlaBlock(スラブロックの攻撃パターン1(岩)のクラス 1度だけプレイヤーの真上から落下攻撃を行います)
|   |        |
|   |        |── SlaBlockPattern2(スラブロックの攻撃パターン2(ガラス,鉄)のクラス 1度だけプレイヤーの真上から落下攻撃を行います)
|   |        |
|   |        |── Suppression(敵や建物を破壊した時に減少していく制圧ゲージを管理するクラス)
|   |        |
|   |        |── TargetZoomCamera(今のカメラの位置を指定の位置に移動する処理を行うクラス)
|   |        |
|   |        |── TimeLimit(制限時間の管理を行うクラス)
|   |        |
|   |        |── Tower(タワーオブジェクトの制御・描画を行うクラス)
|   |        |
|   |        |── TowerParts(タワーパーツの制御を行うクラス)
|   |        |
|   |        |── UIBase(UIの基底クラス)
|   |        |
|   |        |── UIBossHp(ボスのHPを表示するUIクラス)
|   |        |
|   |        |── UIExpPoint(経験値のUIを表示するクラス)
|   |        |
|   |        |── UIHeart(プレイヤーHPのUIを表示するクラス)
|   |        |
|   |        |── UISuppressionGauge(制圧ゲージを表示するクラス)
|   |        |
|   |        |── UITimeLimit(残り時間を表示するUIクラス)
|   |        |
|   |        └── UnbreakableBuilding(破壊不可能な建物のクラス)
|   |
|   └── Game.sln(コードを見る際は、こちらを起動して下さい)
|
|── .gitignore
|
└── README.md
```
</details>

# 起動方法  
上記のプロジェクトをダウンロードし、解凍後「Game/Game.sln」を起動  

# AppFrameについて  
ゲームを作成する上で定形的に必要となる機能また今までの制作で汎用的に使える機能をまとめたフレームワーク  
Static Library の形式で提供(Project 設定)  

***ApplicationBase***  

DX ライブラリを用いた Windows アプリケーションを作成する基底クラス  
本クラスを継承したクラスを作成すれば最小のコードでアプリのメイン部分を記述出来る  

***ModeBase / ModeServer***  

本フレームワークはモードと呼ぶ単位でプログラミング可能になっており、モードを切り替える機能も提供します。  
これによりアプリ作成者は、モード単位の実装をする事でゲームのフローを構築出来ます。  

ModeBase を継承したクラス単位でタイトルやインゲーム、設定画面など好きな単位で構築できModeServer が各モードを切り替える機能を提供します。  