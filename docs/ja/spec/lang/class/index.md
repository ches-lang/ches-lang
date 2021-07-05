# クラス

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [クラス](./index.md)

## 構文

`class` 構文はブロックでないため、インデントや `end` キーワードが必要ない。

```
# 修飾子は省略可
# クラス名はモジュール名を含む ... モジュール名.クラス名 など
class 修飾子 クラス名

メンバ1
メンバ2

関数1
関数2
```

```
#== クラス定義 ==#

class Example.Names

#== メンバ変数 ==#

let creator_name = "Garnet"
let pub mut name = "Micheal"

#== メンバ関数 ==#

fn self greet_creator()
    println("Hello, " ~ self.creator_name ~ "!")
end

fn self change_name(str new_name)
    self.name = new_name
end
```

## アクセス修飾子

[修飾子 > アクセス修飾子](../modifier/index.md#アクセス修飾子) を参照。

## メンバ

### 静的メンバ

## 継承

[継承](./inheritance/index.md) を参照。

## 抽象クラス

[抽象クラス](./abstract/index.md) を参照。

## 構造体

構造体の機能は存在しないため、クラスで代用する。

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
