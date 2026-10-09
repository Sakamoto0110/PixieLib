# Perguntas em aberto

Estado de 09/10/2026. As respondidas saem daqui e vão para `notas.md`, citadas pelo número. Os
números não são reaproveitados. A sugestão, quando há, vem no fim de cada pergunta, e dá para
responder pelo número: "1.7: sim".

## 1. Definição e primeira rodada

Respondidas: da 1.1 à 1.6, a 1.9 e a 1.14 em 03/10, a 1.7, a 1.8, a 1.10 e a 1.15 em 08/10, e a
1.11 em 09/10, com os vetores do GLM (`notas.md`, seções 1, 2 e 4).

- **1.12. A convenção das matrizes** (para a rodada das matrizes). Sugestão: vetor coluna
  (`M * v`) e armazenamento column-major, o que o GLSL e o `glUniformMatrix4fv` esperam sem
  transpor, e o mesmo da Unity e do GLM. A memória fica igual à do `System.Numerics` (row-major com
  `v * M`), então o repasse em `float` continua de graça, só com a ordem dos operandos trocada por
  dentro. Custo: quem usar o `System.Numerics` direto, ao lado da PixieLib, multiplica na ordem
  contrária. Com os apelidos do GLM (`notas.md`, 4.13), o C++ já vem nessa convenção.
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

Respondidas: da 5.1 à 5.7 em 08/10, e a 5.8 em 09/10 (`notas.md`, seção 4.12).

## 6. Vetores e matrizes

Respondida: a 6.1 em 09/10, o GLM no C++ (`notas.md`, seção 4.13).

- **6.2. O SIMD nas contas pesadas da `mat4`** (para a rodada das matrizes). O GLM só usa os caminhos
  SIMD dele com os tipos alinhados, que mudam o layout. Sugestão: os tipos da PixieLib ficam sem
  alinhamento extra, e a inversa e o produto de `mat4` ganham funções da PixieLib que copiam para o
  tipo alinhado do GLM, fazem a conta e copiam de volta (a inversa cai de 155 instruções para 68).
- **6.3. As escolhas dos vetores** (commit `947eac7`), feitas sem decisão anterior. O código já está
  assim; a sugestão é manter.
  - O `pixie::math` define `GLM_FORCE_EXPLICIT_CTOR` e `GLM_FORCE_CTOR_INIT` para quem o linka. Com o
    primeiro, toda conversão entre precisões é explícita no C++, até a que não perde nada, que no C#
    é implícita; sem ele, até a que perde seria implícita, contra a regra de 4.4. Com o segundo, um
    vetor começa em zero, como no C#. Na rodada das matrizes, ele faz a `mat4` começar como
    identidade, enquanto o `default` do C# é zero.
  - No C++, o escalar tem de ser do tipo do vetor: `pxVec3 * 2` não compila, é `pxVec3 * 2.0`. É a
    regra do GLM, mais estrita que a do C# e que a da 5.1.
  - No vetor inteiro do C++, o overflow e a conversão fora do intervalo são do GLM: indefinidos,
    uma pré-condição, enquanto o C# volta ao contrário e satura (5.2 e 5.3).
  - No C#, os nomes são os do `System.Numerics`, e o `float` converte implicitamente de e para o
    `Vector2`, o `Vector3` e o `Vector4`. O `Normalize` e o `Lerp` em `float` usam a fórmula do GLM
    sobre o `System.Numerics`, para dar os mesmos bits do C++; o `Min`, o `Max`, o `Clamp` e o `Abs`
    repassam direto, e com NaN ou -0 podem diferir do GLM.
  - Ainda não há conversão entre `pxPoint` ou `pxSize` e `pxVec2`.
