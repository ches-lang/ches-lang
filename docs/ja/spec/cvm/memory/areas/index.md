# メモリ領域

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [CVM](../../index.md) > [メモリ管理](../index.md) > [メモリ領域](./index.md)

## 概要

## 領域一覧

|領域名|役割|
|:-:|:-:|
|[共有領域<br>( shared area )](./shared/index.md)|全スレッドに共有される領域|
|[スレッド領域<br>( thread area )](./thread/index.md)|各スレッド毎に割り当てられる領域|

- [共有領域 ( Shared Area )](./shared/index.md)
    - [CVM コード領域 ( CVM Code Area )](./shared/cvmcode/index.md)
    - [ヒープ領域 ( Heap Area )](./shared/heap/index.md)
        - [固定データ領域 ( Constant Data Area )](./shared/heap/constant/index.md)
            - [関数領域 ( Function Area )](./shared/heap/constant/index.md#関数領域)
            - [クラス領域 ( Class Area )](./shared/heap/constant/index.md#クラス領域)
            - [参照スタック領域 ( Reference Stack Area )](./shared/heap/constant/index.md#参照スタック領域)
            - [定数領域 ( Constant Variable Area )](./shared/heap/constant/index.md#定数領域)
        - [新規データ領域 ( New Data Area )](./shared/heap/new/index.md)
            - [第一新規データ領域 ( First New Data Area )](./shared/heap/new/index.md)
            - [第二新規データ領域 ( Second New Data Area)](./shared/heap/new/index.md)
        - [長期データ領域 ( Longterm Data Area )](./shared/heap/longterm/index.md)
- [スレッド領域 ( Thread Area )](./thread/index.md)
    - [関数スタック領域 ( Function Stack Area )](./thread/function/index.md)
    - [演算スタック領域 ( Operation Stack Area )](./thread/operation/index.md)

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
