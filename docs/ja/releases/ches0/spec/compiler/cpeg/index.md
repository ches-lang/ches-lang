# CPEG (Chestnut-PEG)

[ドキュメント](../../../../../../index.md) > [日本語](../../../../../index.md) > [リリース](../../../../index.md) > [Ches 0](../../../index.md) > [言語仕様書](../../index.md) > [コンパイラ](../index.md) > [CPEG](./index.md)

## 用途

Chestnutコンパイラの構文定義に使用する。

## 構文

```
構文名 := 値1 値2 ...

# 行コメント
## ブロックコメント ##
```

### 構文一覧

|構文|名称|説明|例|
|:-:|:-:|:-:|:-:|
|"string"|文字列リテラル|文字列|`"class"`|
|[abc0-9]|文字クラス|括弧内のいずれかの1文字|`[a-zA-Z_]`|
|.|ワイルドカード|任意の1文字|`.*`|
|expr1 expr2|連接|成功すれば1つ右に移って検査|`numsign`|
|expr1 > expr2|選択|失敗すれば1つ右に移って再検査|`"class" id > "global"`|
|expr\*|0回以上の繰り返し|0回以上の直前要素の繰り返し|`""*`|
|expr+|1回以上の繰り返し|1回以上の直前要素の繰り返し|`IF+`|
|expr?|0回または1回|直前文字の1回以上の繰り返し|`IF+`|
|&expr1 expr2|肯定先読み|肯定先読みを行う|`&.* "\n"`|
|!expr1 expr2|否定先読み|否定先読みを行う|`!.* "\n"`|

#### 使用例

```
numsign := "#"

# line_comment として解釈されないよう block_comment を先に置く
block_comment := numsign numsign !.* numsign numsign

line_comment := numsign !.* new_line

# note: 解析時は生の状態に展開する？
```

## 命名規則

構文名は `snake_case` で記述する。

---

Copyright © 2020 Garnet3106 All rights reserved.
