# 関数

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [関数](./index.md)

## 概要

機能をひとまとまりに区分する。

## 構文

```
#== 関数定義 ==#

# 修飾子と戻り値は省略できる
fn 修飾子 関数名(引数) 戻り値
    処理
end


#== 関数呼び出し ==#

関数名(引数)
```

```
# 関数定義
fn rev_bool_val(bool val) bool
    # 関数呼び出し
    println(val)

    ret !val
end
```

## 修飾子

[修飾子](../modifier/index.md) を参照。

### 使用可能な修飾子

|修飾子|
|:-:|
|`pub`|
|`dyn`|

## 値返し

関数からその呼び出し元へ値を送る。

関数内で返した値を返り値という。

### 構文

```
# 返り値なし
ret

# 返り値あり
ret 返り値
```

```
fn rev_bool_val(bool val) bool
    # 値を返す
    ret !val
end
```

### 多値返却

[タプル](../tuple/index.md) を用いると複数の異なった型を持つ値を返すことができる。

## 種類

### 組込関数

[組込関数](./builtin/index.md) を参照。

元から言語に組み込まれている関数。

言語の基礎となる機能を持つ。

(例) `println` `readln`

### ユーザ定義関数

ユーザが自由に定義する関数。

#### 特別関数

[特別関数](./special/index.md) を参照。

特殊な用途に用いる関数で、関数名が `@` から始まる。

(例) `@entry` `@const` `@dest`

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
