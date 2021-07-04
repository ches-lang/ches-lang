# 属性

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [属性](./index.md)

## 概要

クラスやクラスメンバに対して任意の情報を付与できる。

## 構文

```
#== 属性の付与 ==#

@属性クラスインスタンス
メンバ定義


#== コード例 ==#

@Attr("Any Messages...")
メンバ定義
```

### 使用例

```
class Calculator


use sys


@ArgAttr("secs", "元の時間 (秒)")
@ArgAttr("meters", "元の長さ (メートル)")
@ExceptAttr(ArgExcept)
@RetAttr("結果の速さ (毎秒メートル)")
f32 getSecSpeed(f32 secs, f32 meters)
    if secs > 0 || meters > 0
        throw ArgExcept()
    end

    ret meters / hours
end
```
## 定義

属性を作成する際には `Attr` クラスを継承する。

このようなクラスを `属性クラス` という。

属性クラス名の最後には `Attr` を付けること。( 例: `BetaAttr` )

### 定義例

```
class BetaAttr : Attr


str msg = ""


@const()
end

@const(str msg)
    this.msg = msg
end
```

### 属性クラス一覧

標準ライブラリの属性クラスについては [Attr]() を参照。

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
