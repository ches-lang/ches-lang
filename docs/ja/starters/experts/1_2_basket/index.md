# 1-2. バスケットを活用しよう

\- ではパッケージにあたるバスケットを作成します。

`bask new` コマンドで `hello` という名前のバスケットを作成しましょう。

同時に作業ディレクトリをプロジェクトディレクトリに移動します。

> \$ bask new hello
> \$ cd hello

バスケット作成時に自動で生成される `src/main.ches` は一番最初に実行される `main` モジュールです。

```
use std::*

fn main()
    println("Hello, world!")
end
```

`bask run` コマンドでバスケットを実行してみましょう。

> \$ bask run
> Hello, world!

`Hello, world!` と表示されれば成功です。

todo: モジュールを解説

---

Copyright © 2019- Ches All rights reserved.
