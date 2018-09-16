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
| burst frequency	| 0x01		| 0x09		| t in ms	|
| stream mode		| 0x02		| -		| 0xXXXXX....\n	|

* example shoot freq. 512 ms: "0x0102000100"
