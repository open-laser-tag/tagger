EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L tagger-rescue:ESP32-WROOM-32-RF_Module U?
U 1 1 5DFF9294
P 5650 3700
F 0 "U?" H 5650 5281 50  0000 C CNN
F 1 "ESP32-WROOM-32" H 5650 5190 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 5650 2200 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32_datasheet_en.pdf" H 5350 3750 50  0001 C CNN
	1    5650 3700
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:GND-power #PWR?
U 1 1 5DFFCCED
P 5650 5100
F 0 "#PWR?" H 5650 4850 50  0001 C CNN
F 1 "GND" H 5655 4927 50  0000 C CNN
F 2 "" H 5650 5100 50  0001 C CNN
F 3 "" H 5650 5100 50  0001 C CNN
	1    5650 5100
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:+3.3V-power #PWR?
U 1 1 5E03E674
P 6400 4850
F 0 "#PWR?" H 6400 4700 50  0001 C CNN
F 1 "+3.3V" H 6415 5023 50  0000 C CNN
F 2 "" H 6400 4850 50  0001 C CNN
F 3 "" H 6400 4850 50  0001 C CNN
	1    6400 4850
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 4350 6700 4350
$Comp
L tagger-rescue:GND-power #PWR?
U 1 1 5E03C9AE
P 6600 4350
F 0 "#PWR?" H 6600 4100 50  0001 C CNN
F 1 "GND" H 6605 4177 50  0000 C CNN
F 2 "" H 6600 4350 50  0001 C CNN
F 3 "" H 6600 4350 50  0001 C CNN
	1    6600 4350
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:TSOP312xx-Interface_Optical U?
U 1 1 5DFFF700
P 7100 4550
F 0 "U?" H 7088 4975 50  0000 C CNN
F 1 "TSOP312xx" H 7088 4884 50  0000 C CNN
F 2 "OptoDevice:Vishay_CAST-3Pin" H 7050 4175 50  0001 C CNN
F 3 "http://www.vishay.com/docs/82492/tsop312.pdf" H 7750 4850 50  0001 C CNN
	1    7100 4550
	-1   0    0    1   
$EndComp
Wire Wire Line
	7600 2400 7900 2400
Wire Wire Line
	7600 3500 7800 3500
Wire Wire Line
	7500 2500 7900 2500
Wire Wire Line
	7500 3300 7500 2500
$Comp
L tagger-rescue:+5V-power #PWR?
U 1 1 5E06C409
P 8450 2200
F 0 "#PWR?" H 8450 2050 50  0001 C CNN
F 1 "+5V" H 8465 2373 50  0000 C CNN
F 2 "" H 8450 2200 50  0001 C CNN
F 3 "" H 8450 2200 50  0001 C CNN
	1    8450 2200
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:+5V-power #PWR?
U 1 1 5E06B13F
P 8450 3300
F 0 "#PWR?" H 8450 3150 50  0001 C CNN
F 1 "+5V" H 8465 3473 50  0000 C CNN
F 2 "" H 8450 3300 50  0001 C CNN
F 3 "" H 8450 3300 50  0001 C CNN
	1    8450 3300
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:GND-power #PWR?
U 1 1 5E00EAED
P 7250 3600
F 0 "#PWR?" H 7250 3350 50  0001 C CNN
F 1 "GND" H 7255 3427 50  0000 C CNN
F 2 "" H 7250 3600 50  0001 C CNN
F 3 "" H 7250 3600 50  0001 C CNN
	1    7250 3600
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:APA102-LED D?
U 1 1 5DFFEDD7
P 8200 2500
F 0 "D?" H 8200 2981 50  0000 C CNN
F 1 "APA102" H 8200 2890 50  0000 C CNN
F 2 "LED_SMD:LED_RGB_5050-6" H 8250 2200 50  0001 L TNN
F 3 "http://www.led-color.com/upload/201506/APA102%20LED.pdf" H 8300 2125 50  0001 L TNN
	1    8200 2500
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 4000 6850 4000
Wire Wire Line
	7550 4000 7300 4000
