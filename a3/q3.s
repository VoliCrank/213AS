.pos 0x100

    # a = 3
    ld $a, r0         # r0 = addr tmp
    ld $3, r1         # r1 = 3
    st r1, 0(r0)      # a = r1

    # p = &a
    ld $a, r0         # r0 = addr a
    ld $p, r1         # r1 = addr p
    st r0, 0(r1)      # p = &a

    # *p = *p - 1
    ld $p, r0         # r0 = addr p
    ld 0(r0), r0      # r0 = p
    ld 0(r0), r1      # r1 = p*
    dec r1            # r1 = r1 - 1
    st r1, 0(r0)      # p* = p* - 1

    # p = &b[0]
    ld $b,r0          # r0 = addr b
    ld $p, r1         # r1 = addr p
    st r0, 0(r1)      # p = r0 = &b[0]

    # p++
    ld $p, r0         # r0 = addr p
    ld 0(r0), r1      # r1 = p
    inca r1           # r1 = r1 + 4
    st r1, 0(r0)      # p = r1

    # p[a] = b[a]
    ld $a, r0         # r0 = addr a
    ld 0(r0), r0      # r0 = a
    ld $b, r1         # r1 = addr b
    ld (r1,r0,4), r1  # r1 = b[a]
    ld $p, r2         # r2 = addr p
    ld 0(r2), r2      # r2 = p
    st r1, (r2,r0,4)  # p[a] = r1

    # *(p+3) = b[0]
    ld $b, r0         # r0 = addr b
    ld $0, r1         # r1 = 0
    ld (r0,r1,4), r1  # r1 = b[0]
    ld $p, r0         # r0 = addr p
    ld 0(r0), r0      # r0 = p
    ld $3, r2         # r2 = 3
    st r1, (r0,r2,4)  # p[3] = b[0]



halt

.pos 0x2000
# Data area

a:  .long 0           # a
p:  .long 0           # p
b:  .long 0           # b[0]
    .long 0           # b[1]
    .long 0           # b[2]
    .long 0           # b[3]
    .long 0           # b[4]
