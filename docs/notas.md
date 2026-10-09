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

- Não implementa SIMD nem inventa a matemática de vetores e matrizes. Repassa: no C++, para o GLM
  (6.1, 09/10; seções 4.13 e 4.14); no C# em `float`, para o `System.Numerics` quando ele dá os
  mesmos bits que o GLM (seção 4.5), e no resto o C# copia as fórmulas do GLM. "Reinventar a roda,
  mas não a madeira" (09/10).
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
  e `int` são implementação própria, porque o `System.Numerics` só tem `float`. Nos vetores, repassa
  quando dá os mesmos bits que o GLM (4.13); nas matrizes, só a conversão com o `Matrix4x4` (4.14),
  porque as contas dele dão outros bits.

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
  exceção, a identidade sem a origem ou sem a instância, o move que não destrói a origem, o leak e o
  double free do heap, as inscrições que não seguem o handler, o `pxEvent` invocável de fora e o
  argumento que um callback poderia mudar para o próximo. Os do `Forward` (commit `1f61f6b`) falham
  em mais 5: sem a checagem de laço, sem corrigir o ponteiro no move, a cópia levando a ligação, o
  alvo sem a inscrição e o `Unforward` que tira a ligação de outro. No MSVC, passam desde 09/10,
  pelo CI (4.16).
- As escolhas do `Forward` que não vinham decididas (4.8) foram aceitas em 09/10: a cópia de um
  handler e o `+=` não levam as ligações dele, o laço é recusado com `false`, no move as ligações
  seguem o handler, e só o dono pode fazer do `pxEvent` o alvo de uma ligação.

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
  perguntas 1.11 a 1.13. Os vetores entraram em 09/10 (seção 4.13).

### 4.9 Os primitivos em C++ (commit `0424940`)

- Aplicado: `pxPoint_t`, `pxSize_t`, `pxRect_t`, `pxRegion_t` e `pxPadding_t` nas três precisões,
  `pxColorRgba` e `pxColorHsl`, cada um no seu header em `cpp/include/pixie/`, com a mesma
  semântica dos primitivos do InteractiveEditor. O layout de cada um está em `layout.md`, conferido
  por `static_assert` no próprio header.
- Os testes (`cpp/tests`, pelo CTest) passam no g++ 13 e no clang++ 18, com `-Wall -Wextra
  -Wpedantic -Wconversion -Werror`, e falham quando se quebra de propósito uma cópia: as bordas do
  `Contains`, o arredondamento e o clamp da HSL, a ordem do hex, o layout e as regras de precisão.
  No MSVC, passam desde 09/10, pelo CI (4.16).
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
  - **A conversão que perde** (2.6) é explícita e trunca, como um cast. Fora do intervalo do `int`,
    satura, e NaN vira 0 (5.3, seção 4.12).
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
  C#. No `net481` também, desde 09/10: os dígitos saem do valor exato, e não do runtime (4.16).
- As escolhas que não vinham decididas (3.1 a 3.5) foram aceitas em 09/10: o namespace `PixieLib`,
  os testes em `dotnet/PixieLib.Tests`, a ponte com o `System.Drawing` só no `double` e na cor, as
  conversões entre as cores só na `PxColorHsl`, sem o `ToHexString` nem as conversões com `int`, e
  o que é propriedade no C# como método no C++, com o mesmo nome.

### 4.12 A revisão do 2D (5.1 a 5.7, 08/10; commit `b863ed4`)

A revisão dos primitivos 2D achou pontos em que o C++ e o C# diziam coisas diferentes, e faltavam
operações. As respostas, todas aplicadas nas duas pontas:

- **O escalar** (5.1): o `*` e o `/` só aceitam um escalar que o C# converteria implicitamente para
  o tipo do primitivo (`pxIsImplicitScalar`, em `pxPrecision.hpp`). `pxPointi * 0.5` não compila
  mais no C++, como já não compilava no C#; antes virava `pxPointi * 0`. `pxPoint * 2` continua.
