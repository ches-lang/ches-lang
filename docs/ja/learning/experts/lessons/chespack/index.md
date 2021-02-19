# エキスパート - ChesPack

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [学習](../../../index.md) > [エキスパート](../../index.md) > [レッスン](../index.md) > [ChesPack](./index.md)

## 概要

## パッケージ

Chestnut のパッケージはアプリケーションやライブラリごとのファイル群です。ChesPack でプログラムを公開する場合はパッケージ単位で管理することになります。

## コマンド操作

### パッケージ作成

`$ chesp create <ProjectName>`

### パッケージ実行

`$ chesp run`

## ディレクトリ構造

```
root/
  ├ .git/
  ├ bin/
  ├ lib/
  ├ prj/
  ├ src/
  │ └ Main.ches
  └ .gitignore
```

### .git/

Git 環境がある場合はプロジェクト作成時に Git リポジトリが作成されます。

### bin/

コンパイル済みファイル等のバイナリファイルを格納します。

### lib/

パッケージに必要なデータファイルや外部パッケージのソースコードを格納します。

### src/

パッケージのソースコードは基本的に `src/` 内に記述されます。

#### Main.ches

初期状態は `Hello, world!` を出力するテストコードです。

### .gitignore

初期状態で追加されるパスは以下の通りです。

- `bin/`

---

[前のレッスン](../environment/index.md) | [次のレッスン](..//index.md) | [レッスン一覧](../index.md)

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
