# CPEG (Chestnut-PEG)

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [コンパイラ](../index.md) > [CPEG](./index.md)

## 用途

Chestnutコンパイラの構文定義に使用する。

## 構文

規則名は `非終端記号` 、規則名を除く右辺の各規則は `終端記号` である。

```
規則名 := 規則

# 行コメント
## ブロックコメント ##
```

### 構文一覧

|構文|名称|説明|例|
|:-:|:-:|:-:|:-:|
|"string"|文字列リテラル<br>( string literal )|文字列|`"class"`|
|[abc0-9]|文字クラス<br>( character class )|括弧内のいずれかの1文字|`[a-zA-Z_]`|
|.|ワイルドカード<br>( wild card )|任意の1文字|`.*`|
|e~1~ e~2~|連接<br>( sequence )|成功すれば1つ右に移って検査|`class id`|
|(e~1~ e~2~)|連接グループ<br>( sequence group )|連接をグループ化する|`(class id)?`|
|e~1~ > e~2~|選択<br>( ordered choice )|失敗すれば1つ右に移って再検査|`"class" id > "global"`|
|e\*|0回以上<br>( zero-or-more )|0回以上の直前の繰り返し|`""*`|
|e+|1回以上<br>( one-or-more )|1回以上の直前の繰り返し|`IF+`|
|e?|0回または1回<br>( zero-or-one )|直前の1回以上の繰り返し|`IF+`|
|&e~1~ e~2~|肯定先読み<br>( positive-lookbehind )|成功しても入力を進めない|`&"\n" "\r"`|
|!e~1~ e~2~|否定先読み<br>( negative-lookbehind ) |成功しても入力を進めない|`!"\n" .`|
|N :=|定義<br>( definition )|右辺の規則を規則名として定義|`class := "class" id`|
|N~1~:N~2~|再命名<br>( renaming )|規則名を上書き|`class id:class_name`|

#### 使用例

```
numsign := "#"

# line_comment として解釈されないよう block_comment を先に置く
block_comment := numsign numsign !.* numsign numsign

line_comment := numsign !.* new_line
```

## 命名規則

規則名は `snake_case` で記述する。

## 出力データ

CPEGで解析した結果はASTとして出力される。

[AST (抽象構文木)](../ast/index.md) を参照。

```
## ソース ##

hoge("String", 123)

## CPEGコード ##

spacing := " " > "\t"

char := "\\" . > !"\"" .
string := "\"" char* "\""

func_call := id:func_name spacing* "(" spacing* func_call_args spacing* ")"
func_call_args := value? > (value ",")* value

id := [a-Z_] [0-9a-Z_]*

## 出力データ ##

func_call
    |- func_name
        |- hoge
    |- args
        |- string
            |- "
            |- Hello, world!
            |- "
        |- number
            |- 123
```

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
