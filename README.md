# openLT
Maybe there will be an open laser tag game here. To be continued...

# TODO List

## Harware
* Gehäuse
* Mehrere IR Empfänger Dioden testen
* ATTiny per Batterie betreiben

## Software-Attiny
register:
* schussfrequenz (aktiv): uint_32 t_in_ms x02...\n
* 3 bytes zum senden (Möglichkeit 1) x01...\n

Möglichkeit 1:
* button löst senden von 3 bytes aus

Möglichkeit 2:
* button fragt App: was soll ich senden?

* daten empfangen per IR per BT weitersenden

## Software-Handy
*