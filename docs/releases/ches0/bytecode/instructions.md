# Chestnut Bytecode Instructions

## Special Instruction List

|Code|Name|Function|
|:-:|:-:|:-:|
|0x|lndiv|Begin a new line.|
|0x|tkdiv|Begin a new token.|
|0x|llpref|Indicate element of the local list.|

## Normal Instruction List

|Code|Name|Args|Function|
|:-:|:-:|:-:|:-:|
|0x|unknown||Unknown instruction|
|0x|group|[grp_num] [grp_name]|Define a group of instructions.|
|0x|lspush|[value]|Push the value to the local stack.|
|0x|llpush|[value]|Push the value to the local list.|
|0x|call|[grp_num]|Call the group.|
|0x|return||Return the value.|
|0x|land||Process AND operation.|
|0x|lor||Process OR operation.|
|0x|nadd||Add numbers on the local stack.|
|0x|nsub||Subtract numbers on the local stack.|
|0x|nmul||Multiply numbers on the local stack|
|0x|ndiv||Divide numbers on the local stack|
