# attiny commands
| command	| command byte	| address byte	| data bytes	|
| -------------	| -------------	| ------------- | ------------- |
| write to reg. | 0x01		| 0xXX		| 0xXXXXXXXX	|
| shoot bytes	| 0x01		| 0x01		| 0x00XXXXXX	|
| shoot freq.	| 0x01		| 0x02		| t in ms	|
| stream mode	| 0x02		| -		| 0xXXXXX....\n	|

* example shoot freq. 512 ms: "0x0102000100"