- **O overflow de inteiro** (5.2): em `int32_t`, a conta volta ao contrário, como no C# (`unchecked`,
  o padrão), em vez de ser comportamento indefinido; no C++ ela passa por `uint32_t` (`pxAdd`,
  `pxSub`, `pxMul`, `pxNeg`). O `Right()` de um retângulo que passa do fim também volta ao contrário;
  o `Contains` e as operações novas do retângulo calculam as bordas em 64 bits, e valem para qualquer
  retângulo.
- **A conversão para `int`** (5.3): trunca, satura fora do intervalo e leva NaN a 0, o que o .NET faz
  num cast desde o 9. Está escrita à mão nas duas pontas (`pxConvert` no C++, `PxConvert.ToInt32` no
  C#), porque no C++ o cast fora do intervalo é indefinido e o `net481` dá `int.MinValue`. Vale para
  as conversões entre precisões, para a ponte com o `System.Drawing` e, com NaN, para a HSL.
- **A divisão inteira por zero** (5.4) é pré-condição, como no próprio `int`: indefinida no C++,
  exceção no C#.
- **O `Equals` do C#** (5.5) compara cada campo com o `Equals` dele: um NaN é igual a si mesmo, e o
  primitivo funciona como chave de `Dictionary` e `HashSet`. O `==` continua IEEE, como o
  `operator==` do C++. O hash (`PxHash.Of`) trata 0 e -0, e todos os NaN, como iguais, também no
  `net481`.
- **A ponte com o `Rectangle`** (5.6) arredonda as bordas, não os campos: a largura é o que o
  retângulo cobre. `(0.5, 0, 0.5, 1)` vira largura 1, de 0 a 1, e não 0.
- **As operações** (5.7), nas duas pontas, com os nomes do C# e o jeito de cada linguagem (3.5):
  - Retângulo: `Location` e `Size`, `Center`, `+` e `-` com um ponto (move, o tamanho fica),
    `Contains` de um retângulo, `IntersectsWith`, `Intersect`, `Union`, e `Deflate` e `Inflate` com
    uma margem.
  - Região: `Contains` de uma região, `IntersectsWith`, `Intersect` e `Union`, com as mesmas regras.
- Os testes passam no g++ 13 (também com o AddressSanitizer e o UBSan) e no clang++ 18, com
  `-Werror`, e no .NET 10; o `net481` é conferido pela compilação. Falham quando se quebra de
  propósito uma cópia: a saturação, o NaN, o `Contains` sem 64 bits, o `Union` e o `IntersectsWith`
  sem a regra da área, o `Center`, a soma sem `uint32_t` (pelo UBSan), o NaN da HSL no C++, o escalar
  recusado, o `Equals` com `==` e a ponte que arredonda os campos. O NaN do `PxConvert`, o da HSL no C#
  e o `PxHash.Of` só mudam algo no `net481`, onde os testes não rodam: no .NET 10 o cast e o hash já
  fazem o mesmo.
- As escolhas das operações feitas sem decisão anterior foram aceitas em 09/10 (5.8): um retângulo
  sem área não entra no `Union`; o `Contains` de um retângulo olha só as bordas; mover é `rect + ponto`,
  e não um `Offset`; `Deflate` e `Inflate` não prendem o tamanho em zero; o `Center` inteiro arredonda
  para zero; no C#, `Location` e `Size` têm `set`; a região só tem `Contains`, `IntersectsWith`,
  `Intersect` e `Union`.

### 4.13 Os vetores (6.1 e 1.11, 09/10; commit `947eac7`)

- **O GLM faz a matemática no C++** (6.1, 09/10): é a matemática do OpenGL, pronta e testada, e a
  PixieLib não refaz a inversa, o quatérnio e a projeção à mão. Fica fixo na 1.0.3, baixado pelo
  CMake (`FetchContent`, só os headers), ou achado pelo `find_package` com `PIXIE_SYSTEM_GLM`.
- **Um módulo próprio**, o `pixie::math`, como os eventos (1.10): quem só quer os primitivos desliga
  o `PIXIE_MATH` e não baixa o GLM.
- **Os vetores são apelidos dos do GLM** (09/10), o que responde à 1.11: `pxVec2`, `pxVec3` e
  `pxVec4` são `glm::dvec2`, `glm::dvec3` e `glm::dvec4`, com `f` e `i` para `float` e `int32_t`
  (4.1), e a API é a do GLM (`glm::dot`, `glm::cross`, `glm::length`, `glm::normalize`,
  `glm::mix`...). O texto sai de uma função, `pxToString(v)`, no formato de 4.10. Os `static_assert`
  do `pxVec.hpp` conferem o layout: os tipos alinhados do GLM (`GLM_FORCE_DEFAULT_ALIGNED_GENTYPES`)
  fariam o `vec3` ter 16 bytes, e o build para.
- **O SIMD** (medido em 09/10, `reviews/vec3-simd.md` na pasta do projeto): num `vec3` o compilador
  já soma com uma instrução empacotada, e o GLM padrão não usa os caminhos SIMD dele, que só existem
  nos tipos alinhados. O ganho real é na `mat4` (a inversa cai de 155 instruções para 68), assunto da
  6.2, na rodada das matrizes.
- **No C#**, `PxVec2`, `PxVec3` e `PxVec4` saem do gerador, com o layout dos vetores do GLM e os
  nomes do `System.Numerics` (`Dot`, `Cross`, `Length()`, `Normalize`, `Lerp`, `Min`, `Max`,
  `Clamp`, `Abs`, `Zero`, `One`, `UnitX`...). Em `float`, repassam ao `System.Numerics` e convertem
  de e para o `Vector2`, `Vector3` e `Vector4` implicitamente; em `double` e `int`, são código próprio,
  com as fórmulas do GLM. O inteiro não tem `Dot`, `Cross`, `Length` nem `Normalize`, como no GLM.
- **Os mesmos bits nas duas pontas**: em `double` e em `float`, o `Dot`, o `Length`, o `Distance`, o
  `Cross`, o `Normalize` e o `Lerp` dão o mesmo número que o GLM, conferido em 20.000 vetores
  aleatórios. Para isso, o `Normalize` e o `Lerp` em `float` usam a fórmula do GLM sobre o
  `System.Numerics` (multiplicar por 1 / comprimento; `a * (1 - t) + b * t`): os do `System.Numerics`
  dariam um bit de diferença em metade dos casos.
- **Corrigido em 09/10** (commit `031d701`): a conferência acima foi feita no `vec3`. No `vec4`, o
  GLM soma o produto escalar em pares, `(x + y) + (z + w)`, como o `Vector4.Dot` do `float`, e o
  `PxVec4` em `double` somava em sequência: o `Dot`, o `Length`, o `Distance` e o `Normalize` saíam
  com um bit de diferença em 29% de 20.000 vetores. Agora nenhum difere. O GLM compilado pelo MSVC
  soma em sequência (`func_geometric.inl`), então lá esses quatro podem diferir do C# no último bit;
  o teste do C++ espera isso no MSVC, e passa lá desde 09/10 (4.16).
- Os testes passam no g++ 13 (também com o AddressSanitizer e o UBSan) e no clang++ 18, com
  `-Werror`, e no .NET 10, e desde 09/10 no MSVC e no `net481`, pelo CI (4.16); o `PIXIE_SYSTEM_GLM`
  não foi testado. Falham quando se quebra de propósito uma cópia: sem o `GLM_FORCE_EXPLICIT_CTOR`
  ou o `GLM_FORCE_CTOR_INIT`, o apelido com a precisão errada, a ordem do texto, a fórmula do
  `Normalize` e do `Lerp` em `double` e em `float`, o `Cross`, o `Equals`, a ordem dos campos e o
  `Min`. A saturação na conversão para `int` só muda algo no `net481`.
- As escolhas que não vinham decididas (6.3) foram aceitas em 09/10: as conversões explícitas e o
  começo em zero (`GLM_FORCE_EXPLICIT_CTOR` e `GLM_FORCE_CTOR_INIT`), o escalar do tipo do vetor no
  C++, o overflow do vetor inteiro como pré-condição no C++, os nomes do `System.Numerics` no C# e a
  falta de conversão entre `pxPoint` ou `pxSize` e `pxVec2`.

### 4.14 As matrizes (1.12, 1.13, 6.2 e 6.3, 09/10; commit `41dbe85`)

- **A convenção** (1.12, 09/10): o vetor é coluna e multiplica à direita (`M * v`), e a memória é
  column-major, o que o GLSL e o `glUniformMatrix4fv` esperam sem transpor. Em `A * B * v`, o `B`
  vem primeiro. A memória é a mesma do `Matrix4x4` do `System.Numerics`, que multiplica o vetor à
  esquerda: a `PxMat4f` converte de e para ele implicitamente, os mesmos bytes são a mesma
  transformação (`Vector4.Transform(v, m)` é `m * v`), e o produto de dois `Matrix4x4` fica na ordem
  contrária.
- **O eixo Y e as projeções** (1.13, 09/10): o 2D segue a interface, com a origem no canto superior
  esquerdo e o Y para baixo; as projeções são as do OpenGL, com a mão direita e a profundidade em
  [-1, 1]. A `pxOrtho2D(largura, altura)` (`PxMat4.Ortho2D` no C#) é o `glm::ortho(0, largura,
  altura, 0)`: leva o (0, 0) ao canto superior esquerdo da tela, o (-1, 1). Com o Y para baixo, um
  ângulo positivo em torno do Z gira no sentido horário na tela.
- **No C++**, `pxMat3` e `pxMat4` são apelidos de `glm::dmat3` e `glm::dmat4`, com `f` para `float`,
  no `pixie/math/pxMat.hpp`; não há matriz de `int`. A matemática é a do GLM (`glm::transpose`,
  `glm::determinant`, `glm::translate`, `glm::rotate`, `glm::scale`, `glm::lookAt`, `glm::ortho`,
  `glm::perspective`), e o texto sai do `pxToString(m)`, coluna por coluna. Com o
  `GLM_FORCE_CTOR_INIT` (6.3), uma matriz começa como a identidade.
- **O SIMD da inversa** (6.2, 09/10): a ideia de copiar para o tipo alinhado do GLM não basta. A
  inversa SIMD do GLM (`glm_mat4_inverse`) só existe com o `GLM_FORCE_INTRINSICS`, e esse define
  tira o `constexpr` de todos os tipos do GLM no programa inteiro (um `constexpr pxVec3` deixa de
  compilar no g++ e no clang). Por isso ela fica sozinha num arquivo compilado,
  `cpp/math/src/pxMat.cpp`, que só inclui as funções SIMD do GLM e recebe `float*`, sem nenhum tipo
  do GLM; o `pixie::math` passou a ser uma biblioteca estática com esse arquivo. A `pxInverse` é a
  `glm::inverse` e, no x86-64, leva a `pxMat4f` por esse caminho: os mesmos bits (20.000 matrizes),
  cerca de 11 ns contra 25 ns. O produto de `mat4` não precisa disso: o compilador já o vetoriza. A
  `dmat4` e a `mat3` não têm versão SIMD no GLM.
- **No C#**, `PxMat3` e `PxMat4` em `double` e `float` saem do gerador, que agora aceita uma linha
  `// PX_PRECISIONS: double float` para pular o `int`. As colunas são `PxVec3` ou `PxVec4`; os
  nomes são os do GLM em PascalCase (`Transpose`, `Inverse`, `Determinant`, `Translate`, `Rotate`,
  `Scale`, `LookAt`, `Ortho`, `Perspective`, mais o `Ortho2D`), porque as funções de criação do
  `System.Numerics` são para o vetor à esquerda e a profundidade em [0, 1]. O indexador é
  `m[coluna]` e `m[coluna, linha]`, como o `m[c][r]` do GLM. O `default` é zero, e a identidade é
  `Identity`.
- **Os mesmos bits nas duas pontas**: as fórmulas do C# são as do GLM, na mesma ordem (o `M * v` da
  `mat4` soma as colunas em pares, o produto e a `mat3` somam em sequência, a inversa multiplica por
  1 / determinante). Em `float`, elas não repassam ao `System.Numerics`: o produto, o `Invert` e o
  `Transform` dele diferem do GLM em quase todos os casos (4.997, 5.000 e 4.653 de 5.000 matrizes).
  Conferido: 18 operações em 5.000 matrizes aleatórias, em `double` e `float`, dão os mesmos bits no
  g++, no clang e no .NET 10. O seno, o cosseno e a tangente vêm da biblioteca C da plataforma nas
  duas pontas (`MathF` no .NET); no `net481`, o `float` passa pelo `double` e pode mudar no último
  bit, e o cosseno do .NET Framework é outro (4.16).
