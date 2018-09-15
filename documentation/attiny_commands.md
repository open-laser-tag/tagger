# attiny commands
| command		| command byte	| address byte	| data bytes	|
| ---------------------	| -------------	| ------------- | ------------- |
| write to register ...	| 0x01		| 0xXX		| 0xXXXXXXXX	|
| shoot bytes		| 0x01		| 0x01		| 0x00XXXXXX	|
| shoot freq.		| 0x01		| 0x02		| t in ms	|
| shoot delay		| 0x01		| 0x03		| t in ms	|
| shoot duration	| 0x01		| 0x04		| t in ms	|
| shoot mode ...	| 0x01		| 0x05		| 0xXXXXXXXX	|
| shoot mode manual	| 0x01		| 0x05		| 0x00000001	|
| shoot mode burst	| 0x01		| 0x05		| 0x00000002	|
| shoot mode automatic 	| 0x01		| 0x05		| 0x00000003	|
| burst shot amount	| 0x01		| 0x06		| 0xXXXXXXXX	|
| stream mode		| 0x02		| -		| 0xXXXXX....\n	|

* example shoot freq. 512 ms: "0x0102000100"
