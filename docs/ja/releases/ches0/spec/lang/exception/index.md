# 例外

[ドキュメント](../../../../../../index.md) > [日本語](../../../../../index.md) > [リリース](../../../../index.md) > [Ches 0](../../../index.md) > [言語仕様書](../../index.md) > [言語仕様リンク](../index.md) > [例外](./index.md)

## 概要

例外は関数の処理におけるエラーである。

例外を発生させることをスロー、例外の処理をすることをキャッチという。

## 構文

```
catch 関数名(引数)
    # 失敗時の処理
succ
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

    catch printYourName(inputName) ? NameExcept except
        println("It looks like the program has failed...")
    succ
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
