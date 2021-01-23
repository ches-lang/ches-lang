# 例外

[ドキュメント](../../../../../../index.md) > [日本語](../../../../../index.md) > [リリース](../../../../index.md) > [Ches 0](../../../index.md) > [言語仕様書](../../index.md) > [言語仕様リンク](../index.md) > [例外](./index.md)

## 構文

```
関数名(引数) ? 失敗時の処理, 成功時の処理

# - 失敗時の処理 ... Func<void>(Except)
# - 成功時の処理 ... Func<void>()

# 改行する例
関数名(引数) ?
    fail(Except except)
        # 失敗時の処理
    end, success()
        # 成功時の処理
    end
```

## コード例

```
# 例外クラス定義

class NameExcept


str .msg = ""

@const()
end

@const(str msg)
    .msg = msg
end
```

```
class NamePrinter


void @entry()
    str inputName = readln()

    # 例外処理
    printYourName(inputName) ?
        fail(NameExcept except)
            println("It looks like the program has failed...")
        end, success()
            println("Your name has printed on your console!")
        end
end


# 対象関数
void printYourName(str yourName) ? NameExcept
    if(yourName == "Garnet")
        # 例外を投げる
        throw Except("It's just my name!")
    end

    println(yourName)
end
```

---

Copyright © 2020 Garnet3106 All rights reserved.