$Comp
L tagger-rescue:GND-power #PWR?
U 1 1 5E011C57
P 7550 4000
F 0 "#PWR?" H 7550 3750 50  0001 C CNN
F 1 "GND" H 7555 3827 50  0000 C CNN
F 2 "" H 7550 4000 50  0001 C CNN
F 3 "" H 7550 4000 50  0001 C CNN
	1    7550 4000
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:GND-power #PWR?
U 1 1 5E010D45
P 8200 3900
F 0 "#PWR?" H 8200 3650 50  0001 C CNN
F 1 "GND" H 8205 3727 50  0000 C CNN
F 2 "" H 8200 3900 50  0001 C CNN
F 3 "" H 8200 3900 50  0001 C CNN
	1    8200 3900
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:GND-power #PWR?
U 1 1 5E010B2D
P 8200 2800
F 0 "#PWR?" H 8200 2550 50  0001 C CNN
F 1 "GND" H 8205 2627 50  0000 C CNN
F 2 "" H 8200 2800 50  0001 C CNN
F 3 "" H 8200 2800 50  0001 C CNN
	1    8200 2800
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:R-Device R?
U 1 1 5E0010A5
P 6700 4000
F 0 "R?" V 6600 4000 50  0000 L CNN
F 1 "20R" V 6770 3955 50  0000 L CNN
F 2 "" V 6630 4000 50  0001 C CNN
F 3 "~" H 6700 4000 50  0001 C CNN
	1    6700 4000
	0    -1   -1   0   
$EndComp
$Comp
L tagger-rescue:IR26-21C_L110_TR8-LED D?
U 1 1 5E000B9A
P 7150 4000
F 0 "D?" H 7143 4216 50  0000 C CNN
F 1 "TSUS5202" H 7143 4125 50  0000 C CNN
F 2 "LED_SMD:LED_1206_3216Metric" H 7150 4200 50  0001 C CNN
F 3 "http://www.everlight.com/file/ProductFile/IR26-21C-L110-TR8.pdf" H 7150 4000 50  0001 C CNN
	1    7150 4000
	-1   0    0    1   
$EndComp
$Comp
L tagger-rescue:SW_Push-Switch SW?
U 1 1 5DFFFB33
P 7050 3600
F 0 "SW?" H 7050 3885 50  0000 C CNN
F 1 "SW_Push" H 7050 3794 50  0000 C CNN
F 2 "" H 7050 3800 50  0001 C CNN
F 3 "~" H 7050 3800 50  0001 C CNN
	1    7050 3600
	1    0    0    -1  
$EndComp
$Comp
L tagger-rescue:WS2811-Driver_LED U?
U 1 1 5DFFD494
P 8200 3600
F 0 "U?" H 8200 4081 50  0000 C CNN
F 1 "WS2811" H 8200 3990 50  0000 C CNN
F 2 "" H 7900 3750 50  0001 C CNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2811.pdf" H 8000 3850 50  0001 C CNN
	1    8200 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7600 2400 7600 3400
Wire Wire Line
	7500 3300 6400 3300
Wire Wire Line
	6400 3300 6400 3700
Wire Wire Line
	6400 3700 6250 3700
Wire Wire Line
	7600 3400 6450 3400
Wire Wire Line
	6450 3400 6450 3800
Wire Wire Line
	6450 3800 6250 3800
Connection ~ 7600 3400
Wire Wire Line
	7600 3400 7600 3500
Wire Wire Line
	6850 3600 6500 3600
Wire Wire Line
	6500 3600 6500 3900
Wire Wire Line
	6500 3900 6250 3900
Wire Wire Line
	6250 4000 6550 4000
Wire Wire Line
	6400 4850 6700 4850
Wire Wire Line
	6700 4550 6400 4550
Wire Wire Line
	6400 4550 6400 4400
Wire Wire Line
	6400 4400 6250 4400
Wire Wire Line
	8450 3300 8200 3300
Wire Wire Line
	8200 2200 8450 2200
Wire Wire Line
	6700 4850 6700 4750
$EndSCHEMATC
