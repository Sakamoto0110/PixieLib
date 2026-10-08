# Notas da PixieLib

O que já foi decidido, com a data e o número da pergunta: `1.3` é a pergunta 1.3 de
`perguntas-em-aberto.md`, e `P8.3` é a pergunta 8.3 do rework do InteractiveEditor
(`Sakamoto0110/InteractiveEditor_Rework`, branch `rework-claude`, `docs/notas-modernizacao.md`). O
que ainda é proposta está marcado como **[proposta]**. O que já está no código cita o commit.

A história do projeto está em `historia.md`, o estado do código de 2023 em `diagnostico-2023.md`,
e a passagem que trouxe a parte em C# em `passagem-pixielib.md`.

---

## 1. O que a PixieLib é (1.1, 03/10/2026)

- Uma biblioteca de primitivos e matemática, uma só em duas linguagens, C++ e C#, com o mesmo
  vocabulário (`px` no C++, `Px` no C#) e o mesmo layout de memória.
- **O layout é contrato.** Todo primitivo é uma struct sequencial, só com campos de tamanho fixo,
  e atravessa P/Invoke sem conversão: um `PxRect` do editor em C# chega na PixieEngine como o
  `pxRect` do C++.
- Do 2D ao 3D: pontos, tamanhos, retângulos, regiões, margens, cores, vetores, matrizes e
  transformações, com quatérnio quando o 3D chegar.
- Em `int`, `float` e `double`: modelos com apelidos no C++ (`pxMat<T, N, M>`), e um source
  generator no C# (seção 4.1).
- Só no C++, dois módulos que não são primitivos: os eventos, para a engine (seção 4.6), e o vt,
  para o terminal (o conceito do `pxvt.h` do pxCryptLib; ver `historia.md`).

## 2. O que a PixieLib não é (1.1)

- Não implementa SIMD. Repassa: no C# em `float`, para o `System.Numerics` (seção 4.5); no C++,
  para um backend opcional, a decidir.
- Não chama código nativo. A parte em C# é 100% gerenciada, um pacote NuGet comum; quem faz
  P/Invoke é a PixieEngine, e a PixieLib só garante que os tipos atravessam.
- Não é engine: nada de janela, render, input ou assets.
- Não depende de framework de interface (1.5).

## 3. Quem usa

- **A PixieEngine**, em C++. Começa em 2D e vai para 3D. O alvo final é o OpenGL (03/10); o
  raylib, no máximo, para protótipos de teste.
- **Um editor em C#, no estilo do da Unity** (o InteractiveEditor), que conversa com a engine por
  P/Invoke. Por isso o layout é contrato (seção 1).
- **A NekoLib**, também no .NET Framework, por isso o alvo `net481` (rework 3.9).

Com a PixieLib, quem usa o InteractiveEditor recebe uma segunda DLL, a `PixieLib.dll`: ela existe
para que o editor, a NekoLib e os bindings da engine falem os mesmos tipos (passagem, 6.6).

## 4. Decisões

### 4.1 Nomes e precisões

- **Prefixo** `px` no C++ e `Px` no C# (rework P8.3). Não colide com `System.Drawing`,
  `System.Numerics` nem SkiaSharp (testado no rework).
- **Sufixo** (1.2, 03/10): o nome sem sufixo é `double` nas duas linguagens; `float` e `int` levam
  `f` e `i` (`PxVec2`, `PxVec2f`, `PxVec2i`). No C++, `pxPoint` deixa de ser `int` e passa a
  `double`. Na engine, onde o `float` é o mais usado, um apelido no namespace dela evita o `f` em
  todo lugar. Com o layout como contrato, o mesmo nome quer dizer o mesmo layout nas duas pontas.
- **As três precisões no C#** saem de um modelo só, por um source generator, porque a matemática
  genérica (`INumber<T>`) não existe no `net481` (rework 3.9, testado).
- **Tamanho fixo no C++**: os campos usam `<cstdint>` (`int32_t`, `uint8_t`...). No C#, `int` tem
  sempre 32 bits e `long` 64; no C++, `long` muda com a plataforma (4 bytes no Windows, 8 no Linux
  de 64 bits), e o layout deixaria de bater.

### 4.2 Cores (1.3, 03/10)

- R, G, B, A na memória, um byte cada, nas duas linguagens. O nome descreve a ordem na memória:
  `PxColorRgba` no C# e `pxColorRgba` no C++. Substitui o `PxColorArgb` (rework P8.3), que tinha a
  ordem A, R, G, B.
- É a ordem padrão do OpenGL (`GL_RGBA` com `GL_UNSIGNED_BYTE`), a do `R8G8B8A8_UNORM` do Vulkan
  e do Direct3D, e a do `Color` do raylib. Uma cor, um array de cores de vértice ou os pixels de
  uma textura vão para o OpenGL sem conversão.
- A ordem na memória e a ordem de um número hex são coisas diferentes: reinterpretar R, G, B, A
  como um `uint32` num processador little-endian dá `0xAABBGGRR`. Por isso a conversão de e para
  um inteiro é explícita e feita com deslocamento de bits. Para o `System.Drawing`, `ToArgb()` e
  `FromArgb(int)` continuam em `0xAARRGGBB`.
- `PxColorHsl` continua com o nome (P8.3) e sem conversão implícita para a outra cor (P8.4).
- **A HSL não tem hex nem sintaxe de CSS** (03/10). No editor, ela é um seletor de cor ou um
  conjunto de sliders, nunca texto digitado, então não precisa de parser. Quando precisar de uma
  string, para mostrar o valor, ela é `(h, s, l, a)`, na ordem dos campos: `(120, 1, 0.5, 255)` é
  o verde puro e opaco. A matiz vai de 0 até antes de 360 (360 é o mesmo que 0), e o alfa é um
  byte, de 0 (transparente) a 255 (opaco). Hex só existe na RGBA, e a HSL que precisar dele passa
  por `ToRgba`, explicitamente.
- A RGBA tem texto no mesmo estilo, `(r, g, b, a)`, além do hex (4.10).

### 4.3 Retângulos (1.4, 03/10)

- `PxRect` é `x, y, w, h`; `PxRegion` é `x1, y1, x2, y2`, nessa ordem na memória (a do `RECT` do
  Win32 e a do `PxPadding`). Os dois representam a mesma coisa, de formas diferentes.
- `x2` e `y2` são exclusivos, como no `PxRect.Contains` do InteractiveEditor, que já é
  meio-aberto: `x >= X && x < Right`.

### 4.4 Conversões (1.5, 03/10)

- As regras do rework continuam (P8.5): implícita quando não perde nada, explícita quando perde ou
  pode lançar.
- As conversões com o `System.Drawing` ficam na PixieLib, porque ele existe fora do Windows.
- As do WinForms e do WPF saem: viram métodos de extensão no InteractiveEditor (`rect.ToWpf()` em
  vez de `(Rect)rect`). A PixieLib não tem alvo `-windows`; os alvos são o `net481` e o .NET
  moderno (rework 3.9). Assim a engine e a NekoLib não herdam dependência de interface.

### 4.5 Quais primitivos (1.6, 03/10)

- Todos os do InteractiveEditor vão, menos o `PxDock`, que é vocabulário de interface e fica no
  editor. O `PxPadding` vai, porque é geometria.
- Com os do C++, a lista inicial: pontos, tamanhos, retângulos, regiões, margens, as duas cores,
  vetores e matrizes. Os nomes citados até aqui são exemplos, não a lista final.
- **Repasse ao `System.Numerics`** (rework 3.9): no C#, a versão `float` repassa as operações ao
  `Vector2`, `Vector3`, `Matrix3x2` e `Matrix4x4`, de graça (testado no assembly do JIT); `double`
  e `int` são implementação própria, porque o `System.Numerics` só tem `float`.

### 4.6 Eventos (03/10; 1.8, 1.10 e 4.1 a 4.7, 08/10; commits `d94ba00` e `1f61f6b`)

- Ficam só no C++, para a engine. O C# já tem `event`.
- **O jeito difícil** (1.8, 08/10): sem ponteiro inteligente. Cada handler é dono, por valor, das
  cópias dos seus callbacks. O `shared_ptr` resolveria a posse, mas não o tempo de vida do objeto
  alvo (um ciclo com o dono do evento nunca é liberado), aloca e conta referência atômica a cada
  disparo, e a destruição do callback deixa de ter hora certa.
- Aplicado em `cpp/events/include/pixie/events/`, portado do `pxEvents.hpp` de 2023 (`indev`,
  `b961405`), um conceito por arquivo:
  - `pxCallback<TArgs...>`: o callable por valor, num buffer de quatro ponteiros dentro dele (32
    bytes no 64 bits), sem alocação; maior que isso, no heap, dono dele. A identidade é a do
    delegate do C#: a mesma função, a mesma instância com o mesmo membro, a mesma lambda sem
    captura, ou uma cópia do mesmo `pxCallback`. A lambda com captura nova não é igual a nenhuma,
    como no C#.
  - `pxEventHandler<TArgs...>`: a lista, com `+=`, `-=`, `Invoke` e `operator()`. Durante um
    `Invoke`, o `-=` só marca o callback como morto e o `+=` espera numa lista à parte; a limpeza
    vem quando nenhum `Invoke` roda. Assim a lista nunca se move debaixo do callback que está
    rodando, e quem se remove não faz o próximo ser pulado. Um callback pode destruir o handler,
    como num `delete this`.
  - `pxSubscription`: o handle RAII, que tira o callback quando morre. O handler e as inscrições
    formam uma lista intrusiva: quando o handler morre ou se move, ele avisa cada uma.
  - `pxEvent<TOwner, TArgs...>`: o `event` do C#. De fora, só `+=`, `-=`, `Subscribe` e
    `Forward`; invocar, limpar, contar, copiar, atribuir e ser alvo de um `Forward`, só o dono.
- Um módulo próprio, sem DLL (1.10, 08/10): o alvo `pixie::events`, só headers; quem só quer os
  primitivos linka o `pixie::pixie` e não o puxa.
- **As escolhas do commit `d94ba00`, confirmadas em 08/10** (4.1 a 4.7):
  - Duplicatas como no C# (4.1): o mesmo callback duas vezes roda duas vezes, e o `-=` tira a
    última ocorrência.
  - O que sai durante um `Invoke` não roda mais nele (4.2), ao contrário do C#, porque o caso
    comum é o alvo que deixou de existir. O que entra roda a partir do próximo, como no C#.
  - O argumento por valor chega como `const&` (4.3): um callback que pede `T&` num evento de `T`
    não compila; quem quer mudar o argumento declara o evento com referência.
  - Não é thread-safe (4.4): a engine dispara no thread dela, e o lock, se precisar, fica por fora.
  - O `+=` não devolve nada (4.5), como no C#; o handle RAII sai do `Subscribe`.
  - De 2023 ficaram de fora o `EventArgs`, o `NOT_USE_STL`, o `Dump` e o logger (4.6); o
    `GetSize()` virou `Count()` e `IsEmpty()`. O encadeamento ao vivo voltou como `Forward`.
  - O buffer interno tem quatro ponteiros (4.7).
- **O `Forward`** (4.6, 08/10; commit `1f61f6b`): o encadeamento ao vivo do `chCallback` de 2023,
  agora seguro. `a.Forward(b)` faz o `a` invocar o `b`, com os callbacks que o `b` tiver a cada
  vez, na posição da ligação; `a.Unforward(b)` desfaz a última. O `b` guarda a inscrição da
  ligação, então ela acaba quando qualquer um dos dois morre, e quando o `b` se move ele corrige o
  ponteiro guardado no `a`. Uma ligação que fecharia um laço (`a` para `b` para `a`) é recusada,
  com `false`, e por isso nenhum `Invoke` entra em recursão infinita.
- Os testes (`cpp/tests`, os `px*.test.cpp` dos eventos) passam no g++ 13, também com o
  AddressSanitizer e o UBSan, e no clang++ 18, com `-Werror`. Pegam os três bugs de 2023
  (`diagnostico-2023.md`, seção 1) e falham nas 18 cópias quebradas de propósito: a remoção da
  primeira ocorrência em vez da última, o `+=` mexendo na lista durante o `Invoke`, a limpeza
  durante o `Invoke`, o handler destruído sem avisar o `Invoke`, o `Invoke` que não se desfaz numa
  exceção, a identidade sem a origem ou sem a instância, o move que não destrói a origem, o leak e
  o double free do heap, as inscrições que não seguem o handler, o `pxEvent` invocável de fora e o
  argumento que um callback poderia mudar para o próximo. Os do `Forward` (commit `1f61f6b`) falham em
  mais 5: sem a checagem de laço, sem corrigir o ponteiro no move, a cópia levando a ligação, o
  alvo sem a inscrição e o `Unforward` que tira a ligação de outro. O MSVC ainda não foi testado.
- **[proposta]** As escolhas do `Forward` que não vinham decididas estão na pergunta 4.8.

### 4.7 O recomeço (1.14 e 1.9, 03/10)

- A PixieLib recomeça limpa, no mesmo repositório. O código de 2023 fica nas branches `master`
  (`328b62d`) e `indev` (`b961405`), como estão (1.15, 08/10). O plano das tags `legado-2023` e
  `legado-2023-indev` foi abandonado: o GitHub recusa o push de tags das sessões (403), nenhuma
  ferramenta delas cria tag, e as duas branches já guardam tudo.
- A história nova está na `main`, uma branch órfã, sem commit anterior, cujo primeiro commit é
  esta definição (`7a43f42`). A `main` é a branch padrão do GitHub desde 08/10.
- O código de 2023 não entra na história nova: o que for reaproveitado é portado, citando o
  commit de onde veio.

### 4.8 O escopo desta rodada (03/10)

- Só os primitivos. A correção do código antigo (a matemática de vetores da `VectorMath.h` e da
  vec2math, os eventos) fica para a próxima rodada.
- **[proposta]** Os primitivos desta rodada: ponto, tamanho, retângulo, região, margens e as duas
  cores. Os vetores e as matrizes, com a matemática deles, vão para a próxima, junto com as
  perguntas 1.11 a 1.13.

### 4.9 Os primitivos em C++ (commit `0424940`)

- Aplicado: `pxPoint_t`, `pxSize_t`, `pxRect_t`, `pxRegion_t` e `pxPadding_t` nas três precisões,
  `pxColorRgba` e `pxColorHsl`, cada um no seu header em `cpp/include/pixie/`, com a mesma
  semântica dos primitivos do InteractiveEditor. O layout de cada um está em `layout.md`, conferido
  por `static_assert` no próprio header.
- Os testes (`cpp/tests`, pelo CTest) passam no g++ 13 e no clang++ 18, com `-Wall -Wextra
  -Wpedantic -Wconversion -Werror`, e falham quando se quebra de propósito uma cópia: as bordas do
  `Contains`, o arredondamento e o clamp da HSL, a ordem do hex, o layout e as regras de precisão.
  O MSVC ainda não foi testado.
- As escolhas feitas ao escrever, sem decisão anterior, foram confirmadas em 08/10 (2.1 a 2.8):
  - **Estrutura** (2.1): só headers, em `cpp/include/pixie/` (`<pixie/pxPoint.hpp>`), com CMake e
    C++20; os testes num executável do CTest, sem framework, com o `check.hpp`.
  - **Nomes** (2.2): os métodos em PascalCase, iguais aos do C# e aos da PixieLib de 2023
    (`IsEmpty`, `Contains`, `Right`); os campos em minúsculas (`x`, `width`); o modelo com `_t`
    (`pxPoint_t<T>`), com os apelidos `pxPoint`, `pxPointf` e `pxPointi`.
  - **A HSL com o alfa por último** (2.3): `h, s, l, a`, como na RGBA.
  - **O hex da cor** (2.4): `FromHex` e `ToHex` em `0xRRGGBBAA`, nas duas pontas; o `ToArgb` do C#
    fica só para o `System.Drawing`.
  - **As precisões fechadas** (2.5): o modelo só aceita `double`, `float` e `int32_t`.
  - **A conversão que perde** (2.6) é explícita e trunca, como um cast.
  - **O escalar** (2.7) do `*` e do `/` é do tipo do primitivo: um `pxPointi` divide como inteiro.
  - **O clamp** (2.8): a conversão de HSL para RGBA prende o valor entre 0 e 255 antes de
    arredondar.

### 4.10 O texto dos primitivos (08/10, commit `c798977`)

- A engine vai consumir os primitivos, então o texto entra no C++ agora, e não quando alguém
  precisar (08/10).
- Aplicado: um `ToString()` em cada primitivo, todos no formato da HSL (4.2): os campos na ordem,
  entre parênteses, separados por `, `. Os números saem pelo `std::to_chars`, que ignora a cultura,
  na forma mais curta que volta ao mesmo valor e sem notação científica (`layout.md`, seção 3).
- Os testes passam no g++ e no clang++ e falham quando se quebra de propósito a notação, o
  separador, quem escreve o número, o tamanho do buffer, os bytes saindo como caractere ou a ordem
  da HSL. Este container só tem a cultura C, então a independência de cultura vem da garantia do
  `std::to_chars`, não de um teste com vírgula decimal.
- O mesmo formato vale para todos os primitivos, só com o `ToString()`, sem `operator<<` nem
  `std::formatter` (2.9, 08/10); o `std::formatter` entra se a engine pedir.

- O texto mudou no commit `d28ddd9`, para bater com o C#: a notação fixa do `std::to_chars` escolhe,
  entre as formas mais curtas, os dígitos mais próximos do valor exato, e o `float` 1e20 saía
  `100000002004087734272`, onde o C# escreve `100000000000000000000`. Agora o número sai da forma
  científica mais curta, com a vírgula decimal movida à mão, o mesmo algoritmo dos dois lados.

### 4.11 O lado C# (1.7, 08/10; commit `c841ae5`)

- O C# fica numa pasta `dotnet/`, ao lado da `cpp/`, com uma solução própria (`PixieLib.slnx`).
- As três precisões saem de um source generator, o projeto `PixieLib.Generators`, referenciado como
  analisador: roda na compilação e não vai junto para quem usa a `PixieLib.dll`. Cada
  `Templates/*.template.cs` vira três tipos; no modelo, `__S__` é o sufixo e `__T__` o tipo, e as
  diferenças ficam em blocos `#if PX_DOUBLE`, `PX_FLOAT`, `PX_INT` e `PX_FLOATING`.
- Aplicado: `PxPoint`, `PxSize`, `PxRect`, `PxRegion` e `PxPadding` nas três precisões, saídos dos
  modelos, e `PxColorRgba` e `PxColorHsl` escritos à mão, para `net481` e `net10.0`. Os campos são
  os de `layout.md`, e a semântica é a do C++: as conversões entre precisões, o `Contains`
  meio-aberto, a HSL com o alfa por último, o arredondamento para o par e o clamp, o hex em
  `0xRRGGBBAA` e o texto.
- Os testes (`dotnet/PixieLib.Tests`) usam os mesmos valores dos testes do C++, mais o layout
  (`Marshal` e `Unsafe`) e a ponte com o `System.Drawing`, e o texto roda numa cultura com vírgula
  decimal (pt-BR). Passam no `net10.0`; o `net481` é conferido pela compilação, porque não roda no
  Linux. Falham quando se quebra de propósito uma cópia: as bordas do `Contains`, o arredondamento,
  o clamp, o layout, a ordem do hex, as regras de precisão, o texto, a cultura e o gerador.
- O texto de 44.905 `double` e `float` aleatórios, comparado linha a linha, é o mesmo no C++ e no
  C#.
- **[proposta]** As escolhas que não vinham decididas estão nas perguntas 3.1 a 3.5.

## 5. Consequências, ainda não aplicadas

- **No InteractiveEditor**, numa sessão do rework depois desta (passagem, 6.8): ele passa a usar a
  PixieLib e apaga os próprios primitivos, menos o `PxDock`; o namespace muda de
  `InteractiveEditor.Primitives` para `PixieLib`. O que muda para quem os usa: `PxColorArgb` vira
  `PxColorRgba`, e a `PxColorHsl` passa o alfa para o fim, nos construtores também; as conversões
  entre as cores ficam só na `PxColorHsl` (`FromRgba` e `ToRgba`); saem o `ToHexString` e as
  conversões explícitas da cor com `int` (o `ToArgb` e o `FromArgb` ficam); o `ToString` passa ao
  formato da 4.10; e as conversões do WinForms e do WPF viram métodos de extensão no editor (4.4).

## 6. Como commitar

- Author e committer: `Rafael Sakamoto <rafael.sakamoto1@hotmail.com>`, sem assinatura
  (passagem, seção 1).
- **Nenhuma marca de assistente** (03/10): sem trailers de coautoria nem de sessão, e sem menção
  a quem ajudou. Substitui o trecho da passagem (seção 1) que pedia os trailers.
- **A branch é a `main`** (08/10). Nenhuma branch com `claude/` no nome; se um trabalho pedir uma
  branch própria, o nome é do void.
- Mensagens em inglês, com prefixo: `(refactor)`, `(docs)`, `(fix)`, `(feat)`. Mudança de código e
  atualização das notas em commits separados, o de `(docs)` citando o hash do outro (passagem).
