.eqv DSP_right 0xFFFF0010
.text
    li x10, DSP_right
    addi x11, x0, 0
volta:
    sb x11, 0(x10)
    xori x11, x11, 0x80
    j volta