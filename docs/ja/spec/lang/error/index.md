# エラー

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [エラー](./index.md)

## 概要

エラー処理では関数内の処理における実行時の失敗を解決する。

エラーを発生させることを送出 ( throw ) 、エラー処理をすることを捕捉 ( catch ) という。

## 構文

```
#== エラー送出 ==#

throw エラークラス(引数)

#== エラー捕捉 ==#

catch 関数名(引数)
    err(エラーの変数)
        # 失敗時の処理
    succ(戻り値の変数)
        # 成功時の処理
    end
end
```

```
class Main

#== エラーを捕捉する ==#

fn @entry()
    let result_num = catch add_natural_num()
        err(e)
            println("A number is invalid.")
            ret
        succ(v)
            break e
        end
    end
end

#== エラーを送出する関数 ==#

fn f32_to_s32(f32 num)
    if !num.is_int()
        throw Err()
    end

    ret num as s32
end
```

```
#== エラークラス定義 ==#

# Err クラスを継承する
class NumErr : Err

use std

let pub msg

fn pub @const(str msg) self
    self.msg = msg
end

fn pub get_msg() str
    ret self.msg
end
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
