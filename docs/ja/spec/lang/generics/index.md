# ジェネリクス

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [ジェネリクス](./index.md)

## 概要

型を限定しない汎用的な処理ができる。

クラス及び関数に対して付与できる。

## 構文

```
class Main<Type>

let obj = null

@const(Type obj)
    self.obj = obj
end

printObj()
    println(self.obj.toStr())
end
```

```
class Main

printObj<Type>(Type obj)
    println(obj.toStr())
end
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
