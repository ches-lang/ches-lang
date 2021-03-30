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

### コード例

```
## クラス定義 ##

class Names


## メンバ変数 ##

str creatorName = "Garnet"
pub dyn str name = "Micheal"


## メンバ関数 ##

greetCreator()
    println("Hello, " ~ creatorName ~ "!")
end

dyn changeName(str newName)
    .name = newName
end
```

## アクセス修飾子

デフォルトの修飾子は `pub` である。

publicなメンバにアクセス修飾子を記述する必要はない。

|修飾子|アクセス範囲|
|:-:|:-:|
|pub|全体|
|prv|クラス内 [\*1]|

[\*1] 継承先クラスからのアクセスは不可。

## メンバ

### アクセス修飾子

|修飾子|スコープ|
|:-:|:-:|
|public|全体|
|private|クラス内|

### 静的メンバ

メンバ名の前に `.` (ピリオド) を置いて表す

(例: `int .num = 10`)

## 継承

[継承](./inheritance/index.md) を参照。

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
