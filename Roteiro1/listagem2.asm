.text                    # secao de codigo (programa a ser executado)
      li a0,7            # addi x10,x0,7
      li a1,5            # addi x11,x0,5
      li a2,3            # addi x12,x0,3
volta:
      add a0,a0,a1       # add x10,x10,x11
      add a0,a0,a2       # add x10,x10,x11
      j volta            # jal x1,0xfffffff8