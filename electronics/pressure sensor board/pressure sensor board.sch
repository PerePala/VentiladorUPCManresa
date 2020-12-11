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
L PersonalLibrary:MP3V5004G U2
U 1 1 5EC37A09
P 7300 4100
F 0 "U2" H 7100 4200 50  0000 C CNN
F 1 "MP3V5004G" H 7275 4124 50  0000 C CNN
F 2 "PersonalLibrary:MP3V5004" H 7300 4100 50  0001 C CNN
F 3 "" H 7300 4100 50  0001 C CNN
	1    7300 4100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5EC38036
P 6650 4650
F 0 "#PWR06" H 6650 4400 50  0001 C CNN
F 1 "GND" H 6655 4477 50  0000 C CNN
F 2 "" H 6650 4650 50  0001 C CNN
F 3 "" H 6650 4650 50  0001 C CNN
	1    6650 4650
	1    0    0    -1  
$EndComp
Wire Wire Line
	6650 4450 6650 4650
Wire Wire Line
	6650 4450 6900 4450
$Comp
L Device:C C1
U 1 1 5EC385E2
P 6350 4750
F 0 "C1" H 6000 4800 50  0000 L CNN
F 1 "470pF" H 6000 4700 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6388 4600 50  0001 C CNN
F 3 "~" H 6350 4750 50  0001 C CNN
	1    6350 4750
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5EC392DB
P 7500 3200
F 0 "C4" H 7600 3400 50  0000 L CNN
F 1 "1uF" H 7600 3300 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7538 3050 50  0001 C CNN
F 3 "~" H 7500 3200 50  0001 C CNN
	1    7500 3200
	1    0    0    -1  
$EndComp
$Comp
L Device:C C2
U 1 1 5EC396D7
P 6850 3200
F 0 "C2" H 6950 3400 50  0000 L CNN
F 1 "10nF" H 6950 3300 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 6888 3050 50  0001 C CNN
F 3 "~" H 6850 3200 50  0001 C CNN
	1    6850 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6900 4550 6350 4550
Wire Wire Line
	6350 4600 6350 4550
Connection ~ 6350 4550
Wire Wire Line
	6350 4550 6000 4550
Wire Wire Line
	6350 4900 6350 4950
$Comp
L power:GND #PWR05
U 1 1 5EC39CE6
P 6350 4950
F 0 "#PWR05" H 6350 4700 50  0001 C CNN
F 1 "GND" H 6355 4777 50  0000 C CNN
F 2 "" H 6350 4950 50  0001 C CNN
F 3 "" H 6350 4950 50  0001 C CNN
	1    6350 4950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5EC39F71
P 6850 3400
F 0 "#PWR07" H 6850 3150 50  0001 C CNN
F 1 "GND" H 6855 3227 50  0000 C CNN
F 2 "" H 6850 3400 50  0001 C CNN
F 3 "" H 6850 3400 50  0001 C CNN
	1    6850 3400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 5EC3A176
P 7500 3400
F 0 "#PWR09" H 7500 3150 50  0001 C CNN
F 1 "GND" H 7505 3227 50  0000 C CNN
F 2 "" H 7500 3400 50  0001 C CNN
F 3 "" H 7500 3400 50  0001 C CNN
	1    7500 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 3350 6850 3400
Wire Wire Line
	7500 3350 7500 3400
$Comp
L Regulator_Linear:LP2985-3.0 U1
U 1 1 5EC3B32F
P 6350 2900
F 0 "U1" H 6150 3250 50  0000 C CNN
F 1 "LP2985-3.0" H 6350 3151 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 6350 3225 50  0001 C CNN
F 3 "http://www.ti.com/lit/ds/symlink/lp2985.pdf" H 6350 2900 50  0001 C CNN
	1    6350 2900
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5EC3D7E3
P 6350 3400
F 0 "#PWR04" H 6350 3150 50  0001 C CNN
F 1 "GND" H 6355 3227 50  0000 C CNN
F 2 "" H 6350 3400 50  0001 C CNN
F 3 "" H 6350 3400 50  0001 C CNN
	1    6350 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 3200 6350 3400
$Comp
L Device:C C3
U 1 1 5EC46006
P 7200 3200
F 0 "C3" H 7300 3400 50  0000 L CNN
F 1 "10nF" H 7300 3300 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric_Pad1.15x1.40mm_HandSolder" H 7238 3050 50  0001 C CNN
F 3 "~" H 7200 3200 50  0001 C CNN
	1    7200 3200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 5EC4639E
