# CVM 設定

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [CVM](../../index.md) > [メモリ管理](../index.md) > [CVM 設定](./index.md)

## 概要

## 設定一覧

|設定名|形式|内容|備考|
|:-:|:-:|:-:|:-:|
|heap-size-def|データサイズ|ヒープ領域の初期サイズ|heap-size-max 未満であること|
|heap-size-max|データサイズ|ヒープ領域の最大サイズ||
|heap--size-max|データサイズ|ヒープ領域の最大サイズ||
|mem-size-max|データサイズ|メモリ領域全体の最大サイズ|heap-size-max 及び stack-size-max をそれぞれ超えること|
|stack-size-def|データサイズ|各スレッドの Threst 領域の初期サイズ|stack-size-max 未満であること|
|stack-size-max|データサイズ|各スレッドの Threst 領域の最大サイズ||

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
