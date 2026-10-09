# Perguntas em aberto

Estado de 09/10/2026. As respondidas saem daqui e vão para `notas.md`, citadas pelo número. Os
números não são reaproveitados. A sugestão, quando há, vem no fim de cada pergunta, e dá para
responder pelo número: "1.7: sim".

## 1. Definição e primeira rodada

Todas respondidas: da 1.1 à 1.6, a 1.9 e a 1.14 em 03/10, a 1.7, a 1.8, a 1.10 e a 1.15 em 08/10,
e da 1.11 à 1.13 em 09/10, com os vetores e as matrizes do GLM (`notas.md`, seções 1, 2 e 4).

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

Respondidas em 09/10: da 6.1 à 6.6 (`notas.md`, seções 4.13 a 4.15).

- **6.7. As escolhas das transformações 2D** (commit `215bdb2`), feitas sem decisão anterior. O
  código já está assim; a sugestão é manter.
  - O `GLM_ENABLE_EXPERIMENTAL` é definido só em volta do include das transformações 2D, e não para
    quem linka o `pixie::math`, como a 6.6 dizia.
  - O `ShearX` e o `ShearY` entram também, com o sentido do GLM: o `ShearX(m, k)` soma `k * x` ao
    `y`.
  - A conversão entre ponto e `vec2` é explícita, como a entre ponto e tamanho. O tamanho não
    converte no `vec2`, e não há função para transformar um ponto: escreve-se `m * pxVec3(p, 1)`.
