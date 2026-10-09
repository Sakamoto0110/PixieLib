# O layout dos primitivos

O contrato entre o C++ e o C# (`notas.md`, seção 1): cada primitivo tem os mesmos campos, na mesma
ordem e com os mesmos tipos nas duas linguagens, e atravessa P/Invoke sem conversão. No C++, cada
header confere o próprio layout com `static_assert` (tamanho e posição de cada campo), aplicado no
commit `0424940`; no C#, as structs são sequenciais (`[StructLayout(LayoutKind.Sequential)]`), com
os campos nesta ordem, e os testes conferem tamanho e posição pelo `Marshal` e pelo `Unsafe`,
aplicado no commit `c841ae5`.

---

## 1. Os tipos

O tamanho está em bytes, por precisão: `double` (sem sufixo), `float` (`f`), `int32_t` (`i`).

| C++ | C# | Campos, na ordem | double | float | int32 |
|---|---|---|---|---|---|
| `pxPoint` | `PxPoint` | `x`, `y` | 16 | 8 | 8 |
| `pxSize` | `PxSize` | `width`, `height` | 16 | 8 | 8 |
| `pxRect` | `PxRect` | `x`, `y`, `width`, `height` | 32 | 16 | 16 |
| `pxRegion` | `PxRegion` | `x1`, `y1`, `x2`, `y2` | 32 | 16 | 16 |
| `pxPadding` | `PxPadding` | `left`, `top`, `right`, `bottom` | 32 | 16 | 16 |
| `pxVec2` | `PxVec2` | `x`, `y` | 16 | 8 | 8 |
| `pxVec3` | `PxVec3` | `x`, `y`, `z` | 24 | 12 | 12 |
| `pxVec4` | `PxVec4` | `x`, `y`, `z`, `w` | 32 | 16 | 16 |

Os vetores do C++ são os do GLM (`notas.md`, 4.13), nos tipos sem alinhamento extra, os padrão; os
de `float` do C# têm o mesmo layout que o `Vector2`, o `Vector3` e o `Vector4` do `System.Numerics`.

As cores têm uma precisão só:

| C++ | C# | Campos, na ordem | Tamanho |
|---|---|---|---|
| `pxColorRgba` | `PxColorRgba` | `r`, `g`, `b`, `a`, um byte cada | 4 |
| `pxColorHsl` | `PxColorHsl` | `h`, `s`, `l` em `double`, `a` em byte | 32 |

Só a `pxColorHsl` tem enchimento: os três `double` ocupam 24 bytes, o alfa o 25º, e sete bytes
completam o tamanho até um múltiplo de 8, o alinhamento do `double`.

## 2. Regras

- Inteiro é sempre `int32_t` no C++ e `int` no C#; byte é `uint8_t` e `byte`. Nada de `long`, que
  muda de tamanho entre plataformas no C++, nem de `bool`, que não tem layout fixo no P/Invoke.
- Um tipo novo entra nas tabelas acima antes de ganhar código nas duas pontas.

## 3. A semântica que as duas pontas repetem

O layout garante que os dados atravessam; estas regras garantem que eles querem dizer o mesmo.

- `Contains`, no retângulo e na região: as bordas esquerda e de cima estão dentro, a direita e a de
  baixo não (`x >= x1 && x < x2`).
- Região e retângulo: `x2 = x + width`, `y2 = y + height`.
- Entre precisões: implícita quando não perde nada (`float` e `int32` para `double`), explícita
  quando perde. Para `int32`, trunca, satura fora do intervalo e leva NaN a 0 (`notas.md`, 4.12).
- Inteiros: a conta volta ao contrário quando passa do `int32` (`INT32_MAX + 1` é `INT32_MIN`), e a
  divisão por zero é pré-condição. As bordas do retângulo, nas operações dele, são calculadas em 64
  bits.
- Igualdade: o `==` segue o IEEE nas duas pontas (NaN é diferente de tudo); no C#, o `Equals` diz que
  NaN é igual a NaN, para o primitivo servir de chave.
- Operações do retângulo e da região: `Intersect` dá o vazio (todos os campos zero) quando os dois não
  se tocam; quem só encosta não intersecta, e quem não tem área não intersecta nada nem entra no
  `Union`. O `Contains` de um retângulo olha só as bordas.
- HSL para RGBA: a meia unidade arredonda para o par (o padrão do `Math.Round` do C#; no C++,
  `std::nearbyint`), e o valor é preso entre 0 e 255 antes.
- Texto (`ToString`): os campos na ordem, entre parênteses, separados por `, `, como em
  `(1.5, -2)` e `(120, 1, 0.5, 255)`. O ponto decimal é ponto em qualquer cultura. O número sai com
  os menos dígitos significativos que voltam ao mesmo valor, completados com zeros e sem notação
  científica: `100000`, não `1e+05`; o `float` 1e20 sai `100000000000000000000`. NaN sai `nan`,
  qualquer que seja o sinal, e os infinitos, `inf` e `-inf`. Bytes e inteiros saem como números. A
  HSL não tem hex (`notas.md`, 4.2). Conferido: o texto de 44.905 `double` e `float` aleatórios é o
  mesmo nas duas linguagens (`notas.md`, 4.11).
- Vetores: a matemática do C++ é a do GLM, e o C# usa as mesmas fórmulas, na mesma ordem; o `Dot`,
  o `Length`, o `Distance`, o `Cross`, o `Normalize` e o `Lerp` dão os mesmos bits em `double` e em
  `float` (`notas.md`, 4.13). Isso vale com o C++ compilado sem juntar multiplicação e soma numa
  instrução só (FMA): com `-march` de um processador com FMA e `-ffp-contract=fast`, o último bit pode
  mudar. O `Min`, o `Max`, o `Clamp` e o `Abs` com NaN ou -0 seguem cada biblioteca. No vetor
  inteiro, o overflow e a conversão fora do intervalo são indefinidos no C++, porque a conta é do
  GLM, e voltam ao contrário e saturam no C#.
- Hex da cor: `0xRRGGBBAA`, a ordem em que o número é escrito, feita com deslocamento de bits. No
  C#, o `ToArgb()` em `0xAARRGGBB` existe só para o `System.Drawing`.
