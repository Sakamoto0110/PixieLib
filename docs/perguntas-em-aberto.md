# Perguntas em aberto

Estado de 08/10/2026. As respondidas saem daqui e vão para `notas.md`, citadas pelo número. Os
números não são reaproveitados. A sugestão, quando há, vem no fim de cada pergunta, e dá para
responder pelo número: "1.7: sim".

## 1. Definição e primeira rodada

Respondidas: da 1.1 à 1.6, a 1.9 e a 1.14 em 03/10, e a 1.7, a 1.8 e a 1.15 em 08/10
(`notas.md`, seções 1, 2 e 4).

- **1.10. Eventos como módulo.** Os eventos são modelos (`pxEventHandler<TArgs...>`), então não
  dá para pô-los numa DLL: só existem em headers, instanciados por quem usa. Sugestão: um módulo
  próprio, com pasta e alvo de build próprios (uma biblioteca só de headers), de que a engine
  depende e que quem só quer os primitivos não puxa. Nenhum binário a mais. Aplicada assim no
  commit `d94ba00` (`cpp/events`, o alvo `pixie::events`); a sugestão é manter.
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

O que eu escolhi ao escrever os eventos (commit `d94ba00`) sem decisão anterior. O código já está
assim; a sugestão de cada uma é manter.

- **4.1. Duplicatas como no C#.** O mesmo callback adicionado duas vezes roda duas vezes, e o `-=`
  tira a última ocorrência. O código de 2023 recusava a segunda.
- **4.2. O que sai durante um `Invoke` não roda mais nele.** No C#, o disparo usa a lista de antes,
  e um callback removido no meio ainda roda. Aqui ele não roda, porque o caso comum de remover
  durante o disparo é o alvo que deixou de existir. O que entra durante um `Invoke` roda a partir do
  próximo, como no C#.
- **4.3. O argumento por valor chega como `const&`** a cada callback, sem uma cópia por callback; um
  callback que pede `T&` num evento de `T` não compila, porque mudaria o valor para o próximo. Quem
  quer que os callbacks mudem o argumento declara o evento com referência (`pxEventHandler<T&>`).
- **4.4. Não é thread-safe.** A engine dispara os eventos no thread dela; usar um handler de mais
  de um thread pede um lock por fora.
- **4.5. O `+=` não devolve nada**, como no C#; quem quer o handle RAII usa o `Subscribe`.
  Devolver o handle do `+=` faria um `e += f;` sozinho desinscrever o `f` na mesma linha.
- **4.6. O que de 2023 ficou de fora:** o `EventArgs`, o encadeamento ao vivo (`chCallback`, que
  guardava um ponteiro para outro handler; agora `a += b` copia os callbacks do `b`, como no C#), o
  modo `NOT_USE_STL`, o `Dump` e o logger. Os nomes seguem os primitivos (2.2): `Count()` e
  `IsEmpty()` no lugar do `GetSize()`.
- **4.7. O buffer de quatro ponteiros** (32 bytes no 64 bits) cabe uma função membro com a instância
  e lambdas com até quatro capturas por referência; o que passa disso vai para o heap.
