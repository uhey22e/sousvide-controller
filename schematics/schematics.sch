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
L RF_Module:ESP32-WROOM-32D U?
U 1 1 5E538DF0
P 5600 3750
F 0 "U?" H 5600 5331 50  0000 C CNN
F 1 "ESP32-WROOM-32D" H 5600 5240 50  0000 C CNN
F 2 "RF_Module:ESP32-WROOM-32" H 5600 2250 50  0001 C CNN
F 3 "https://www.espressif.com/sites/default/files/documentation/esp32-wroom-32d_esp32-wroom-32u_datasheet_en.pdf" H 5300 3800 50  0001 C CNN
	1    5600 3750
	1    0    0    -1  
$EndComp
Text Label 6400 3950 0    50   ~ 0
SDA
Text Label 6400 4050 0    50   ~ 0
SCL
Wire Wire Line
	6400 3950 6200 3950
Wire Wire Line
	6200 4050 6400 4050
Text Label 6400 4450 0    50   ~ 0
TEMP
Wire Wire Line
	6400 4450 6200 4450
Text Label 6400 4350 0    50   ~ 0
SSR
Wire Wire Line
	6200 4350 6400 4350
$Comp
L Connector:Jack-DC J?
U 1 1 5E540D44
P 2350 2600
F 0 "J?" H 2407 2925 50  0000 C CNN
F 1 "Jack-DC" H 2407 2834 50  0000 C CNN
F 2 "" H 2400 2560 50  0001 C CNN
F 3 "~" H 2400 2560 50  0001 C CNN
	1    2350 2600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
