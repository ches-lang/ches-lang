# タプル

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [言語仕様書](../../index.md) > [データ構造](../index.md) > [タプル](./index.md)

## 概要

異なる型の値を複数個格納できる。

## 構文

```
#== タプル型変数の定義 ==#

# 型 ... (s32, str, Vec<f64>)
let tuple = (100s32, "Hello", Vec<f64>())

#== タプルの展開 ==#

# 複数の変数に展開する
let (v1, v2, v3) = tuple

# 値の数が合わないためコンパイルエラー
let (v1, v2) = tuple
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
