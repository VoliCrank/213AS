.pos 0x100

compute_all:      
           ld $s, r0       # r0 = addr s
           ld (r0), r0     # r0 = s
           ld $n, r1       # r1 = addr n
           ld (r1), r4     # r1 = n
           not r4
           inc r4          # r1 = -n
           ld $0, r7       # r7 = 0 = i

outer_loop:
           mov r7, r2       # r2 = r7 = i
           add r4, r2       # r2 = r4 + r2 = n - i
           beq r2, end_all  # if i == n, goto end_avg
           inc r7           # i++

compute:   
           ld $4, r1             # r1 = 4 = j
           ld $0, r3             # r3 = 0 = total

compute_loop:
           beq r1, end_compute   # if j == 0, goto end_compute
           ld (r0,r1,4),r2       # r2 = s[i].grade[j]
           add r2,r3             # total += s[i].grade[j]
           dec r1                # j--
           br compute_loop       # goto compute_loop

end_compute:
           ld $5, r1             # r1 = 5
           shr $2 , r3           # total = total>>2 to find avg
           st r3, (r0,r1,4)      # s[i].average = r3
           ld $24, r3            # student offset
           add r3,r0             # add student offset to get to next student
           br outer_loop         # goto outer_loop
end_all:   
          j sort                 # jump to sort all registers cleared



sort:
          ld $n, r7              # r7 = addr n
          ld (r7), r7            # r7 = n
          dec r7                 # i = n - 1

sort_outer:
          beq r7, sort_end       # if i == 0, goto sort_end
          ld $1, r1              # r1 = 1 = j

sort_inner:
          mov r7, r4             # r4 = r7 = i
          not r4
          inc r4                 # r4 = -i
          add r1, r4             # r4 = j -i
          bgt r4, inner_end      # if j-i > 0, goto inner_end
          ld $temp, r2           # r2 = addr temp
          st r7, (r2)            # temp[0] = r7 = i
          st r1, 4(r2)           # temp[1] = r1 = j
          mov r1, r4             # r4 = r1 = j, for swap
          gpc $6, r6             # get ra
          j swap                 # jump to swap
          ld $temp, r2           # r2 = addr temp
          ld (r2), r7            # r7 = i
          ld 4(r2), r1           # r1 = j
          inc r1                 # j++
          br sort_inner          # goto sort_inner
inner_end:
          dec r7                 # r7--
          br sort_outer          # goto sort outer

sort_end: 
         j find_med              # jump to find median method


#assume r4 contains student index
swap:
            mov r4, r1              # r1 = r4 = j (student index)
            shl $4, r1              # r1 = r1 * 16
            shl $3, r4              # r4 = r4 * 8
            add r1,r4               # get addr offset, simulate j * 24

swap1:   
            ld $s, r0               # r0 = addr s
            ld (r0), r0             # r0 = s
            add r4, r0              # r0 = s+ offset

            #if s[i-1].avg > s[i] then...

            ld 20(r0), r1           # r1 = s[i].avg
            ld $-1, r2              # r2 = -4
            ld (r0,r2,4), r2        # r2 = s[i-1].avg
            not r2    
            inc r2                  # r2 = -s[i-1].avg
            add r2,r1               # r1 = s[i].avg - s[i-1].avg
            bgt r1, swap_end        # if s[i].avg >= s[i-1].avg goto swap_end
            beq r1, swap_end        # addition to previous line

            ld $t0, r2              # r2 = addr t0
            ld $6, r3               # r3 = 6 = x

           # t0 = s[i], s[i] = s[i-1] 
swap1_loop:   
            beq r3, swap2          # if r3 = x == 0, goto swap2
            dec r3                 # r3 -- = x--
            ld (r0,r3,4), r1       # r1 = s[i].attribute[x]
            st r1, (r2,r3,4)       # store into t1
            ld $-24, r7            # offset for student
            add r0, r7             # get prev student
            ld (r7,r3,4),r7        # r7 = s[i-1].attribute[x]
            st r7, (r0,r3,4)       # store into t0
            br swap1_loop          # goto swap1_loop

           #s[i-1] = t0

swap2: 

            ld $-24, r1             # r1 = -24
            add r1, r0              # s-= offset for student
            ld $t0, r1              # r1 = addr t0
            ld $6, r3               # r3 = x = 6 
swap2_loop:
            beq r3, swap_end        # if r3 = x == 0, goto swap_end
            dec r3                  # r3-- (x--)
            ld (r1,r3,4), r2        # r2 = t.attribute[x]
            st r2, (r0,r3,4)        # s[i-1].attribute= t.attribute[x]
            br swap2_loop           # goto swap2_loop

swap_end:   
            j (r6)                  # jump back to ra



find_med:

          ld $n, r0                # r0 = addr n
          ld (r0), r0              # r0 = n
          shr $1, r0               # r0 >>1 = n / 2
          mov r0, r1               # r1 = r0 = n/2
          shl $4, r1               # r1 <<4
          shl $3, r0               # r0 <<3
          add r1, r0               # r0 = n/2*24, offset for median index
          ld $s, r1                # r1 = addr s
          ld (r1), r1              # r1 = s
          add r1, r0               # offset for med student
          ld (r0), r0              # r0 = id for med
          ld $m, r1                # r1 = addr m
          st r0,(r1)               # m = med studentid
          halt












.pos 0x1000
n:    .long 5
m:    .long 0
s:    .long base


.pos 0x2000
base:
#s0
     .long 1234
     .long 80
     .long 60
     .long 78
     .long 90
     .long 0
 #s1:
     .long 444
     .long 45
     .long 100
     .long 1000
     .long 5456
     .long 0
#s2:     
     .long 555
     .long 5
     .long 5
     .long 5
     .long 5
     .long 0
#s3:     
     .long 666
     .long 90
     .long 90
     .long 100
     .long 100
     .long 0
 #s4:    
     .long 777
     .long 0
     .long 0
     .long 0
     .long 4
     .long 0

.pos 0x4000
t0:    .long 0
      .long 0
      .long 0
      .long 0
      .long 0
      .long 0
temp: .long 0
      .long 0



