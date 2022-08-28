# Repository template to be used by ELF74/CSW41 students

## Lab1 - Tiva board without TivaWare

Questão 6.

- As mensagem enviadas via cout são mostradas na janela "Terminal I/O".
- A janela "Input" serve para enviar os dados da chamada cin.
- "__DATE__" e "__TIME__", data e hora, respectivamente.
- "VADD.F32" pelo o uso da TM4C1294NCPDT, arquitetura ARM.

## Lab2 - Tiva board with TivaWare

Questão 6.

- A variável ui32Loop é declarada como **volatile** para que o compilador não
- tente otimizá-la, ignorando o loop **for** utilizado para visualizar o LED
- apagando e acendendo.
