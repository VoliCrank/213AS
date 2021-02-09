.pos 0x0
                 ld   $0x1028, r5
                 ld   $0xfffffff4, r0    # r0 = -12
                 add  r0, r5
                 ld   $0x200, r0         # r0 = addr a
                 ld   0x0(r0), r0        # r0 = a
                 st   r0, 0x0(r5)        # store a into stack
                 ld   $0x204, r0         # r0 = addr b
                 ld   0x0(r0), r0        # r0 = b
                 st   r0, 0x4(r5)        # store b into stack
                 ld   $0x208, r0         # r0 = addr c
                 ld   0x0(r0), r0        # r0 = c
                 st   r0, 0x8(r5)        # store c into stack
                 gpc  $6, r6             # get ra
                 j    0x300              # q2(a,b,c)
                 ld   $0x20c, r1         # r1 = addr d
                 st   r0, 0x0(r1)        # d = r0
                 halt
.pos 0x200
 a:               .long 0x00000000
 b:               .long 0x00000000
 c:               .long 0x00000000
 d:               .long 0x00000000
.pos 0x300
                 ld   0x0(r5), r0         # r0 = a = arg1
                 ld   0x4(r5), r1         # r1 = b = arg2
                 ld   0x8(r5), r2         # r2 = c = arg3
                 ld   $0xfffffff6, r3     # r3 = -10
                 add  r3, r0              # r0 = a-10 = arg1-10
                 mov  r0, r3              # r3 = r0 = a-10 = arg1-10
                 not  r3                  
                 inc  r3                  # r3 = -r0 = 10-a = 10-arg1
                 bgt  r3, L6              # if arg1 < 10, goto L6
                 mov  r0, r3              # r3 = r0 = a-10 = arg1-10
                 ld   $0xfffffff8, r4     # r4 = -8
                 add  r4, r3              # r3 = a-10-8 = a-18 = arg1-18
                 bgt  r3, L6              # if arg1 > 18 goto L6
                 ld   $0x400, r3          # r3 = 0x400 = addr jumptable
                 ld   (r3, r0, 4), r3     # r3 = jumptable [arg1-10]
                 j    (r3)                #  goto jumptable[arg1-10]
.pos 0x330
 case1:          add  r1, r2              # r2 = r1+r2 = arg2 + arg3 
                 br   L7                  # goto L7 (end)

 case2:          not  r2                  
                 inc  r2                  # r2 = -r2 = -c = -arg3
                 add  r1, r2              # r2 = b - c = arg2-arg3
                 br   L7                  # goto L7 (end)

 case3:          not  r2                  
                 inc  r2                  # r2 = -c
                 add  r1, r2              # r2 = b - c
                 bgt  r2, L0              # if b - c > 0 (b>c) goto L0 (arg2>arg3)
                 ld   $0x0, r2            # r2 = 0
                 br   L1                  # goto L1

L0:              ld   $0x1, r2            # r2 = 1
L1:              br   L7                  # goto L7 (end)

 case4:          not  r1                 
                 inc  r1                  # r1 = -arg2
                 add  r2, r1              # r1 = arg3 - arg2
                 bgt  r1, L2              # if arg3 - arg2 > 0, goto L2 (arg3>arg2)
                 ld   $0x0, r2            # r2 = 0
                 br   L3                  # goto L3

L2:              ld   $0x1, r2            # r2 = 1
L3:              br   L7                  # goto L7

case5:           not  r2                 
                 inc  r2                 # r2 = -r2 = -arg3
                 add  r1, r2             # r2 = arg2 - arg3
                 beq  r2, L4             # if arg2 == arg3 goto L4
                 ld   $0x0, r2           # r2 = 0
                 br   L5                 # goto L5

L4:              ld   $0x1, r2          # r2 = 1
L5:              br   L7                # goto L7 (end)

L6:              ld   $0x0, r2          # r2 = 0
                 br   L7                # goto L7 (end)

L7:              mov  r2, r0            # r0 = r2 = return value
                 j    0x0(r6)           # jump back
.pos 0x400
jumptable:       .long 0x00000330       # case 1
                 .long 0x00000384       # default
                 .long 0x00000334       # case 2
                 .long 0x00000384       # default
                 .long 0x0000033c       # case 3
                 .long 0x00000384       # default
                 .long 0x00000354       # case 4
                 .long 0x00000384       # default
                 .long 0x0000036c       # case 5
.pos 0x1000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
                 .long 0x00000000
