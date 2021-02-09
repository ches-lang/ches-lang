# 関数式

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [言語仕様書](../../index.md) > [関数](../index.md) > [関数式](./index.md)

## 概要

関数式は関数を式として表現する構文である。

関数式ではFuncインスタンスを取得できる。

Chestnutに無名関数は存在せず、関数名の記述が必須である。

## 構文

```
関数名()
    # 処理
end
```

## コード例

```
class Calculator


use sys


void @entry()
    runCalcFunc(calc(int x, int y)
        ret x + y
    end, 10)
end

void runCalcFunc(Func<int> func, int count)
    for count
        println(func(0, 1))
    end
end
```

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
