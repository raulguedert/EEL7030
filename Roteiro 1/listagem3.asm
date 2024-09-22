#
# Constantes
#
.eqv DSP_right 0xFFFF0010                   # endereco do display
.eqv NRO_dados 15

#
# Area de dados (variaveis)
#
.data
    dado:
    .byte 0x3F, 0x06, 0x5B, 0x4F, 0x66          # 0, 1, 2, 3, 4
    .byte 0x6D, 0x7D, 0x07, 0x7F, 0x6F          # 5, 6, 7, 8, 9
    .byte 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71    # A, B, C, D, E, F

#
# Area de codigo (programa)
#
.text
        li a3, NRO_dados
        li a0, DSP_right                     # endereco do display em a0
        la a1, dado                          # dado a ser mostrado em a1
    volta: lb a2, 0(a1)
        sb a2, 0(a0)
        addi a1, a1, 1
        addi a3, a3, -1
        bgez a3, volta
    fim:   j fim