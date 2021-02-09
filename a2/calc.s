.pos 0x100
								 ld   $data, r0           # load data into r0
								 ld   $i, r1              # load i into r1
								 ld   $y, r5							# load address of y into r5
								 ld   0x0(r1), r1         # load value of i into r1
								 ld   (r0, r1, 4), r2     # store data[i] into r2
								 inc  r1                  # i++
								 ld (r0,r1,4),r3					#load data[i+1] into r3
								 add r3,r2								#r2=r2+r3
								 st r2,(r5)								#y=r2
								 ld $0xff,r4							#r4=0xff
								 and r2,r4								#r4=r2&r4
								 ld $x,r6									#r6=address of x
								 st r4,(r6)								#x=r4
								 halt                     # stop
.pos 0x1000
data:            .long 0xff
                 .long 0x0
                 .long 0x0
                 .long 0x0
.pos 0x2000
i:               .long 0x0
x:               .long 0x0
y:               .long 0xff
