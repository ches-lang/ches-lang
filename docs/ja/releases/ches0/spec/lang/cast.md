# 型変換 (キャスト)

## 明示的型変換

### 構文

```
// (<対象の型>)(<元の値>)
```

#### コード例

```
(byt)10
(int)true
(dec)(123 * 2)

(byt)char  // chr char = ''
(int)decimal  // dec decimal = 5.14
```

## 暗黙の型変換

暗黙の型変換は基本的に行われない。

予期しないバグやエラーを防止するためである。

```
// 型変換を明示しない場合
byt num1 = 10
int num2 = num1  // エラー: invalid variable type

// 型変換を明示する場合
byt num1 = 10
int num2 = (int)num  // エラーなし

// 関数の引数においても型変換が必要である (int型を除く)
// func(int, byt)
func(500, (byt)100)
```

ただし、変数の初期化では数値リテラルの型変換は不要である。

```
byt num1 = 10  // エラーなし
byt num2 = (byt)10  // エラーは出ないが型変換は不要
```

## オーバーフロー

型変換時に型範囲を超えるとオーバーフローが発生する。

```
// エラー例
(byt)500  // overflow error
```

---

Copyright © 2020 Garnet3106 All rights reserved.
