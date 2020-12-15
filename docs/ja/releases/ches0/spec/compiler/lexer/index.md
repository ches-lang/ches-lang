# レクサ (字句解析器)

[ドキュメント](../../../../../../index.md) > [日本語](../../../../../index.md) > [リリース](../../../../index.md) > [Ches 0](../../../index.md) > [言語仕様書](../../index.md) > [コンパイラ](../index.md) > [レクサ](./index.md)

## 役割

レクサは構文解析のためにChesコードをトークンごとに分割する。

コンパイルの流れについては [コンパイラ > コンパイルの流れ](../index.md) を参照。

## トークンの種類一覧

|名称|内容|記号|
|:-:|:-:|:-:|
|Unknown|不明||
|EOF|ファイルの終了||
|NewLine|改行||
|Comment|コメントアウト||
|Key|キーワード||
|ID|識別子||
|Num|数値||
|Char|文字||
|String|文字列||
|Exclam|感嘆符|!|
|Question|疑問符|?|
|Tilde|チルダ|\~|
|Plus|プラス|\+|
|Hyphen|ハイフン|\-|
|Asterisk|アスタリスク|\*|
|Slash|スラッシュ|/|
|Percent|パーセント記号 |%|
|Caret|キャレット|\^|
|Equal|イコール|\=|
|Pipe|パイプ|\||
|Amper|アンパサンド|\&|
|Period|ピリオド|\.|
|Comma|コンマ|,|
|Colon|コロン|\:|
|Semicolon|セミコロン|;|
|Paren|括弧|( ) [ ] { } < >|

## 解析例

### Chesコード

```
println("Hello, world!")
```

### 解析結果

|種類|トークン|
|:-:|:-:|
|識別子|println|
|記号|(|
|文字列|"Hello, world!"|
|記号|)|

---

Copyright © 2020 Garnet3106 All rights reserved.
