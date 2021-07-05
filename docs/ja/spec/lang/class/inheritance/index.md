# 継承

[ドキュメント](../../../../../index.md) > [日本語](../../../../index.md) > [仕様書](../../../index.md) > [言語仕様書](../../index.md) > [クラス](../index.md) > [継承](./index.md)

## 構文

```
#== クラスの継承 ==#

class 継承先 : 継承元
```

```
#== 継承元クラスの定義 ==#

class SimpleTV

let mut is_power_on = false

fn self switch_power()
    self.is_power_on = !self.is_power_on
end
```

```
#== 継承先クラスの定義 ==#

class BrokenTV : SimpleTV

fn self switch_power()
    println("TV is broken now.")
end
```

---

Copyright © 2019-2021 Garnet3106 All rights reserved.
