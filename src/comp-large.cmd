del *.rel
del *.ihx
del *.asm
del *.lst
del *.hex
del *.rst
del *map
del *mem
del *lk
del *sym


sdcc -c -mmcs51 --iram-size 256 --xram-loc 0x00 --xram-size 0x0700 --code-size 61440 --opt-code-size --stack-auto --model-large game.c
sdcc -c -mmcs51 --iram-size 256 --xram-loc 0x00 --xram-size 0x0700 --code-size 61440 --opt-code-size --stack-auto --model-large graph.c
sdcc -c -mmcs51 --iram-size 256 --xram-loc 0x00 --xram-size 0x0700 --code-size 61440 --opt-code-size --stack-auto --model-large lcd.c
sdcc -c -mmcs51 --iram-size 256 --xram-loc 0x00 --xram-size 0x0700 --code-size 61440 --opt-code-size --stack-auto --model-large sys.c


sdcc -mmcs51 --iram-size 256 --xram-size 0x0700 --code-size 61440 --opt-code-size --stack-auto --model-large main.c game.rel graph.rel lcd.rel sys.rel 
packihx main.ihx > flappyBird2.hex