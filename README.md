# 🐍 Galaxy Snake: a cobra inteligente

Jogo da cobrinha no terminal do Windows, escrito em **C**, com tema de galáxia e um **modo robô** em que a cobra encontra sozinha a fruta mais próxima usando **busca em largura (BFS)**.

Projeto da faculdade para praticar estruturas de dados: fila, matriz de visitados e reconstrução de caminho.

## Funcionalidades

- **3 modos de jogo:** solo, multiplayer (Dupla Galáctica, duas cobras na mesma tela) e automático (a cobra é pilotada pelo algoritmo BFS)
- **Visualização do algoritmo:** mostra na tela o caminho escolhido e as células que o BFS visitou
- **Galáxia viva:** estrelas, meteoros que caem pela arena e obstáculos
- **Superpoder:** pode ser usado uma vez por partida e dura 8 segundos
- **Níveis:** a velocidade aumenta conforme a pontuação
- **Ranking:** os 5 melhores resultados ficam salvos em arquivo

## Controles

| Tecla | Ação |
|---|---|
| Setas / W A S D | Mover a cobra (no multiplayer, um jogador em cada) |
| Q | Ativar o superpoder |
| P | Pausar e continuar |
| T | Trocar entre modo manual e automático |
| V | Mostrar ou esconder o caminho calculado pelo BFS |
| G | Mostrar ou esconder as células visitadas pelo BFS |

## Como funciona o BFS

A cada passo, o algoritmo parte da cabeça da cobra e explora o tabuleiro em camadas, nas 4 direções, até encontrar a fruta mais próxima:

- **Fila circular** (`bfs_queue`): guarda as próximas células a explorar
- **Matriz de visitados** (`visited`): evita passar duas vezes pela mesma célula
- **Matriz de pais** (`parent`): permite reconstruir o caminho da fruta até a cabeça
- Paredes, obstáculos, meteoros e o próprio corpo da cobra são tratados como células bloqueadas

O código fica em [`src/bfs.c`](src/bfs.c).

## Como executar

Requer Windows e o compilador **GCC** (MinGW). O Windows Terminal é recomendado.

```bash
make
./snake_bfs.exe
```

Sem `make`:

```bash
gcc -Wall -Wextra -std=c11 -O2 -Iinclude src/*.c -o snake_bfs.exe
```

## Estrutura

```
include/   cabeçalhos e configurações (tamanho da arena, velocidade, cores)
src/
  main.c        inicializa o console e roda o loop principal
  game.c        regras do jogo, meteoros e superpoder
  bfs.c         busca em largura
  renderer.c    desenho da arena, painel e menus
  input.c       teclado e troca de modos
  ranking.c     leitura e gravação do ranking
Makefile
```

## O que aprendi

- Aplicar BFS em um problema real e visualizar o algoritmo funcionando
- Organizar um projeto em C em módulos com cabeçalhos
- Controlar o console do Windows (cores, cursor e redesenho sem piscar)
- Persistir dados em arquivo

---
Feito por [Kauany Paula da Silva](https://github.com/Kaukau-gif) durante o curso de Análise e Desenvolvimento de Sistemas.
