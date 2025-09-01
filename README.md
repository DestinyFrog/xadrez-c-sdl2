# Xadrez (C)

Feito em SDL2.

Um jogo de xadrez básico implementado em C com SDL2.
🎯 Funcionalidades

## 🌚 O que foi implementado
> - Tabuleiro 8x8 com todas as peças
> - Sistema de turnos (pretas/brancas)
> - Movimentos válidos para cada peça
> - Seleção visual e highlights

## ⚠️ Limitações Atuais
> - Não verifica xeque ou xeque-mate
> - Não implementa roque ou en passant
> - Sem promoção de peão
> - Interface básica sem menus
> - Sem sistema de captura avançado

## 🚀 Como Executar
```bash
gcc xadrez.c -o xadrez CFLAGS=`sdl2-config --cflags --libs` -lSDL2;
./xadrez;
```

Projeto educativo focado na lógica básica do xadrez.