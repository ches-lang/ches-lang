# 命名規則

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [命名規則](./index.md)

## 概要

要素への命名では命名規則に従っている必要がある。

## 基本規則

以下の規則に反する命名はコンパイル時に警告を発生させる。

|要素|規則|例|
|:-:|:-:|:-:|
|プロジェクト|snake_case|std|
|モジュール|snake_case|file|
|クラス|PascalCase|FileErr|
|列挙体|PascalCase|FileErrKind|
|関数|snake_case|read_lines|
|変数|snake_case|rel_file_path|

## その他規則

### 予約語の不使用

命名に予約語を用いることはできない。

これに反する命名はコンパイル時にエラーを発生させることがある。

(誤例) `pub` `class` `fn` `let` `mut`

### 単語の省略

単語は可能な限り省略する。

以下はその例である。

|元単語|変換先|意味|
|:-:|:-:|:-:|
|`absolute`|`abs`|絶対の|
|`abstract`|`abs`|抽象的な|
|`directory`|`dir`|ディレクトリ|
|`error`|`err`|エラー|
|`length`|`len`|長さ|
|`original`|`orig`|原物の|
|`relative`|`rel`|相対的な|

### 番号割り当て

識別子に番号を割り振る際は `名前_番号` とする。

(例1) `var_1` `var_2` `var_3`

(例2) `MyClass_1` `MyClass_2` `MyClass_3`

### 関数

変数の型や状態を変換する関数は `to_` で始まる。

(例) `to_s32` `to_abs_path`

### 変数
