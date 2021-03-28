# コード例

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [CVM](../../index.md) > [CVM コード](../index.md) > [コード例](./index.md)

## 概要

## 変数

### スタックプッシュ

```
00| spush_i32   123     ; 32 ビットの数値をプッシュ
01| spush_arg   0       ; 第一引数をプッシュ
02| spush_arr   0       ; スタックトップの配列の要素 0 をプッシュ
03| 
```

## 配列操作

### 領域確保

```
00| spush_32    10      ; 要素数をプッシュ
01| aalc_32             ; 要素数分の領域を確保
```

### 要素設定

```
00| sapush_32   123     ; 追加する値をプッシュ
01| spush_32    0       ; インデックスをプッシュ
02| aset                ; 値を設定
```

### 要素取得

```
00| spush_32    0       ; インデックスをプッシュ
01| sapush              ; 指定されたインデックスにある要素をプッシュ
```

### サイズ変更

```
00| spush_32    5       ; 要素数をプッシュ
01| ares_32             ; 要素数分の領域にリサイズ
```

## 関数呼び出し

### 静的関数

```
00| spush_str   "Hello, world!"         ; 第一引数をプッシュ
01| spush_str   "Hello, world!"         ; 第一引数をプッシュ
02| sttcall     Example.join(str, str)  ; 関数を呼び出す
```

### 動的関数

```
00| spush_str   "Hello, world!"         ; 第一引数をプッシュ
01| spush_str   "Hello, world!"         ; 第一引数をプッシュ
03| dyncall     join(str, str)          ; 関数を呼び出す
```

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
