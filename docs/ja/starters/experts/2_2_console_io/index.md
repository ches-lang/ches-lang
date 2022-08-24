# 2-2. コンソール I/O

コンソール出力を行う関数には末尾に改行を付けない [print()](/docs/api/std#print) と改行を付ける [println()](/docs/api/std#println) があります。

コンソール入力を受け付ける関数としては 1 文字の入力を受け付ける [read()](/docs/api/std#read) と 1 行の入力を受け付ける [readln()](/docs/api/std#readln) が用意されています。

以下の例ではコンソールで入力された文字を変数 `input` に代入してその内容を出力しています。

`2-2.1`

```ruby
fn main()
    let input = read()
    println(input)
end
```

以下の例では文字ではなく入力された行を出力します。

`2-2.2`

```ruby
fn main()
    let input = readln()
    println(input)
end
```



---

Copyright © 2019- Ches All rights reserved.