- Os testes passam no g++ 13 (também com o AddressSanitizer e o UBSan, e com `-O2`) e no clang++ 18,
  com `-Werror`, e no .NET 10, e desde 09/10 no MSVC e no `net481`, pelo CI (4.16). Falham quando se
  quebra de propósito uma cópia: a ordem das somas do `M * v`, do produto, do determinante, da
  inversa e da `mat3`, a divisão no lugar da multiplicação na inversa, um sinal do `Rotate`, do
  `LookAt`, do `Ortho` e do `Perspective`, os argumentos do `Ortho2D` nas duas pontas, o indexador,
  a ponte com o `Matrix4x4`, o `Equals`, a ordem das colunas e do texto, o apelido com a precisão
  errada e a inversa SIMD.
- As escolhas que não vinham decididas foram aceitas em 09/10: o `pixie::math` compilado, por causa
  da inversa SIMD (6.4), e as escolhas das matrizes (6.5): só `mat3` e `mat4`, sem `int`; os nomes
  do GLM no C#; o indexador `m[coluna, linha]`; o texto coluna por coluna; e as contas da `PxMat4f`
  com as fórmulas do GLM, com só a conversão repassando ao `Matrix4x4`.

### 4.15 As transformações 2D (6.6, 09/10; commit `215bdb2`)

