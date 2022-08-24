# 2-5. 関数

関数はモジュールだけでなく今後学習する構造体や列挙体などの中にも記述することができます。

一番最初に実行される `main()` ( `main関数` ) はモジュール直下に定義します。

以下に関数の基本的な構文を示します。

`2-5.1`

```ruby
fn 関数名() 返り値の型
    関数内の処理
end
```

返り値がない (正確には空値にあたる `void` を返す) 関数では返り値の型を省略します。

引数を受け取るには `関数名(引数名 型, ...)` と記述し、関数から呼び出し元に値を返すには `ret` 式を用います。

`2-5.2`

```ruby
fn main()
    println(get_message("John", 20))
end

# 引数 name, age を受け取って str 値を返す関数
fn get_message(name str, age s32) str
    ret "私は ${age} 歳の ${name} です。"
end
```

`2-5.3`

```ruby

```

---

Copyright © 2019- Ches All rights reserved.
