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

Respondidas em 09/10: da 6.1 à 6.3 (`notas.md`, seções 4.13 e 4.14).

- **6.4. O `pixie::math` compilado** (commit `41dbe85`). Para a inversa SIMD da 6.2, o `pixie::math`
  deixou de ser só headers: tem um `.cpp`, que o CMake compila como biblioteca estática. Para quem
  usa pelo CMake, nada muda. A alternativa é voltar a só headers, com a `glm::inverse` comum, e a
  inversa da `pxMat4f` levando o dobro do tempo. Sugestão: manter.
- **6.5. As escolhas das matrizes** (commit `41dbe85`), feitas sem decisão anterior. O código já
  está assim; a sugestão é manter.
  - Só `mat3` e `mat4`, em `double` e `float`: sem matriz de `int` (o GLM não inverte inteiro), sem
    `mat2` e sem as retangulares, como a `mat3x2`, que seria a `Matrix3x2` do `System.Numerics`.
  - No C#, os nomes são os do GLM em PascalCase, e não os do `System.Numerics` (`CreateTranslation`,
    `CreatePerspectiveFieldOfView`...), cujas projeções são para a profundidade em [0, 1]. O
    `Translate(m, v)` multiplica `m` pela translação, como o `glm::translate`; para criar só a
    translação, `Translate(Identity, v)`.
  - O indexador do C# é `m[coluna, linha]`, como o `m[c][r]` do GLM; o do `Matrix4x4` é
    `[linha, coluna]`.
  - O texto da matriz vai coluna por coluna, na ordem da memória.
  - Em `float`, as contas da `PxMat4f` são as fórmulas do GLM escritas no C#, e não as do
    `System.Numerics`, que dão outros bits; só a conversão com o `Matrix4x4` repassa.
- **6.6. As transformações 2D** (para a próxima rodada). Na `mat3`, com o ponto como `(x, y, 1)`:
  translate, rotate e scale 2D. O GLM as tem no `gtx/matrix_transform_2d.hpp`, que é experimental e
  pede o `GLM_ENABLE_EXPERIMENTAL` em quem linka o `pixie::math`. Sugestão: entram na próxima
  rodada, nas duas pontas, junto com a conversão entre `pxPoint` e `pxVec2`, que ainda não existe.
