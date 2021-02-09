# CVM コマンド

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [コマンドライン](../index.md) > [CVM コマンド](./index.md)

## 概要

`CVM` コマンドではCVMコードを実行することができる。

## ファイルパス

### Windows

```
<HOME>\bin\cvm
```

### macOS

```
<HOME>/bin/cvm
```

## サブコマンド

### run

#### 引数と機能

|オプション名|オプション引数|機能|
|:-:|:-:|:-:|
|-i|入力ファイルパス|入力ファイルを指定する|

#### 代替コマンド

サブコマンド名 `run` を省略可。

`$ cvm run -i <入力ファイル>` → `$ cvm -i <入力ファイル> ...`

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
