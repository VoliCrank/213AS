.pos 0x100
start:
    ld $sb, r5            # ld stack addr on r5
    inca    r5            # r5 += 4
    gpc $6, r6            # get ra
    j main                # jump to main
    halt

f:
    deca r5               # allocate callee from for f
    ld $0, r0             # r0 = 0 = ret
    ld 4(r5), r1          # r1 = a
    ld $0x80000000, r2    # r2 = 0x80000000

f_loop:
    beq r1, f_end      # if r1 = a == 0; goto f_end
    mov r1, r3         # r3 = r1 
    and r2, r3         # r3 = c = r2 & r3
    beq r3, f_if1      # if r3 = c == 0, goto f_if1
    inc r0             # r0 ++ (ret++)
f_if1:
    shl $1, r1         # r1 = a << 1
    br f_loop          # goto f_loop
f_end:
    inca r5            # shrink stack
    j(r6)              # jump to ra

main:
    deca r5            # make space on stack
    deca r5            # allocate callee frame for main
    st r6, 4(r5)       # store ra in stack
    ld $8, r4          # r4 = 8 = i'

main_loop:
    beq r4, main_end   # if r4(i') == 0, goto main_end
    dec r4             # r4-- (i'--)
    ld $x, r0          # r0 = addr x
    ld (r0,r4,4), r0   # r0 = x[i']

    deca r5            # allocate caller frame
    st r0, (r5)        # pass r0 as arg
    gpc $6, r6         # get ra
    j f                # jump to f

    inca r5            # restore stack
    ld $y, r1          # r1 = addr y
    st r0, (r1,r4,4)   # y[i'] = r0 (return value)
    br main_loop       # goto main_loop

main_end:
    ld 4(r5), r6       # ld ra
    inca r5            # restore stack
    inca r5            # restore stack
    j (r6)             # jump back to start

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long -1
    .long -2
    .long 0
    .long 184
    .long 340057058

y:
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0

