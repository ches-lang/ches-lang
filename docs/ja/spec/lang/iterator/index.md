# イテレータ

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [イテレータ](./index.md)

## 概要

集合中の特定の要素を示す。

## 構文

```
let vec = Vec<str>["a", "b", "c"]

let end_iter = vec.end_iter()

for iter_i = vec.begin_iter(); iter_i.value() != end_iter; item = iter_i.next()
    println(iter_i.value())
end
```

## 種類

|名称|英名|説明|
|:-:|:-:|:-:|
|ランダムアクセスイテレータ|random access iterator|ランダムアクセス; 自由な位置を参照できる|
|双方向イテレータ|bidirectional iterator|シーケンシャルアクセス; 前後に移動できる|
|前方向イテレータ|forward iterator|シーケンシャルアクセス; 前に移動できる|

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
