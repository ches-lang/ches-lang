# メモリ領域

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [CVM](../../index.md) > [メモリ管理](../index.md) > [メモリ領域](./index.md)

## 概要

## 領域一覧

### 全領域

- [共有領域 ( Shared Area )](./shared/index.md)
    - [命令領域 ( Instruction Area )](./shared/instruction/index.md)
    - [関数領域 ( Function Area )](./shared/function/index.md#関数領域)
    - [クラス領域 ( Class Area )](./shared/class/index.md#クラス領域)
    - [参照領域 ( Reference Area )](./shared/reference/index.md#参照領域)
    - [定数領域 ( Constant Variable Area )](./shared/constvar/index.md#定数領域)
    - [GC 領域 ( GC Area )](./shared/gc/index.md)
        - [新規 GC 領域 ( New GC Area )](./shared/gc/new/index.md)
            - [第一新規 GC 領域 ( First New GC Area )](./shared/gc/new/first/index.md)
            - [第二新規 GC 領域 ( Second New GC Area)](./shared/gc/new/second/index.md)
        - [長期 GC 領域 ( Longterm GC Area )](./shared/gc/longterm/index.md)
- [スレッド領域 ( Thread Area )](./thread/index.md)
    - [関数領域 ( Function Area )](./thread/function/index.md)
    - [演算領域 ( Operation Area )](./thread/operation/index.md)

### 保有領域

|領域名|役割|
|:-:|:-:|
|[共有領域<br>( Shared Area )](./shared/index.md)|全スレッドに共有される領域|
|[スレッド領域<br>( Thread Area )](./thread/index.md)|各スレッド毎に割り当てられる領域|

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
