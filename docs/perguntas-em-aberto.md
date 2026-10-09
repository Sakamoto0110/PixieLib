# Perguntas em aberto

Estado de 08/10/2026. As respondidas saem daqui e vão para `notas.md`, citadas pelo número. Os
números não são reaproveitados. A sugestão, quando há, vem no fim de cada pergunta, e dá para
responder pelo número: "1.7: sim".

## 1. Definição e primeira rodada

Respondidas: da 1.1 à 1.6, a 1.9 e a 1.14 em 03/10, e a 1.7, a 1.8, a 1.10 e a 1.15 em 08/10
(`notas.md`, seções 1, 2 e 4).

- **1.11. `Vec2` e `Vector<n>`** (para a rodada dos vetores). Sugestão: no C++, `pxVec2` é o
  `pxVector<2>` com os membros `x` e `y` nomeados, uma implementação só, como no GLM; no C#, tipos
  concretos do gerador, porque o C# não tem parâmetro genérico inteiro (e `Vector<T>` lá já é o
  vetor SIMD do .NET).
- **1.12. A convenção das matrizes** (para a rodada das matrizes). Sugestão: vetor coluna
  (`M * v`) e armazenamento column-major, o que o GLSL e o `glUniformMatrix4fv` esperam sem
  transpor, e o mesmo da Unity e do GLM. A memória fica igual à do `System.Numerics` (row-major com
  `v * M`), então o repasse em `float` continua de graça, só com a ordem dos operandos trocada por
  dentro. Custo: quem usar o `System.Numerics` direto, ao lado da PixieLib, multiplica na ordem
  contrária.
- **1.13. Projeção e eixo Y** (para a rodada das transformações). No OpenGL, o espaço normalizado
  tem o Y para cima e a profundidade em [-1, 1]; o editor (WinForms e WPF) usa o Y para baixo.
  Sugestão: os primitivos 2D seguem a interface (origem no canto superior esquerdo, Y para baixo), e
  as funções de projeção (`Ortho`, `Perspective`) geram matrizes para o [-1, 1] do OpenGL, com a
  ortográfica 2D já invertendo o Y.

## 2. Os primitivos em C++

Todas respondidas em 08/10, da 2.1 à 2.9 (`notas.md`, seções 4.9 e 4.10).

## 3. Os primitivos em C#

O que eu escolhi ao escrever o lado C# (commit `c841ae5`) sem decisão anterior. O código já está
assim; a sugestão de cada uma é manter.

- **3.1. O namespace** é `PixieLib`, o nome da DLL.
- **3.2. Os testes ficam no repositório**, em `dotnet/PixieLib.Tests`, um console sem framework,
  como os do C++ (2.1), e não num console de fora, como no rework. Rodam no `net10.0`; o `net481` é
  conferido pela compilação.
- **3.3. A ponte com o `System.Drawing`** só existe nos tipos em `double` e na cor, como no
  InteractiveEditor de hoje; os tipos em `float` e `int` ganham a deles quando alguém precisar.
- **3.4. O vocabulário da cor igual ao do C++.** As conversões entre as cores ficam só na
  `PxColorHsl` (`FromRgba` e `ToRgba`); saem o `PxColorArgb.FromHsl` e o `ToHsl`, o `ToHexString`
  e as conversões explícitas com `int`, que não diziam se o número era ARGB ou RGBA. O `ToArgb` e o
  `FromArgb` ficam, para o `System.Drawing` (2.4).
- **3.5. O que é propriedade no C# é método no C++**, com o mesmo nome: `IsEmpty`, `Right`,
  `Width` e `Horizontal` são propriedades no C# e métodos no C++ (`IsEmpty()`), cada um no jeito da
  sua linguagem.

## 4. Os eventos em C++

Respondidas em 08/10, da 4.1 à 4.7 (`notas.md`, seção 4.6); a 4.6 trouxe o `Forward` de volta.

- **4.8. As escolhas do `Forward`** (commit `1f61f6b`), feitas sem decisão anterior. O código já está
  assim; a sugestão é manter.
  - Uma cópia do handler não leva as ligações dele, como não leva as inscrições; o `+=` com um
    handler também não. Uma ligação copiada não seria conhecida pelo alvo, que não teria como
    desfazê-la quando morresse.
  - Um laço é recusado com `false`, sem assert, como o `Remove` que não acha nada.
  - No move, as ligações para o handler movido o seguem. Numa atribuição por move (`b = move(c)`),
    as que iam para o `b` acabam, porque o conteúdo do `b` foi trocado, e as que iam para o `c`
    passam para o `b`; é o que um `std::vector` precisa quando apaga um elemento do meio. Numa
    atribuição por cópia, as que iam para o `b` continuam.
  - O `pxEvent` pode ser ligado a um handler por qualquer um, mas só o dono pode fazê-lo alvo de
    uma ligação, porque ser alvo é ser invocado.

## 5. O 2D

Respondidas em 08/10, da 5.1 à 5.7, as da revisão do 2D (`notas.md`, seção 4.12).

- **5.8. As escolhas das operações** (commit `b863ed4`), feitas sem decisão anterior. O código já está
  assim; a sugestão é manter.
  - Um retângulo sem área (largura ou altura zero ou negativa) não entra no `Union`, para um `Union`
    que começa do vazio não crescer até (0, 0); se nenhum dos dois tem área, o resultado é o primeiro.
  - O `Contains` de um retângulo olha só as bordas: um retângulo sem área encostado na borda direita
    está dentro, como no `System.Drawing`.
  - Mover é `rect + ponto` e `rect - ponto`, e não um `Offset`: no `System.Drawing` o `Offset` muda o
    retângulo e não devolve nada, e um `Offset` que devolvesse um novo seria fácil de chamar e
    descartar.
  - `Deflate` e `Inflate` devolvem um retângulo novo e não prendem o tamanho em zero: uma margem maior
    que o retângulo dá tamanho negativo.
  - O `Center` em `int` arredonda a metade para zero, como a divisão inteira.
  - No C#, `Location` e `Size` têm `set`, como os campos e como no `System.Drawing`.
  - A região ganhou só o item 2 da revisão (`Contains`, `IntersectsWith`, `Intersect` e `Union`); o
    `Center`, o movimento e as margens ficaram só no retângulo.
