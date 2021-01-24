# 継承

[ドキュメント](../../../../../../../index.md) > [日本語](../../../../../../index.md) > [リリース](../../../../../index.md) > [Ches 0](../../../../index.md) > [言語仕様書](../../../index.md) > [言語仕様リンク](../../index.md) > [クラス](../index.md) > [継承](./index.md)

## 構文

```
class 継承先クラス名 : 継承元クラス名
```

### 使用例

```
class PCInterface


dyn vir bool destroyed = false

dyn vir void destroy()
end
```

```
class BeefedUpPC : PCInterface


dyn ovr bool destroyed = false

dyn ovr void destroy()
    println("Beefed-up PC never be broken!")
end
```

---

Copyright © 2020 Garnet3106 All rights reserved.
