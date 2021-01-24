# return

[ドキュメント](../../../../../../../index.md) > [日本語](../../../../../../index.md) > [リリース](../../../../../index.md) > [Ches 0](../../../../index.md) > [言語仕様書](../../../index.md) > [言語仕様リンク](../../index.md) > [関数](../index.md) > [return](./index.md)

## 概要

## 構文

```
ret
ret 返り値
```

## コード例

```
class Hello


void @entry()
    println(getGreetingMsg())
end

str getGreetingMsg()
    # str値を返す
    ret "Hello!"
end
```

---

Copyright © 2020 Garnet3106 All rights reserved.
