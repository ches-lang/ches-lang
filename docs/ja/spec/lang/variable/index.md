# 変数

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [変数](./index.md)

## 構文

```
#== 初期化 ==#

let size = 128s32
let pi = 3.14f32
let capitalA = 'A'
let abc = "ABC"
let obj = Class()

#== 代入 ==#

size = 128s32
pi = 3.14f32
capitalA = 'A'
abc = "ABC"
obj = Class()
```

変数の定義には初期値が必須である。

## 組込型

[変数型 > 組込型](./type/index.md#組込型) を参照。

## 修飾子

[修飾子](../modifier/index.md) を参照。

### 使用可能な修飾子

|修飾子|ローカル変数|メンバ変数|
|:-:|:-:|:-:|
|`pub`|不可|可|
|`mut`|可|可|

## 可変と不変

可変 ( mutable ) と不変 ( immutable ) な変数がある。

変数はデフォルトで不変であり、その場合値を変更することができない。

変数を可変にするには `mut` 修飾子を付与する必要がある。

```
#== 不変な変数 ==#

let mut immut_arg = true

# 不変な変数を変更するとエラー
immut_arg = false

#== 可変な変数 ==#

let mut mut_arg = true

# 可変であるため変更できる
mut_arg = false
```

### 相互変換

可変な変数を不変に変換することはできるが、その逆は不可能である。

```
fn @entry
    let immut_arg = false
    let mut mut_arg = false

    # 第一引数で不変 -> 可変の変換が起こるためエラー
    pass_args(immut_arg, mut_arg)

    # 代入の場合、コピー値が代入されるため変換が起こらず問題ない
    immut_arg = mut_arg
end

fn pass_args(bool mut mut_arg, bool immut_arg)
end
```

## コピーと参照

変数は場合によってコピーまたは参照が作成される。

### スコープ外れ

スコープを外れると変数が破棄される。

```
fn func_scope
    let var = 0s32

    for i in 0:5
        # 定義されたスコープ内 ... ここでは使える
        println(var)
    end

    # 定義されたスコープ内 ... ここでは使える
end

# 定義されたスコープ外 ... ここでは使えない
```

### 引数渡し

引数として渡す場合、変数の参照が渡されることになる。

引数の変数が不変である場合は参照やデータを変更できないため安全である。

```
fn @entry
    let arg = 0s32
    let mut mut_arg = 0s32

    # ここで arg, mut_arg の参照が渡される
    pass_arg(arg, mut_arg)
end

fn pass_arg(int arg, int mut mut_arg)
    # arg は不変であるため参照先の値が変更できない
    # 値にアクセスすることはできる
    let added_arg = arg + 1

    # mut_arg は可変であるため参照先の値が変更できる
    mut_arg = 0s32
end

```

### 変数代入

変数を変数に代入する場合、元変数のコピー値が代入されることになる。

## スコープ

|名称|スコープ|用途|
|:-:|:-:|:-:|
|ローカル変数|関数内|関数内の処理|
|メンバ変数|アクセス修飾子による|クラスの管理|

## オーバーフロー

オーバーフローは主に以下の場合において発生する。

- 代入値 / 演算値のサイズが代入元の変数の範囲を超えた場合
- 型変換元の値のサイズが変換先の値の範囲を超えた場合

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
