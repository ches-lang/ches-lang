# クラス

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [CVM](../../index.md) > [処理手順](../index.md) > [クラス](./index.md)

## 概要

## クラスロード

アプリケーションの初期化時に、固定データ領域のクラス領域にクラスデータを格納する。

[固定データ領域 > クラス領域](../../memory/areas/shared/heap/constant/index.md#クラス領域) を参照。

## インスタンス化

インデックスデータに参照アドレスを追加する。

ヒープ領域の新規データ領域にインスタンスデータの領域を確保する。

[新規データ領域](../../memory/areas/shared/heap/new/index.md) を参照。

## インスタンス破棄

インデックスデータの値を `0xFFFFFFFF` に置き換える。

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
