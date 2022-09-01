<link rel="stylesheet" href="../style.css">

# ブロック

## 定義ブロック

`定義ブロック`

```
ブロック開始
    子要素1
    子要素2
    ...
end
```

Start: ブロックの開始部分

<div class="syntax-definition">
    Block::BlockDefinition(Start) :=
    <div class="syntax-definition-indent">
        Start
        [a](a)
    </div> 
    <div class="syntax-definition-indent">
        "end"
    </div>
</div>

## ブロック式

\>\> [式 > ブロック式](../expression/index.md#ブロック式)

---

Copyright © 2019- Ches All rights reserved.