- **Na `mat3`**, com o ponto como `(x, y, 1)`. No C++, são as do GLM (`gtx/matrix_transform_2d`),
  que o `pxMat.hpp` já inclui: `glm::translate(m, vec2)`, `glm::rotate(m, ângulo)`,
  `glm::scale(m, vec2)`, `glm::shearX` e `glm::shearY`. No C#, `PxMat3.Translate`, `Rotate`,
  `Scale`, `ShearX` e `ShearY`, com as fórmulas do GLM. Cada uma é `m` vezes a transformação, então
  em `T * R * S * p` o `S` vem primeiro; com o Y para baixo, um ângulo positivo gira no sentido
  horário na tela. O `ShearX(m, k)` do GLM soma `k * x` ao `y`, e o `ShearY(m, k)` soma `k * y` ao
  `x`.
- **O `GLM_ENABLE_EXPERIMENTAL`**: o GLM pede esse define para as extensões experimentais, como
  esta. O `pxMat.hpp` o define só em volta desse include, e não para quem linka o `pixie::math`,
  como a 6.6 dizia: assim os outros headers experimentais continuam pedindo o define.
- **Ponto e `vec2`**: guardam os mesmos dados na mesma precisão, então um converte no outro,
  explicitamente, como o ponto e o tamanho (4.9): `pxToVec2(ponto)` e `pxToPoint(vetor)` no C++, que
  não pode dar construtor a um apelido, e `(PxVec2)ponto` e `(PxPoint)vetor` no C#.
