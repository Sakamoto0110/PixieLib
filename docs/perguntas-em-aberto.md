# Perguntas em aberto

Estado de 03/10/2026. As respondidas saem daqui e vão para `notas.md`, citadas pelo número. Os
números não são reaproveitados. A sugestão, quando há, vem no fim de cada pergunta, e dá para
responder pelo número: "1.7: sim".

## 1. Definição e primeira rodada

Respondidas em 03/10: da 1.1 à 1.6, a 1.9 e a 1.14 (`notas.md`, seções 1, 2 e 4).

- **1.7. Onde e o gerador** (passagem, 6.2 e 6.7). O C# numa pasta `dotnet/` ao lado da `cpp/`,
  com uma solução própria; o source generator num projeto `PixieLib.Generators`, referenciado como
  analisador, que roda na compilação e não vai junto para quem usa. Sugestão: sim.
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

## 2. Os primitivos em C++

O que eu escolhi ao escrever os primitivos em C++ (commit `0424940`) sem decisão anterior. O código
já está assim; a sugestão de cada uma é manter.

- **2.1. A estrutura.** Só headers, em `cpp/include/pixie/` (incluídos como
  `<pixie/pxPoint.hpp>`), construídos com CMake, em C++20; os testes num executável do CTest, sem
  framework, com um `check.hpp` de 14 linhas.
- **2.2. Os nomes.** Os métodos em PascalCase, iguais aos do C# e aos da PixieLib de 2023
  (`IsEmpty`, `Contains`, `Right`); os campos em minúsculas (`x`, `width`); o modelo com `_t`
  (`pxPoint_t<T>`), como no `pxCorelib.h`, com os apelidos `pxPoint`, `pxPointf` e `pxPointi`.
- **2.3. A HSL com o alfa por último** (`h, s, l, a`), como na RGBA. O C# de hoje tem o alfa
  primeiro e muda junto quando os primitivos forem para a PixieLib.
- **2.4. O hex da cor.** `FromHex` e `ToHex` em `0xRRGGBBAA`, a ordem em que o número é escrito
  (a do `#RRGGBBAA` do CSS e a do `GetColor` do raylib), nas duas pontas; o `ToArgb` do C# fica só
  para o `System.Drawing`.
- **2.5. As precisões fechadas.** O modelo só aceita `double`, `float` e `int32_t`
  (`static_assert`), porque o layout das três é o contrato.
- **2.6. A conversão que perde** é explícita e trunca, como um cast: `pxPointi(pxPoint{2.9, -2.9})`
  dá `(2, -2)`. O C# arredonda na conversão para o `System.Drawing`, mas isso é coisa da ponte com
  ele.
- **2.7. O escalar** do `*` e do `/` é do mesmo tipo do primitivo, então um `pxPointi` divide como
  inteiro (`7 / 2` dá `3`).
- **2.8. O clamp.** A conversão de HSL para RGBA prende o valor entre 0 e 255 antes de arredondar;
  no C# de hoje, uma saturação ou uma luminosidade fora de 0 a 1 estoura o byte. O C# ganha o mesmo
  clamp quando vier.
- **2.9. O texto de todos os primitivos** (commit `c798977`). O formato decidido para a HSL,
  `(h, s, l, a)`, vale para todos: `(x, y)`, `(width, height)`, `(x, y, width, height)`,
  `(x1, y1, x2, y2)`, `(left, top, right, bottom)` e `(r, g, b, a)`. Só o `ToString()`, sem
  `operator<<` nem `std::formatter`: quem usa o `std::format` passa o `ToString()` como argumento.
  Sugestão: manter, e acrescentar o `std::formatter` só se a engine pedir.
