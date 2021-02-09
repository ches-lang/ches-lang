# 変数

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [変数](./index.md)

## 構文

```
# 定義
<型名> <変数名>

# 初期化
<型名> <変数名> = <初期化値>
```

### コード例

```
## 定義 ##

int number
dec decimal
chr character
str string
Class object


## 初期化 ##

int number = 128
dec decimal = 3.14
chr character = 'A'
str string = "ABC"
Class object = new()  // *
```

\* new() については [オブジェクト](./object.md) を参照。

## 変数型

[変数型](./type/index.md) を参照。

## アクセス修飾子

[クラス > メンバ > アクセス修飾子](../class/index.md) を参照。

## 変数スコープ

|名称|スコープ|用途|
|:-:|:-:|:-:|
|グローバル変数|全体|複数クラス間との変数共有|
|メンバ変数|クラス内 \*|クラスの管理|
|ローカル変数|関数内|関数内の処理|

\* アクセス修飾子により変更可

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
