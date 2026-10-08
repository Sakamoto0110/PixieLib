# Perguntas em aberto

Estado de 08/10/2026. As respondidas saem daqui e vão para `notas.md`, citadas pelo número. Os
números não são reaproveitados. A sugestão, quando há, vem no fim de cada pergunta, e dá para
responder pelo número: "1.7: sim".

## 1. Definição e primeira rodada

Respondidas: da 1.1 à 1.6, a 1.9 e a 1.14 em 03/10, e a 1.7 em 08/10 (`notas.md`, seções 1,
2 e 4).

- **1.8. Eventos: como corrigir** (adiada em 03/10, para outra hora). Sugestão: corrigir à mão
  primeiro (a regra dos cinco no `pxCallback`, a lambda guardada por valor, o handler guardando os
  callbacks por valor), e depois trocar o ponteiro cru pelo `unique_ptr`, para usar a ferramenta
  entendendo o que ela faz. O `-=` não se resolve com ponteiro nenhum: é decisão de desenho
  (`diagnostico-2023.md`, seção 1).
- **1.10. Eventos como módulo.** Os eventos são modelos (`pxEventHandler<TArgs...>`), então não
  dá para pô-los numa DLL: só existem em headers, instanciados por quem usa. Sugestão: um módulo
  próprio, com pasta e alvo de build próprios (uma biblioteca só de headers), de que a engine
  depende e que quem só quer os primitivos não puxa. Nenhum binário a mais.
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
- **1.15. Onde fica o legado de 2023** (`notas.md`, 4.7). As tags não saem das sessões (o GitHub
  recusa o push, e nenhuma ferramenta delas cria tag), então guardar o legado em tags depende de o
  void criá-las à mão. Sugestão: desistir das tags e manter a `master` e a `indev` como estão, como
  o registro de 2023: nada se perde e não sobra trabalho manual. Custo: a lista de branches mostra
  as duas ao lado da `main`.

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

