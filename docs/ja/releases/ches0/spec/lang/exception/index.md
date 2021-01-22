# 例外

[ドキュメント](../../../../../../index.md) > [日本語](../../../../../index.md) > [リリース](../../../../index.md) > [Ches 0](../../../index.md) > [言語仕様書](../../index.md) > [言語仕様リンク](../index.md) > [例外](./index.md)

## コード例

```
# 例外クラス定義

class NameExcept


str .msg = ""

@NameExcept()
end

@NameExcept(str msg)
    .msg = msg
end
```

```
# 例外処理
void @entry()
    printYourName("Garnet")
        success
            println("Your name has printed on your console!")
        end
        fail(NameExcept except)
            println("It looks like the program has failed...")
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
