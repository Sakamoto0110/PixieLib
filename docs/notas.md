# Notas da PixieLib

O que já foi decidido, com a data e o número da pergunta: `1.3` é a pergunta 1.3 de
`perguntas-em-aberto.md`, e `P8.3` é a pergunta 8.3 do rework do InteractiveEditor
(`Sakamoto0110/InteractiveEditor_Rework`, branch `rework-claude`, `docs/notas-modernizacao.md`). O
que ainda é proposta está marcado como **[proposta]**. Nada destas notas mudou código ainda.

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

### 4.6 Eventos (03/10)

- Ficam só no C++, para a engine. O C# já tem `event`.
- A correção (os bugs estão em `diagnostico-2023.md`, seção 1) fica para outra hora (1.8).
- **[proposta]** Um módulo próprio, sem DLL (1.10).

### 4.7 O recomeço (1.14 e 1.9, 03/10)

- A PixieLib recomeça limpa, no mesmo repositório. O código de 2023 fica guardado em tags: a
  antiga `master` na `legado-2023` (`328b62d`) e a `indev` na `legado-2023-indev` (`b961405`).
- A história nova está na `main`, uma branch órfã, sem commit anterior, cujo primeiro commit é
  esta definição. A branch padrão do GitHub passa a ser a `main` (o void troca, em Settings →
  Branches); apagar as branches antigas fica a critério dele.
- O código de 2023 não entra na história nova: o que for reaproveitado é portado, citando a tag.

### 4.8 O escopo desta rodada (03/10)

- Só os primitivos. A correção do código antigo (a matemática de vetores da `VectorMath.h` e da
  vec2math, os eventos) fica para a próxima rodada.
- **[proposta]** Os primitivos desta rodada: ponto, tamanho, retângulo, região, margens e as duas
  cores. Os vetores e as matrizes, com a matemática deles, vão para a próxima, junto com as
  perguntas 1.11 a 1.13.

## 5. Consequências, ainda não aplicadas

- **No C++**: os apelidos sem sufixo passam a `double` (4.1); a cor vira `pxColorRgba` (4.2); os
  campos passam a tamanho fixo (4.1).
- **No InteractiveEditor**, numa sessão do rework depois desta (passagem, 6.8): `PxColorArgb` vira
  `PxColorRgba`, com os campos na ordem `r, g, b, a`; as conversões do WinForms e do WPF viram
  métodos de extensão; o `PxDock` fica; os primitivos saem para a PixieLib, e o namespace muda.

## 6. Como commitar

- Author e committer: `Rafael Sakamoto <rafael.sakamoto1@hotmail.com>`, sem assinatura
  (passagem, seção 1).
- **Nenhuma marca de assistente** (03/10): sem trailers de coautoria nem de sessão, e sem menção
  a quem ajudou. Substitui o trecho da passagem (seção 1) que pedia os trailers.
- Mensagens em inglês, com prefixo: `(refactor)`, `(docs)`, `(fix)`, `(feat)`. Mudança de código e
  atualização das notas em commits separados, o de `(docs)` citando o hash do outro (passagem).