- **Os mesmos bits nas duas pontas**: as cinco transformações e o ponto transformado, em 5.000
  matrizes aleatórias, em `double` e `float`, no g++ e no .NET 10.
- Os testes passam no g++ 13 (também com o AddressSanitizer e o UBSan) e no clang++ 18, com
  `-Werror`, e no .NET 10. Falham quando se quebra de propósito uma cópia: a ordem da soma do
  `Translate`, o sinal do `Rotate`, os eixos do `Scale` e dos dois `Shear`, a ordem do produto no
  `Shear`, e a ordem dos campos nas duas conversões, nas duas linguagens.
- As escolhas que não vinham decididas (6.7) foram aceitas em 09/10: o `GLM_ENABLE_EXPERIMENTAL` só
  em volta do include, os dois `Shear` com o sentido do GLM, a conversão explícita entre ponto e
  `vec2`, sem conversão do tamanho e sem função para transformar um ponto.

### 4.16 O primeiro deploy (7.1 a 7.4, 09/10; commits `5693b4c` a `5777471`)

As perguntas 7.1 a 7.4 foram feitas na conversa, e não em `perguntas-em-aberto.md`; o void aceitou
as sugestões em 09/10 ("ok"), com a 4.8.

- **A versão 0.1.0** (7.1): abaixo da 1.0, a API ainda pode mudar. O número fica no `Version` do
  `dotnet/PixieLib/PixieLib.csproj`, e uma versão nova começa subindo ele.
