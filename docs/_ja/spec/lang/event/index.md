# イベント

[ドキュメント](../../../../index.md) > [日本語](../../../index.md) > [仕様書](../../index.md) > [言語仕様書](../index.md) > [イベント](./index.md)

## 概要

実行中の出来事を抽象的に定義し、処理する。

## 用法

イベントを定義して利用する。

```
class MyCar

# イベントを定義
let self on_car_start = Event(s32 speed)

let self mut speed = 0s32

fn @const()
    # イベントリスナを追加
    CarEvent.on_car_start.add_listener(CarEvent.listen_car_start)
end

# イベントリスナを定義 ... リスナの引数をイベントと一致させる
fn listen_car_start(s32 speed)
    println("A car is running at " ~ speed ~ ".")
end

fn run(s32 speed)
    self.speed = speed
    # イベントを起こす
    OnCarStart.raise(speed)
end
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
