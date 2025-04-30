# Isometric Chess Game - C++ OpenGL

Este é um jogo de xadrez 2D desenvolvido em C++ utilizando a biblioteca OpenGL para renderização gráfica. O jogo implementa as regras tradicionais do xadrez, com movimentação de peças, captura e checkmate. A lógica de jogo está dividida em funções responsáveis por desenhar e movimentar as peças, além de verificar a situação de vitória.

## Funcionalidades

- **Movimentação das peças**: O jogo permite mover as peças de acordo com suas regras tradicionais.
- **Captura de peças**: Ao mover uma peça para um espaço ocupado por uma peça adversária, a peça adversária é capturada.
- **Troca de jogador**: O jogo alterna entre dois jogadores (Player 1 e Player 2).
- **Condicional de Vitória**: O jogo termina quando um dos jogadores captura o Rei do adversário.

## Tecnologias Utilizadas

- **C++**: Linguagem de programação principal.
- **OpenGL**: Biblioteca gráfica para renderizar as peças e o tabuleiro de xadrez.

## Estrutura do Código

O código está dividido em várias funções e componentes principais:

### 1. **Funções de Movimentação de Peças**

- `DrawPosPeao()`: Desenha os movimentos possíveis para o Peão.
- `DrawPosCavalo()`: Desenha os movimentos possíveis para o Cavalo.
- `DrawPosRei()`: Desenha os movimentos possíveis para o Rei.
- `DrawPosTorre()`: Desenha os movimentos possíveis para a Torre.
- `DrawPosBispo()`: Desenha os movimentos possíveis para o Bispo.
- `DrawPosRainha()`: Combina as funções da Torre e do Bispo para desenhar os movimentos da Rainha.

### 2. **Movimentação e Captura de Peças**

- `MovePiece()`: Lida com a lógica de movimento e captura de peças, alternando entre os jogadores e verificando a condição de vitória.
- `checkTile()`: Verifica se o jogador selecionou uma peça válida ou se deve mover uma peça para o próximo espaço disponível.

### 3. **Função de Atualização**

- `UpdateGame()`: Atualiza o estado do jogo, incluindo a verificação de peças e a troca de jogadores.
  
### 4. **Renderização**

- `DrawTable()`: Desenha o tabuleiro e as peças no estado atual.
- `display()`: Função principal de renderização usando OpenGL, que limpa a tela e desenha todos os elementos gráficos, incluindo o tabuleiro e as peças.

### 5. **Organização das Peças**

As peças são representadas por um objeto `Object` que contém:
- `type`: Tipo da peça (Peão, Cavalo, Torre, Bispo, Rainha, Rei).
- `player`: Indica o jogador que possui a peça.
- `isActive`: Marca se a peça está ativa ou foi capturada.
- `x`, `y`: Posições da peça no tabuleiro.
- `texture`, `w`, `h`: Textura e dimensões da peça.

### Pré-requisitos

- **C++ Compiler**: Necessário para compilar o código fonte.
- **OpenGL**: Certifique-se de ter o OpenGL instalado no seu sistema.

1. Clone o repositório:
   ```bash
   git clone https://github.com/SEU_USUARIO/chess-game.git
