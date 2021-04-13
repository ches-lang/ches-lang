# CVM コード

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [CVM](../index.md) > [CVM コード](./index.md)

## 構成部分

### ヘッダ

ヘッダ長は 128 バイトである。

|開始位置|サイズ ( バイト )|形式|内容|
|:-:|:-:|:-:|:-:|
|0|8|文字列|マジックナンバ|
|8|8|文字列|ビルドバージョンのコードネーム|
|16|4|Unix タイムスタンプ|最終変更日時|
|16|32|文字列|作成者|
|||||

#### Ches マジックナンバ

[マジックナンバ](./magnum/index.md) を参照。

#### Ches バージョン情報

### ID データ

#### 関数

|内容|サイズ ( バイト )|形式|
|:-:|:-:|:-:|
|関数 ID|16|UUID|
|命令開始位置|4|数値|
|関数名|可変|文字列|
|終端コード|1|バイナリ `0x00`|

#### グローバル変数

|内容|サイズ ( バイト )|形式|
|:-:|:-:|:-:|
|アドレス開始位置|4|数値|
|データの種類|1|数値|
|変数値|1|任意|

##### データの種類

|数値|種類|サイズ ( バイト )|形式|
|:-:|:-:|:-:|:-:|
|`0x01`|整数|1|数値|
|`0x02`|整数|2|数値|
|`0x03`|整数|4|数値|
|`0x04`|整数|8|数値|
|`0x05`|浮動小数点数|4|数値|
|`0x06`|浮動小数点数|8|数値|
|`0x00`|文字列|`0x00` までのサイズ|文字列|
|`0x00`|インスタンス|16|コンストラクタの関数 ID||

### 命令



## 例

### Hello

#### Ches コード

```
class Hello

void @entry(str[] args)
    if(args.len != 2)
        ret

    println("Hello, world!")
end
```

#### CVM コード

```
00| Hello.@entry:

01| push_arg    0
02| push_32     2
03| cmp_32

04| goto_if     2
05| ret

06| push_str    "Hello, world"
07| syscall     print

08| ret
```

### User

#### Ches コード

```
class HowAreYou

void @entry()
    var user = User("Garnet")
    user.greet("How are you?")
end
```

```
class User

pub name = ""

@const(str name)
    this.name = name
end

dyn void say(str msg)
    println(this.name ~ ": " ~ msg)
end
```

#### CVM コード

```
00| Hello.@entry(str[]):

0 | spush_str   "Garnet"
0 | spush_new   User(str)

0 | spush_str   "Hello, world"
0 | call        User.say(str)

0 | ret

0 | User.@const(str):

0 | spush_this
0 | spush_arg   0
0 | hset        name
0 | ret

0 | User.say(str):

0 | push_this
0 | push_mem    name

0 | push_str    ": "
0 | join_str

0 | push_arg    0
0 | join_str

0 | call        println(str)

0 | ret
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
