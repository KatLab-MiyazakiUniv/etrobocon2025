# etrobocon2025

宮崎大学片山徹郎研究室チーム KatLab が作成する[ET ロボコン 2025](https://www.etrobo.jp/)アドバンストクラスの走行システムプログラムです。

## 構成
### ./modules
ソースファイルを格納
### ./tests
テストソースを格納

## プロジェクトのビルド
### 実機の場合
```etrobocon2025```で以下を実行
```shell
make build
```
### 実機以外の場合
etrobo環境でビルドが出来ないため，現状 (20250502) ではこの方法を考えています
```etrobocon2025```で以下を順に実行
1. (まだの場合は)bookworm環境のイメージを以下のコマンドで作成
```shell
docker buildx build --platform linux/arm64 -t kat_etrobo2025:arm64 .
```
2. コンテナ作成と起動
```shell
docker run -it --rm -v $(pwd):/RasPike-ART/sdk/workspace/etrobocon2025 kat_etrobo2025:arm64 bash
```
もし，ファイル保存などの権限問題に引っかかったら，以下のコマンドでUIDとGIDを確認し，
```shell
id -u
```
```shell
id -g
```
その番号でそれぞれ，以下のコマンドのUIDとGIDを置き換えて実行してみてください
```shell
docker run -it --rm --user UID:GID -v $(pwd):/RasPike-ART/sdk/workspace/etrobocon2025 kat_etrobo2025:arm64 bash
```

3. プロジェクトのビルド
```shell
make build
```

## テスト
```etrobocon2025```で以下を実行
```shell
make test
```
もしくは，```etrobocon2025```で以下を実行
```shell
make smart-clean
make test-build
make test-exec
```