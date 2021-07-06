# for 文

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [for 文](./index.md)

## 概要

条件付きのループ処理を行う。

## 構文

### 真偽ループ

`true` を指定すれば無限ループになり、`false` を指定すれば一度も処理がなされない。

`false` の指定は非推奨であるため警告が発生する。

```
#== for <真偽値> ==#

for true
    println("infinite loop")
end

# 警告
for false
    println("never called")
end
```

### 回数ループ

```
#== for <数値> ==#

for 10
    println("ten times")
end
```

### 回数ループ (for-in)

```
#== for <数値> ==#

# for i in 0..10 と同様
for i in 10
    println("ten times: " ~ i)
end
```

### スライスループ (for-in)

スライスについては [スライス](../slice/index.md) を参照。

```
#== for <変数> in <スライス値> ==#

for i in 5:10
    println("five times from five")
end
```

### イテレータループ (for-in)

イテレータについては [イテレータ](../iterator/index.md) を参照。

```
#== for <変数> in <イテレータ> ==#

let vec = Vec<str>["a", "b", "c"]

for i in vec
    println(i)
end

for [i, item] in vec
    println(i ~ ": " ~ item)
end

for [i, item] in vec[1:]
    println(i ~ ": " ~ item)
end
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
