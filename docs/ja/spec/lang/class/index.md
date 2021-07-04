# クラス

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [クラス](./index.md)

## 構文

```
class クラス名

メンバ1
メンバ2

関数1
関数2
```

```
#== クラス定義 ==#

class Names

#== メンバ変数 ==#

let creator_name = "Garnet"
let pub name = "Micheal"

#== メンバ関数 ==#

fn dyn greet_creator()
    println("Hello, " ~ self.creator_name ~ "!")
end

fn dyn change_name(str new_name)
    self.name = new_name
end
```

## アクセス修飾子

[修飾子 > アクセス修飾子](../modifier/index.md#アクセス修飾子) を参照。

## メンバ

### アクセス修飾子

|修飾子|スコープ|
|:-:|:-:|
|public|全体|
|private|クラス内|

### 静的メンバ

## 継承

[継承](./inheritance/index.md) を参照。

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
