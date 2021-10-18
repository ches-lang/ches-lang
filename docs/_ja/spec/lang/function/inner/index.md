# 内部関数

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [言語仕様書](../../index.md) > [関数](../index.md) > [内部関数](./index.md)

## 概要

関数の中に関数を定義する。

`let-fn` 構文により [Func](nolink) 型の変数を定義できる。

定義された関数の外では利用できない。

## 構文

```
#== 関数定義 ==#

let fn 関数名(引数)
    # 処理
end
```

```
# Func<str> 型
let fn func(str name) bool
    ret 0s32
end

func()
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
