# return

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [言語仕様書](../../index.md) > [関数](../index.md) > [return](./index.md)

## 概要

## 構文

```
# 戻り値なし (void)
ret

# 戻り値あり
ret 返り値
```

## コード例

```
class Hello

use sys

void @entry()
    println(getGreetingMsg())
end

str getGreetingMsg()
    # str値を返す
    ret "Hello!"
end
```

## 多値返却

多値返却には [タプル](../../tuple/index.md) を用いる。

```
Main

void @entry()
    (str name, int birthYear) profile = Main.getMyProfile()
    println(profile.name ~ ": " ~ profile.age.toStr())
end

(str name, int birthYear) getMyProfile()
    ret { "Garnet", 2004 }
end
```

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
