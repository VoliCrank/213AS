.pos 0x100

    # tmp = 0, tos = 0
    ld $tmp, r0           # r0 = addr tmp
    ld $0, r1             # r1 = 0
    st r1, 0(r0)          # tmp = 0
    ld $tos, r0           # r0 = addr tos
    ld $0, r1             # r1 = 0
    st r1, 0(r0)          # tmp = 0

    # s[tos] = a[0]
    ld $s, r0             # r0 = addr s
    ld $0, r1             # r1 = 0
    ld $a, r2             # r2 = addr a
    ld (r2,r1,4),r2       # r2 = a[0]
    ld $tos, r1           # r1 = addr tos
    ld 0(r1), r3          # r3 = tos
    st r2, (r0,r3,4)      # s[tos] = a[0]

    # tos++
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r1          # r1 = tos
    inc r1                # r1 = r1 + 1
    st r1, 0(r0)          # tos = r1

    # s[tos] = a[1]
    ld $s, r0             # r0 = addr s
    ld $1, r1             # r1 = 1
    ld $a, r2             # r2 = addr a
    ld (r2,r1,4),r2       # r2 = a[1]
    ld $tos, r1           # r1 = addr tos
    ld 0(r1), r3          # r3 = tos
    st r2, (r0,r3,4)      # s[tos] = a[1]

    # tos++
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r1          # r1 = tos
    inc r1                # r1 = r1 + 1
    st r1, 0(r0)          # tos = r1

    # s[tos] = a[2]
    ld $s, r0             # r0 = addr s
    ld $2, r1             # r1 = 2
    ld $a, r2             # r2 = addr a
    ld (r2,r1,4),r2       # r2 = a[2]
    ld $tos, r1           # r1 = addr tos
    ld 0(r1), r3          # r3 = tos
    st r2, (r0,r3,4)      # s[tos] = a[2]

    # tos++
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r1          # r1 = tos
    inc r1                # r1 = r1 + 1
    st r1, 0(r0)          # tos = r1

    # tos--
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r1          # r1 = tos
    dec r1                # r1 = r1 - 1
    st r1, 0(r0)          # tos = r1

    # tmp = s[tos]
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r0          # r0 = tos
    ld $s, r1             # r1 = addr s
    ld (r1,r0,4), r1      # r1 = s[tos]
    ld $tmp, r0           # r0 = tmp
    st r1, 0(r0)          # tmp = r1 = s[tos]

    # tos--
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r1          # r1 = tos
    dec r1                # r1 = r1 - 1
    st r1, 0(r0)          # tos = r1

    # tmp = tmp + s[tos]
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r0          # r0 = tos
    ld $s, r1             # r1 = addr s
    ld (r1,r0,4), r1      # r1 = s[tos]
    ld $tmp, r0           # r0 = addr tmp
    ld 0(r0), r2          # r2 = tmp
    add r2, r1            # r1 = r1 + r2 = tmp + s[tos]
    st r1, 0(r0)          # tmp = tmp + s[tos]

    # tos--
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r1          # r1 = tos
    dec r1                # r1 = r1 - 1
    st r1, 0(r0)          # tos = r1

    # tmp = tmp + s[tos]
    ld $tos, r0           # r0 = addr tos
    ld 0(r0), r0          # r0 = tos
    ld $s, r1             # r1 = addr s
    ld (r1,r0,4), r1      # r1 = s[tos]
    ld $tmp, r0           # r0 = addr tmp
    ld 0(r0), r2          # r2 = tmp
    add r2, r1            # r1 = r1 + r2 = tmp + s[tos]
    st r1, 0(r0)          # tmp = tmp + s[tos]


    halt

.pos 0x2000
# Data area

tos:  .long 0           # tos
tmp:  .long 0           # tmp
s:    .long 0           # s[0]
      .long 0           # s[1]
      .long 0           # s[2]
      .long 0           # s[3]
      .long 0           # s[4]
a:    .long 0           # a[0]
      .long 0           # a[1]
      .long 0           # a[2]
