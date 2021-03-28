# 命令

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [CVM](../../index.md) > [バイトコード](../index.md) > [命令](./index.md)

## 命令一覧

|命令コード|命令名|演算内容|
|:-:|:-:|:-:|
|0x00|none|何もしない|
|0x<br>0x<br>0x<br>0x|add_i8<br>add_i16<br>add_i32<br>add_i64|ビット加算|
|0x<br>0x<br>0x<br>0x|sub_i8<br>sub_i16<br>sub_i32<br>sub_i64|ビット減算|
|0x<br>0x<br>0x<br>0x|mul_i8<br>mul_i16<br>mul_i32<br>mul_i64|ビット乗算|
|0x<br>0x<br>0x<br>0x|div_i8<br>div_i16<br>div_i32<br>div_i64|ビット除算|
|0x|and_8|8 ビット論理積|
|0x|or_8|8 ビット論理和|
|0x|eq_ref|参照比較|
|0x<br>0x<br>0x<br>0x|eq_i8<br>eq_i16<br>eq_i32<br>eq_i64||
|0x<br>0x<br>0x<br>0x|cmp_i8<br>cmp_i16<br>cmp_i32<br>cmp_i64|ビット比較|
|0x<br>0x<br>0x<br>0x|rev_i8<br>rev_i16<br>rev_i32<br>rev_i64|ビット反転|
|0x|spush_arg|引数プッシュ|
|0x|spush_arr|配列要素のプッシュ|
|0x|spop|ポップ及びヒープ開放|
|0x|spop_ref|参照ポップ|
|0x<br>0x<br>0x<br>0x|aalc_8<br>aalc_16<br>aalc_32<br>aalc_64|配列領域確保|
|0x|ares|配列領域リサイズ|
|0x|aset|配列要素設定|
|0x|goto|命令移動|
|0x|goto_if|条件付き命令移動|
|0x|ret|ブロック抜け|
|0x|syscall|システム命令|

### none ( 0x00 )

何もしない。

### add_8 add_16 add_32 add_64 ( 0x ~ 0x )

8 ビットの値同士を加算する。

#### スレッド領域

スレッドスタックに演算結果をプッシュする。

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
