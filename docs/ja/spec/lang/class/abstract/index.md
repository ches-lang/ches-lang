# 抽象クラス

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [言語仕様書](../../index.md) > [クラス](../index.md) > [抽象クラス](./index.md)

## 概要

継承先のクラスに対して関数の実装を強制することができる。

他言語でいうインタフェイスは存在せず、抽象クラスを用いることになる。

## 構文

```
## 抽象クラスの定義 ##

class abs クラス名
```

```
## 抽象関数の定義 ##

# 行の最後に end キーワードを置く
fn abs 関数名(引数) end
```

```
class abs Car

# 抽象関数を定義する
fn abs run() end

# 通常の関数も定義できる
fn stop()
    self.speed = 0
end
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