P 7200 3400
F 0 "#PWR08" H 7200 3150 50  0001 C CNN
F 1 "GND" H 7205 3227 50  0000 C CNN
F 2 "" H 7200 3400 50  0001 C CNN
F 3 "" H 7200 3400 50  0001 C CNN
	1    7200 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	7200 3350 7200 3400
Wire Wire Line
	6750 2800 7200 2800
Wire Wire Line
	6750 2900 6850 2900
Wire Wire Line
	6850 2900 6850 3050
Wire Wire Line
	7200 3050 7200 2800
Connection ~ 7200 2800
Wire Wire Line
	7200 2800 7500 2800
Wire Wire Line
	7500 3050 7500 2800
Connection ~ 7500 2800
Wire Wire Line
	7500 2800 7650 2800
$Comp
L Device:R R1
U 1 1 5EC47904
P 5650 2900
F 0 "R1" V 5750 2900 50  0000 C CNN
F 1 "1k" V 5850 2900 50  0000 C CNN
F 2 "Resistor_SMD:R_0805_2012Metric_Pad1.15x1.40mm_HandSolder" V 5580 2900 50  0001 C CNN
F 3 "~" H 5650 2900 50  0001 C CNN
	1    5650 2900
	0    1    1    0   
$EndComp
Wire Wire Line
	5950 2900 5800 2900
Wire Wire Line
	5950 2800 5400 2800
Wire Wire Line
	5500 2900 5400 2900
Wire Wire Line
	5400 2900 5400 2800
Connection ~ 5400 2800
Wire Wire Line
	5400 2800 5300 2800
Text GLabel 6750 4350 0    50   Input ~ 0
Vs
Text GLabel 7650 2800 2    50   Output ~ 0
Vs
Wire Wire Line
	6750 4350 6900 4350
Text GLabel 6000 4550 0    50   Output ~ 0
Vout
$Comp
L power:+5V #PWR03
U 1 1 5EC5E100
P 5300 2800
F 0 "#PWR03" H 5300 2650 50  0001 C CNN
F 1 "+5V" V 5315 2928 50  0000 L CNN
F 2 "" H 5300 2800 50  0001 C CNN
F 3 "" H 5300 2800 50  0001 C CNN
	1    5300 2800
	0    -1   -1   0   
$EndComp
$Comp
L Connector_Generic:Conn_01x05 J1
U 1 1 5EC5F514
P 3700 3400
F 0 "J1" H 3618 2975 50  0000 C CNN
F 1 "Conn_01x05" H 3618 3066 50  0000 C CNN
F 2 "Connector_Molex:Molex_SL_171971-0005_1x05_P2.54mm_Vertical" H 3700 3400 50  0001 C CNN
F 3 "~" H 3700 3400 50  0001 C CNN
	1    3700 3400
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5EC78124
P 4100 3700
F 0 "#PWR01" H 4100 3450 50  0001 C CNN
F 1 "GND" H 4105 3527 50  0000 C CNN
F 2 "" H 4100 3700 50  0001 C CNN
F 3 "" H 4100 3700 50  0001 C CNN
	1    4100 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3900 3400 4100 3400
Wire Wire Line
	4100 3400 4100 3600
Wire Wire Line
	3900 3600 4100 3600
Connection ~ 4100 3600
Wire Wire Line
	4100 3600 4100 3700
Wire Wire Line
	3900 3200 4250 3200
Wire Wire Line
	3900 3500 4250 3500
$Comp
L power:+5V #PWR02
U 1 1 5EC7A196
P 4250 3200
F 0 "#PWR02" H 4250 3050 50  0001 C CNN
F 1 "+5V" V 4265 3328 50  0000 L CNN
F 2 "" H 4250 3200 50  0001 C CNN
F 3 "" H 4250 3200 50  0001 C CNN
	1    4250 3200
	0    1    1    0   
$EndComp
Text GLabel 4250 3500 2    50   Input ~ 0
Vout
NoConn ~ 3900 3300
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5EC8828D
P 5400 2800
F 0 "#FLG01" H 5400 2875 50  0001 C CNN
F 1 "PWR_FLAG" H 5400 2973 50  0000 C CNN
F 2 "" H 5400 2800 50  0001 C CNN
F 3 "~" H 5400 2800 50  0001 C CNN
	1    5400 2800
	1    0    0    -1  
$EndComp
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 5EC8FED7
P 4100 3400
F 0 "#FLG0101" H 4100 3475 50  0001 C CNN
F 1 "PWR_FLAG" H 4100 3573 50  0000 C CNN
F 2 "" H 4100 3400 50  0001 C CNN
F 3 "~" H 4100 3400 50  0001 C CNN
	1    4100 3400
	1    0    0    -1  
$EndComp
Connection ~ 4100 3400
$EndSCHEMATC