# 関数

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [関数](./index.md)

## 構文

### 関数定義

```
## 関数定義 ##

戻り値の型 関数名(引数)
    処理
end


## 関数呼び出し ##
関数名(引数)
```

#### コード例

```
# 関数定義
void printName(str name)
    # 関数呼び出し
    println(name)
end
```

### 関数呼び出し

```
関数名(引数)
```

#### コード例

```
printName("Garnet")
```

## 返り値

### 構文

### 例外値

例外値を返すことで呼び出し元で例外処理を行うことができる。

詳細は [例外 > 例外値](../exception/index.md) を参照。

## アクセス修飾子

[クラス > メンバ > アクセス修飾子](../class/index.md) を参照。

---

Copyright © 2020-2021 Garnet3106 All rights reserved.
