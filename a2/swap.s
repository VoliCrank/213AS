.pos 0x100
						ld $t,r1						#r1 = address of t
						ld $array,r2				#r2 = address of array[0]
						ld $array,r3
						inca r3
						ld (r2),r4					#r4 = value of array[0]
						ld (r3),r5					#r5 = value of array[1]
						st r4,(r1)					#t=array[0]
						st r5,(r2)
						ld (r1),r1
						st r1,(r3)








						halt								#stop



.pos 0x1000
t:					.long 0		#t
.pos 0x2000
array:			.long 0		#arr[0]
						.long 0		#arr[1]
