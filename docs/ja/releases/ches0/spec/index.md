# 言語仕様書 (Ches 0)

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [リリース](../../index.md) > [Ches 0](../index.md) > [言語仕様書](./index.md)

## 概要

Ches0は、Chestnutで最初のリリースである。

この仕様書ではChes0の全般的な仕様を簡潔に記述する。

## 使用用途

Chestnutを使用する用途は問わない。

開発規模や分野に関わらず、広く汎用的に利用できる言語を目指す。

## 方針

- コーディング効率の向上
    - 書きやすく、読みやすい
- 大規模開発への対応
    - オブジェクト指向＆クラスベース
    - コーディングスタイルの統一
- 言語開発へのユーザ参加
    - リポジトリのOSS化
    - 公式コミュニティの運営

## パラダイム

- 構造化
- 命令型
- 手続き型
- オブジェクト指向
- イベント駆動型

## 他言語の影響

主に C#, Java から影響を受ける。

## 各ページ

- [言語](./lang/index.md)

    - [型変換 (キャスト)](./lang/cast/index.md)
    - [クラス](./lang/class/index.md)
    - [関数](./lang/function/index.md)
    - [数値](./lang/number/index.md)
    - [変数](./lang/variable/index.md)

- [コンパイラ](./compiler/index.md)

    - [抽象構文木 (AST)](./compiler/ast/index.md)

- [仮想マシン (CVM)](./cvm/index.md)

    - [バイトコード](./cvm/bytecode/index.md)
    - [命令](./cvm/instruction/index.md)

---

Copyright © 2020 Garnet3106 All rights reserved.
