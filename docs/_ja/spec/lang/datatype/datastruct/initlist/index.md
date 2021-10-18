# 初期化子リスト

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [言語仕様書](../../index.md) > [データ構造](../index.md) > [初期化子リスト](./index.md)

## 概要

データクラスの初期化のためにデータ列を表現する。

## 構文

```
#== インデックスを指定しない ==#

型[値1, 値2...]

#== インデックスを指定する ==#

型[キー1 = 値1, キー2 = 値2...]
```

```
let arr = Arr<char>['A', 'B', 'C']
let tuple = Tuple['A', 'B', 'C']
let vec = Vec<char>['A', 'B', 'C']

let map = Map<char, str>['A' = "Alpha", 'B' = "Beta", 'C' = "Charlie"]
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
