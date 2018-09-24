# attiny commands
| command		| command byte	| address byte	| data bytes	|
| ---------------------	| -------------	| ------------- | ------------- |
| write to register ...	| 0x01		| 0xXX		| 0xXXXXXXXX	|
| shoot bytes		| 0x01		| 0x01		| 0x00XXXXXX	|
| shoot cooldown	| 0x01		| 0x02		| t in ms	|
| shoot trigger delay	| 0x01		| 0x03		| t in ms	|
| shoot duration min	| 0x01		| 0x04		| t in ms	|
| shoot duration max	| 0x01		| 0x05		| t in ms	|
| shoot mode ...	| 0x01		| 0x06		| 0xXXXXXXXX	|
| shoot mode manual	| 0x01		| 0x06		| 0x00000001	|
| shoot mode automatic 	| 0x01		| 0x06		| 0x00000002	|
| burst shot amount min	| 0x01		| 0x07		| 0xXXXXXXXX	|
| burst shot amount max | 0x01		| 0x08		| 0xXXXXXXXX	|
| burst cooldown	| 0x01		| 0x09		| t in ms	|
| magazine size		| 0x01		| 0x0A		| 0xXXXXXXXX	|
| stream mode		| 0x02		| -		| 0xXXXXX....\n	|
| read register		| 0x03		| 0xXX		| 0x00000000	|

* example shoot freq. 512 ms: "0x0102000100"

** explanation **
* shoot bytes:
3 bytes long

* burst shot:
min = max = 1 => single shot per trigger

* magazine size:
0 => infinite shots
x > 0 => x shots per magazine
