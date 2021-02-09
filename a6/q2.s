.pos 0x100
code:

        ld $0, r0             # r0 = 0 = i
        ld $n, r1             # r1 = addr n
        ld (r1), r1           # r1 = n
        ld $a, r3             # r3 = addr a
        ld $b, r4             # r4 = addr b
        ld $c, r5             # r5 = addr c
        ld (r5), r5           # r5 = c' (since not using procedure calls, I use r5)

  loop: mov r0, r2            # r2 = r0 = i'
        not r2       
        inc r2                # r2 = -i'
        add r1,r2             # r1 = n-i
        beq r2, end           # if n-i == 0, goto end
        ld (r3,r0,4), r2      # r2 = a[i']
        ld (r4,r0,4), r7      # r7 = b[i']
        not r7
        inc r7                # r7 = -b[i']
        add r2,r7             # r2 = a[i'] - b[i']
        bgt r7, if            # if a[i] - b[i] > 0, goto if
        inc r0                # i++
        br loop               # goto loop
 
   if:  inc r5                # r5++ (c'++)
        inc r0                # i++
        br loop               # goto loop

   end: ld $c, r1             # r1 = addr c
        st r5, (r1)           # c = r5 = c'
        ld $i, r1             # r1 = addr i
        st r0, (r1)           # i = i'
        halt




.pos 0x1000

i:  .long -1
n:  .long 5
a:  .long 10
    .long 20
    .long 30
    .long 40
    .long 50
 b: .long 11
    .long 20
    .long 28
    .long 44
    .long 48
 c: .long 0