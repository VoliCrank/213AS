.pos 0x100
					ld $a,r0
					ld $b,r1
					ld $b,r2
					ld (r1),r1
					ld (r2),r2
					inc r1
					inc r1
					inc r1
					inc r1
					inc r1
					shr $1,r1
					and r1,r2
					shl $2,r2
					st r2,(r0)
						halt								#stop
.pos 0x1000
a:					.long 0
.pos 0x2000
b:					.long 0
