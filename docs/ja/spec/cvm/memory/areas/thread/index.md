# スレッド領域

[ドキュメント](../../../../../../index.md) > [日本語](../../../../../index.md) > [仕様書](../../../../index.md) > [CVM](../../../index.md) > [メモリ管理](../../index.md) > [メモリ領域](../index.md) > [スレッド領域](./index.md)

## 概要

各スレッド毎に 1 つ割り当てられる。

## 保有領域

|領域名|役割|
|:-:|:-:|
|[関数領域<br>( Thread Area )](./function/index.md)|実行中の関数に関するデータをスタック構造で格納する|
|[演算領域<br>( Operation Area )](./reference/index.md)|関数内で使用する一時的な参照データをスタック構造で格納する|

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
