# if 文

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [if 文](./index.md)

## 概要

条件分岐を行う。

## 構文

```
#== これらの場合 if 文を用いることは無意味である ==#

if true
    println("always be called")
end

if false
    println("never be called")
end

#== 条件を指定する ==#

let s = "Hello"

if s.len() == 5
    println("'" ~ s ~ "' is five characters.")
end
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
