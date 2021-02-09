.pos 0x100

    # i = a[3]
    ld $i, r0           # r0 = addr i
    ld $a, r1           # r1 = addr a
    ld $3, r2           # r2 = 3
    ld (r1,r2,4), r2    # r2 = a[3]
    st r2, 0(r0)        # i = a[3]


    # i = a[i]
    ld $i, r0           # r0 = addr i
    ld $a, r1           # r1 = addr a
    ld 0(r0), r2        # r2 = i
    ld (r1,r2,4), r2    # r2 = a[i]
    st r2, 0(r0)        # i = a[i]

    # p = &j
    ld $j, r0           # r0 = addr j
    ld $p, r1           # r1 = addr p
    st r0, 0(r1)        # p = &j

    # *p = 4
    ld $p, r0           # r0 = addr p
    ld 0(r0), r0        # r0 = p
    ld $4, r1           # r1 = 4
    st r1, 0(r0)        # *p = 4

    # p = &a[a[2]]
    ld $a, r0           # r0 = addr a
    ld $2, r1           # r1 = 2
    ld (r0,r1,4), r2    # r2 = a[2]
    shl $2,r2           # r2 >> 2 (r2 * 4)
    ld $p, r3           # r3 = addr p
    add r0,r2           # r2 = r0 + r2
    st r2, 0(r3)        # p = r3 = &a[a[2]]

    # *p = *p + a[4]
    ld $a, r0           # r0 = addr a
    ld $4, r1           # r1 =4
    ld (r0,r1,4),r3     # r3 = a[4]
    ld $p, r1           # r1 = addr p
    ld 0(r1), r1        # r1 = p
    ld 0(r1), r2        # r2 = p*
    add r2, r3          # r0 = r2 + r3
    st  r3, 0(r1)       # *p = *p + a[4]

    halt

.pos 0x200
# Data area

i:  .long 0             # i
j:  .long 0             # j
p:  .long 0             # p
a:  .long 0             # a[0]
    .long 0             # a[1]
    .long 0             # a[2]
    .long 0             # a[3]
    .long 0             # a[4]
    .long 0             # a[5]
    .long 0             # a[6]
    .long 0             # a[7]
    .long 0             # a[8]
    .long 0             # a[9]
