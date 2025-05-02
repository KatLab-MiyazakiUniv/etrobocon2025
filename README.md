# etrobocon2025

宮崎大学片山徹郎研究室チーム KatLab が作成する[ET ロボコン 2025](https://www.etrobo.jp/)アドバンストクラスの走行システムプログラムです。

## プロジェクトのビルド
### 実機の場合
```etrobocon2025```で以下を実行
```shell
make build
```
### 実機以外の場合
```etrobocon2025```で以下を順に実行
1. (まだの場合は)bookworm環境のイメージを以下のコマンドで作成
```shell
docker buildx build --platform linux/arm64 -t myimage:arm64 .
```
2. コンテナ作成と起動
```shell
docker run -it --rm -v $(pwd):/RasPike-ART/sdk/workspace/etrobocon2025 myimage:arm64 bash
```
3. プロジェクトのビルド
```shell
make build
```