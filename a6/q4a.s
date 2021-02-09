.pos 0x0
                 ld   $sb, r5       # r5 = addr sb (stack)
                 inca r5            # r5 += 4
                 gpc  $6, r6        # get ra               
                 j    0x300         # jump to 0x300 jump to bar   
                 halt               # halt      
.pos 0x100
a*:
                 .long 0x00001000   # pointer to 0x1000 likely an array, call it b

.pos 0x200
foo(x,y):

                 ld   (r5), r0          # r0 = x 
                 ld   4(r5), r1         # r1 = y 
                 ld   $0x100, r2        # r2 = addr a    
                 ld   (r2), r2          # r2 = a
                 ld   (r2, r1, 4), r3   # r3 = a[y]   
                 add  r3, r0            # r0 = r3 + r0 = a[y] + x
                 st   r0, (r2, r1, 4)   # a[y] = a[y] + x
                 j    (r6)              # jump back to ra

.pos 0x300
bar():

                 ld   $-12, r0       # r0 = -12 allocate callee part of frame
                 add  r0, r5         # allocate stack   
                 st   r6, 8(r5)      # store ra
                 ld   $1, r0         # r0 = 1 = x
                 st   r0, (r5)       # save val to stack
                 ld   $2, r0         # r0 = 2 = y
                 st   r0, 4(r5)      # save val to stack

                 ld   $-8, r0        # r0 = -8 allocate caller part of frame
                 add  r0, r5         # r5 = r5 - 8 (add space to stack)    
                 ld   $3, r0         # r0 = 3 arg1
                 st   r0, (r5)       # save arg1 to stack
                 ld   $4, r0         # r0 = 4 arg2
                 st   r0, 4(r5)      # save arg2 to stack

                 gpc  $6, r6         # set return addr        
                 j    0x200          # jump to 0x200 (foo(3,4))

                 ld   $8, r0         # r0 = 8
                 add  r0, r5         # r5 += 8 (shrink stack)    
                 ld   (r5), r1       # r1 = x = 1
                 ld   4(r5), r2      # r2 = y = 2

                 ld   $-8, r0        # r0 = -8
                 add  r0, r5         # r5 -= 8 allocate for caller frame  
                 st   r1, (r5)       # x' = r1 = 1
                 st   r2, 4(r5)      # y' = r2 = 2
                 gpc  $6, r6         # get ra         
                 j    0x200          # jump foo(1,2)

                 ld   $8, r0         # r0 = 8
                 add  r0, r5         # r5 += 8 (shrink stack)     
                 ld   8(r5), r6      # get ra
                 ld   $12, r0        # r0 = 12
                 add  r0, r5         # shrink stack back to orginal    
                 j    (r6)           # jump back
.pos 0x1000
b:               .long 0
                 .long 0
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