- **MIT** (7.2), no `LICENSE`, com o copyright no nome do git (seção 6). O `README.md`, em inglês,
  diz o que a PixieLib é, como pegar o pacote e o C++, e quando os bits batem; os exemplos dele
  foram compilados e rodados, o do C++ pelo `FetchContent` deste repositório.
- **A publicação** (7.3): o pacote `PixieLib` do nuget.org, da conta Nekovih, sai do
  `.github/workflows/publish.yml`, que o void dispara na `main` (Actions, Publish, Run workflow).
  Ele roda o CI, empacota, publica e cria o release com a tag `v` mais a versão; as sessões não
  criam tag (4.7), e o workflow cria. O nuget.org deixa o workflow entrar por trusted publishing: a
  política da conta nomeia o `publish.yml`, e não há chave no repositório. Uma tag que já existe
  para tudo antes de publicar, e uma versão no nuget.org não se apaga, só se esconde. O pacote leva
  as DLLs de `net10.0` e `net481`, o README e os símbolos (`snupkg`), sem dependência.
- **O C++ não tem pacote**: quem usa pega pelo `FetchContent`, com a tag e `SOURCE_SUBDIR cpp`
  (README), e como subprojeto os testes não entram.
- **O CI** (7.4): o `.github/workflows/ci.yml` roda a cada push na `main` e em PR. O C++, em
  Release, no g++ com o AddressSanitizer e o UBSan, no clang++ e no MSVC; o C#, no .NET 10 no Linux
  e no Windows, onde roda também no .NET Framework (`net481`), e empacota. Para isso os testes do C#
  ganharam o alvo `net481`: o atributo `CallerArgumentExpression` declarado para o .NET Framework, o
  `System.Memory` só nos testes (o `Unsafe` e o `MemoryMarshal` do layout), e o `Layout<T>` pedindo
  `unmanaged`. As checagens de texto das matrizes mostram, quando falham, o texto e os bits de cada
  número (`Check.Text`, commit `02554ad`), porque o .NET Framework só roda no CI.
