# 命令

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [CVM](../../index.md) > [CVM コード](../index.md) > [命令](./index.md)

## 命令一覧

命令名の `T` はデータ型、`S` はデータサイズを示す。

[命令 > データ型一覧](./index.md#データ型一覧) 及び [命令 > データサイズ一覧](./index.md#データサイズ一覧) を参照。

|命令コード|命令名|演算内容|
|:-:|:-:|:-:|
|0x00|none|何もしない|
|0x ~ 0x|aalc_S|配列領域確保|
|0x|ares|配列領域サイズ変更|
|0x|aset|配列要素設定|
|0x ~ 0x|add_T|ビット加算|
|0x|and_8b|8 ビット論理積|
|0x|call|関数呼び出し|
|0x ~ 0x|cmp_ref|参照ビット比較|
|0x ~ 0x|cmp_T|ビット比較|
|0x ~ 0x|div_T|ビット除算|
|0x|eq_ref|参照ビット一致|
|0x ~ 0x|eq_T|ビット一致|
|0x|goto|命令移動|
|0x|goto_if|条件付き命令移動|
|0x|hset|ヒープ値設定|
|0x|memset|クラスメンバ値設定|
|0x ~ 0x|mul_T|ビット乗算|
|0x|or_8b|8 ビット論理和|
|0x|ret|ブロック抜け|
|0x ~ 0x|rev_T|ビット反転|
|0x|spop|ポップ及びヒープ開放|
|0x|spop2|2 要素ポップ及びヒープ開放|
|0x|spop_ref|参照ポップ|
|0x|spop2_ref|2 要素参照ポップ|
|0x|spush_arg|引数プッシュ|
|0x|spush_arr|配列要素プッシュ|
|0x|spush_copy|コピープッシュ|
|0x|spush_new|新規インスタンスプッシュ|
|0x|spush_null|null 値プッシュ|
|0x|spush_ref|参照プッシュ|
|0x|spush_str|文字列プッシュ|
|0x|spush_this|クラス参照プッシュ|
|0x ~ 0x|sub_T|ビット減算|
|0x|syscall|システム命令|
|0x|thcret|スレッド作成|
|0x|thdisp|スレッド破棄|
|0x|thstart|スレッド処理開始|
|0x|thstop|スレッド処理停止|

<style>
/* 命令のタイトルに使用する */

span.instTitle {
    background-color: #888888;
    padding: 3px 5px;
}
</style>

### <span class="instTitle">none ( 0x00 )</span>

何もしない。

### <span class="instTitle">add_T ( 0x ~ 0x )</span>

値同士を加算する。

#### スレッドスタック

##### ポップ

- 入力されたデータ値 ( 要素数 2 )

##### プッシュ

- 演算結果値 ( サイズ T; 要素数 1 )

### <span class="instTitle">call ( 0x )</span>

[処理手順 - 関数](../../procs/function/index.md) を参照。

### <span class="instTitle">thcret ( 0x )</span>

新たにスレッドを作成し、スタック領域を割り当てる。

#### スレッドスタック

##### ポップ

- 実行する関数の参照 ( 4 バイト参照; 要素数 1 )

##### プッシュ

- スレッド ID ( 8 バイト整数; 要素数 1 )

### <span class="instTitle">thdisp ( 0x )</span>

スレッド及び割り当てられたメモリ領域を破棄する。

スレッドが処理途中である場合は処理が停止されるため、スレッド破棄の直前に `thstop` 命令を実行する必要はない。

#### スレッドスタック

##### ポップ

- スレッド ID ( 8 バイト整数; 要素数 1 )

##### プッシュ

- 実行した関数の返り値 ( 値の参照; 要素数 1 )

### <span class="instTitle">thstart ( 0x )</span>

スレッド処理の実行を開始する。

#### スレッドスタック

##### ポップ

- スレッド ID ( 8 バイト整数; 要素数 1 )

### <span class="instTitle">thstop ( 0x )</span>

スレッドの処理を停止する。

#### スレッドスタック

##### ポップ

- スレッド ID ( 8 バイト整数; 要素数 1 )

## データ型一覧

|型|名称|対応型|
|:-:|:-:|:-:|
|i8|8 ビット整数, 8 ビット論理値|bool, s8, u8|
|i16|16 ビット整数|s16, u16|
|i32|32 ビット整数|s32, u32|
|i64|64 ビット整数|s64, u64|
|f32|32 ビット浮動小数点数|f32|
|f64|64 ビット浮動小数点数|f64|

## データサイズ一覧

|サイズ ( ビット )|
|:-:|
|8|
|16|
|32|
|64|

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
