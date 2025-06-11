; Tol Radius Compensation test program
; Comments start with ';' or '#'
N10 G00 X-30 Y0 Z300 S1000 T01 ; Load tool 1 with radius 3 mm
N20 G00 Z50
; TRC enabled at the end of this block:
N30 G01 G42 Z20 F500
N40 G01 X0 F2000
N50 G01 X100
N60 G01 X130 Y30
N70 G01 X50
N80 G01 Y100
N90 G03 X80 Y130 R30
N100 G03 Y170 I0 J20
N110 G01 X50
N120 G03 X30 Y150 R-20
N130 G02 X0 Y125 R-20
; TRC disabled at the end of this block:
N140 G01 G40 Y0
N150 G01 Y-30
N160 G00 Z150