# プロパティ

[ドキュメント](../../../../../../../index.md) > [日本語](../../../../../../index.md) > [リリース](../../../../../index.md) > [Ches 0](../../../../index.md) > [言語仕様書](../../../index.md) > [言語仕様リンク](../../index.md) > [クラス](../index.md) > [プロパティ](./index.md)

## 概要

メンバ変数の値を柔軟に読み書きすることができる。

## 構文

```
prop 型 変数名
    get
        # getter
    set
        # setter
    end
end
```

### 使用例

```
class TV


use sys


prop bool power = false
    set
        if power == val
            println("Nothing happened.")
            ret
        end

        this = val
    end
end


@const()
end
```

## アクセサ

プロパティのアクセスに用いる処理を `アクセサ` と呼ぶ。

### ゲッタ

`ゲッタ` は値の取得を行うアクセサである。

### セッタ

`セッタ` は値の設定を行うアクセサである。

---

Copyright © 2020 Garnet3106 All rights reserved.
