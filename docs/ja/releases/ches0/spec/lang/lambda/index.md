# ラムダ式

[ドキュメント](../../../../../../index.md) > [日本語](../../../../../index.md) > [リリース](../../../../index.md) > [Ches 0](../../../index.md) > [言語仕様書](../../index.md) > [言語仕様リンク](../index.md) > [ラムダ式](./index.md)

## コード例

```
void @entry()
    runCalcFunc([int](int x, int y)
        ret x + y
    end, 10)
end

void runCalcFunc(Func func, int count)
    for count
        println(func(0, 1))
    end
end
```

---

Copyright © 2020 Garnet3106 All rights reserved.