- **O que o CI achou**, nas primeiras vezes que o MSVC e o `net481` rodaram:
  - O clang junta multiplicação e soma (FMA) por padrão (`-ffp-contract=on`), e com otimização faz
    isso até num processador sem FMA, nas expressões constantes que calcula ao compilar: em Release,
    cinco checagens das matrizes falhavam. Os testes compilam com `-ffp-contract=off` no GCC e no
    clang (commit `5693b4c`), e o `layout.md` diz isso a quem usa.
  - O `pxInverse` deixava inalcançável o `glm::inverse` de uma `mat4` em `float`, e o MSVC avisa
    (C4702) em quem compila o header com `/W4`; foi para um `else` (commit `46de4ce`). No mesmo
    commit, o `log` dos testes dos eventos, que no MSVC conflitava com o da biblioteca C, virou
    `trace`.
  - No `net481`, o texto de um número: o `ToString` de um `double` perto do maior lançava
    `OverflowException`, porque o parser do .NET Framework lança onde o .NET dá infinito, e o texto
    de alguns números saía com outro último dígito, porque a formatação dele não arredonda sempre
    para o mais próximo o 16º e o 17º dígito de um `double`, nem o 9º de um `float`
    (-0.5400000000000002 no lugar de -0.5400000000000001, que voltam ao mesmo `double`). O `PxText`
    agora gera os dígitos sozinho, com `BigInteger`, do valor exato: para cada número de dígitos, o
    valor arredondado (metade para o par), até um que volte ao mesmo valor, decidido sem parser
    (commits `46de4ce`, `829983b` e `63a49e1`). No .NET 10, o texto é o mesmo de antes em 1,2 milhão
    de `double` e `float`, com as potências de dois, os subnormais e os extremos.
  - O `Math.Cos(Math.PI / 2)` do .NET Framework é 6.123031769111886e-17, e não o
    6.123233995736766e-17 do .NET, da glibc e do MSVC. A trigonometria vem de cada plataforma
    (`layout.md`, 3), e a PixieLib não escreve a sua: no `net481`, os testes esperam o valor do .NET
    Framework (commit `5777471`).

## 5. Consequências

- **No InteractiveEditor** (aplicado em 09/10, no commit `424f6d0` da `rework-claude` dele, que
  entrou na `main` dele pelo PR 3; passagem, 6.8): ele passa a usar a PixieLib e apaga os próprios
  primitivos, menos o `PxDock`; o namespace muda de `InteractiveEditor.Primitives` para `PixieLib`.
  O que muda para quem os usa: `PxColorArgb` vira `PxColorRgba`, e a `PxColorHsl` passa o alfa para
  o fim, nos construtores também; as conversões entre as cores ficam só na `PxColorHsl` (`FromRgba`
  e `ToRgba`); saem o `ToHexString` e as conversões explícitas da cor com `int` (o `ToArgb` e o
  `FromArgb` ficam); o `ToString` passa ao formato da 4.10; e as conversões do WinForms e do WPF
  viram métodos de extensão no editor (4.4). Um ponto que o compilador não pega: o construtor de
  quatro `byte` da `PxColorArgb` recebe o alfa primeiro, e o da `PxColorRgba` por último, então um
  `new(255, 0, 128, 255)` continua compilando com a cor trocada; o da `PxColorHsl` também, quando o
  último argumento é um inteiro que cabe num `byte` (conferido em 09/10 numa cópia do editor, onde o
  único caso é o `Gadget.Fill` do DemoObjects). O editor usa a cópia em `external/PixieLib` até
  trocar pelo pacote (4.16).

## 6. Como commitar

- Author e committer: `Rafael Sakamoto <rafael.sakamoto1@hotmail.com>`, sem assinatura
  (passagem, seção 1).
- **Nenhuma marca de assistente** (03/10): sem trailers de coautoria nem de sessão, e sem menção
  a quem ajudou. Substitui o trecho da passagem (seção 1) que pedia os trailers.
- **A branch é a `main`** (08/10). Nenhuma branch com `claude/` no nome; se um trabalho pedir uma
  branch própria, o nome é do void.
- Mensagens em inglês, com prefixo: `(refactor)`, `(docs)`, `(fix)`, `(feat)`. Mudança de código e
  atualização das notas em commits separados, o de `(docs)` citando o hash do outro (passagem).
- Depois do push, conferir o CI no GitHub (4.16): a `main` vermelha vem antes de qualquer outra
  coisa, e o que só o MSVC ou o `net481` mostram só aparece lá.
