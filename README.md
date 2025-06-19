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
|   |        |── ExplosionActor(サンダークロスの爆発のアクタクラス)
|   |        |
|   |        |── GimmickWallActor(上下に動く壁のアクタクラス)
|   |        |
|   |        |── GoalItemActor(ゴールアイテムのアクタクラス)
|   |        |
|   |        |── GroupAttackActor(集団戦用のアクタクラス)
|   |        |
|   |        |── ioJson(Jsonロード用の関数)
|   |        |
|   |        |── ioJsonText(JsonでTextデータを使用する際に変換等を行う関数)
|   |        |
|   |        |── ItemActor(アイテム類のアクタクラス)
|   |        |
|   |        |── LaserActor(サンダークロスの技、リフリジ・レイザーのアクタクラス)
|   |        |
|   |        |── LavaActor(溶岩のアクタクラス)
|   |        |
|   |        |── ModeGame(インゲームを処理するクラス)
|   |        |
|   |        |── ModeGameGoal(ゴール時の演出を行うクラス)
|   |        |
|   |        |── ModeGameOver(ゲームオーバー時の演出を行うクラス)
|   |        |
|   |        |── ModeGameUI(インゲームのUIを担当するクラス)
|   |        |
|   |        |── ModeLoading(ローディング時の演出を行うクラス)
|   |        |
|   |        |── ModeMenu(メニュー項目を作る用のヘッダークラス)
|   |        |
|   |        |── ModeOption(ポーズ画面を担当するクラス)
|   |        |
|   |        |── ModeScenario(シナリオ画面を担当するクラス)
|   |        |
|   |        |── ModeStageSelect(ステージセレクト画面を担当するクラス)
|   |        |
|   |        |── ModeStory(ストーリーを担当するクラス)
|   |        |
|   |        |── ModeTitle(タイトル画面を担当するクラス)
|   |        |
|   |        |── ModeTitleMenu(タイトル画面のボタンを担当するクラス)
|   |        |
|   |        |── OutlineComponent(キャラクターのアウトラインの制御・描画を行うクラス)
|   |        |
|   |        |── PlayerActor(プレイヤーの制御を行うアクタクラス)
|   |        |
|   |        |── PlayerCursorComponent(サンダークロス時のカーソルを制御・描画するクラス)
|   |        |
|   |        |── PlayerMoveCollisionComponent(プレイヤー用の衝突判定クラス)
|   |        |
|   |        |── PlayerMoveComponent(プレイヤーの移動を制御するクラス)
|   |        |
|   |        |── PunchActor(サンダークロスの技、レイゾー功のアクタクラス)
|   |        |
|   |        |── SlashActor(サンダークロスの技、Z対レイ刀のアクタクラス)
|   |        |
|   |        |── SnowComponent(積もる雪を制御するクラス)
|   |        |
|   |        |── StageBox(ステージのオブジェクトを制御するクラス)
|   |        |
|   |        |── TimerComponent(一定時間後に親アクタをdeleteするコンポーネント)
|   |        |
|   |        |── TreeActor(木の制御を行うアクタクラス)
|   |        |
|   |        |── UIBossHPClass(ボスHPの表示を行うクラス)
|   |        |
|   |        |── UIItemClass(アイテムのUIを表示するクラス)
|   |        |
|   |        |── UIPlayerHPClass(プレイヤーのHPUIを表示するクラス)
|   |        |
|   |        |── UISoundActor(UI等の入力に合わせて音を鳴らすクラス)
|   |        |
|   |        |── UITextActor(インゲームでのテキストを処理するアクタクラス)
|   |        |
|   |        └── UITextComponent(インゲームでのテキストを処理するコンポーネント)
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