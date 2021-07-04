# データ構造

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [データ構造](./index.md)

## 概要

## 一覧

標準ライブラリ内に用意されたデータ構造の種類を列挙する。

|名称|英名|型|要素数|インデックスの型|
|:-:|:-:|:-:|:-:|:-:|
|[配列](./array/index.md)|array|[std::Arr\<T>](nolink)|固定|u32|
|[タプル](./tuple/index.md)|tuple|[(T1, T2...)](nolink)|固定|u32|
|ベクタ|vector|[std::Vec\<T>](nolink)|可変|u32|
|リスト|list|[std::List\<T>](nolink)|可変|u32|
|マップ|map|[std::Map\<T1, T2>](nolink)|可変|T1|
|キュー|queue|[std::Queue\<T>](nolink)|可変|-|
|スタック|stack|[std::Stack\<T>](nolink)|可変|-|

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
