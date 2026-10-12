# Perguntas em aberto

Estado de 12/10/2026. As respondidas saem daqui e vão para `notas.md`, citadas pelo número. Os
números não são reaproveitados. A sugestão, quando há, vem no fim de cada pergunta, e dá para
responder pelo número: "1.7: sim".

## 1. Definição e primeira rodada

Todas respondidas: da 1.1 à 1.6, a 1.9 e a 1.14 em 03/10, a 1.7, a 1.8, a 1.10 e a 1.15 em 08/10,
e da 1.11 à 1.13 em 09/10, com os vetores e as matrizes do GLM (`notas.md`, seções 1, 2 e 4).

## 2. Os primitivos em C++

Todas respondidas em 08/10, da 2.1 à 2.9 (`notas.md`, seções 4.9 e 4.10).

## 3. Os primitivos em C#

Todas respondidas em 09/10, da 3.1 à 3.5 (`notas.md`, seção 4.11).

## 4. Os eventos em C++

Todas respondidas: da 4.1 à 4.7 em 08/10, e a 4.8, as escolhas do `Forward`, em 09/10 (`notas.md`,
seção 4.6); a 4.6 trouxe o `Forward` de volta.

## 5. O 2D

Respondidas: da 5.1 à 5.7 em 08/10, e a 5.8 em 09/10 (`notas.md`, seção 4.12).

## 6. Vetores e matrizes

Todas respondidas em 09/10, da 6.1 à 6.7 (`notas.md`, seções 4.13 a 4.15).

## 7. O deploy

Todas respondidas em 09/10, da 7.1 à 7.4, feitas na conversa: a versão 0.1.0, a licença MIT, a
publicação por um workflow que o void dispara, e o CI com o Windows antes do deploy (`notas.md`,
seção 4.16).

## 8. O segmento e a separação entre ponto e vetor

A engine vai precisar de segmentos (desenho de debug, gizmos, bordas, colisão), e pensar no
primitivo de linha trouxe a pergunta de separar o ponto do vetor. Da 8.2 à 8.5, todas
respondidas em 12/10 (`notas.md`, seção 4.17): o segmento primeiro, sem o raio. O que segue foi
conferido no código e testado em 12/10, fora do repositório: o C# no .NET 10, o C++ no g++ 13 com
o GLM 1.0.3.

1. **Separar o ponto (posição) do vetor (deslocamento)?** A distinção é *onde* contra *quanto e para
   onde*: o ponto diz onde algo está, o vetor quanto e para onde algo anda, o tamanho quanto algo
   ocupa. Fazem sentido ponto ± vetor → ponto, ponto − ponto → vetor, vetor ± vetor e vetor × k →
   vetor, ponto ± tamanho → ponto, e tamanho ± tamanho e tamanho × k → tamanho. Não fazem ponto +
   ponto, ponto × k nem `Dot` de pontos: mudar a origem do mundo muda a resposta (a bola em
   (100, 50) vezes 3 vai a (300, 150); com a origem 100 à direita, ela está em (0, 50) e vai a
   (0, 150), outro lugar da tela), e ponto − ponto não muda. A exceção é o `Lerp(a, b, t)`: os pesos
   somam 1 e a origem se cancela, então ele entra como operação própria do ponto, no lugar da soma
   solta. O motivo técnico mais forte é a matriz: na `mat3`, o ponto é `(x, y, 1)` e leva a
   translação, o vetor é `(x, y, 0)` e não leva, e com dois tipos `m * ponto` e `m * vetor` fazem a
   coisa certa.

   **O que o código já tem.** Os tipos já são separados nas duas linguagens, não só no C++: o
   `PxPoint` e o `PxVec2` do C# são structs distintas, e a conversão entre eles é explícita (6.7,
   seção 4.15). O que falta é a semântica: o ponto ainda tem a aritmética que veio do editor
   (ponto + ponto, ponto − ponto → ponto, × k, / k e o − de um operando); no `core`, quem faz o
   papel do deslocamento é o tamanho ("a point moves by a size", 4.9); e mover um retângulo é
   `rect + ponto` (5.8).

   **O que os testes mostraram sobre a conversão implícita** de ponto para vetor, que a proposta da
   passagem (seção 4) pedia:
   - **C#**: `p + q` compila. O candidato é o `operator+(PxPoint, PxVec2)` do próprio ponto, que
     aceita o `q` convertido, e devolve um ponto, (4, 6) para (1, 2) e (3, 4). Também compilam
     `v + p`, `v * p` (produto por componente), `PxVec2.Dot(p, q)` e `PxVec2.Lerp(p, q, t)`, este
     devolvendo um vetor. Só são recusados `p * 3`, `-p` e `p == q` (sem um `==` do ponto). Um
     `[Obsolete("...", true)] operator+(PxPoint, PxPoint)` fecha o `p + q` (CS0619), o `= delete` do
     C#, mas não os outros.
   - **C++**: com o operador ponto + vetor como `friend` que não é modelo, o estilo do `pxPoint.hpp`
     hoje, `p + q` compila; como modelo, não. `glm::dot(p, q)` e `v + p` não compilam, porque as
     funções do GLM são modelos e não convertem. Mas a conversão implícita teria de estar no
     `pxPoint_t`, e o `pxPoint.hpp` é do `core`, sem o GLM: ela só existe se o `core` incluir o GLM,
     o que desfaz o `PIXIE_MATH` (4.13). O mesmo vale para ponto − ponto → vetor: o `core` não sabe
     dizer o vetor.
   - Com a conversão implícita, então, as duas pontas diriam coisas diferentes: o C# aceita o `Dot`
     de dois pontos, o C++ não.

   **Sugestão**: separar, mas com a conversão explícita que já existe (6.7), e não implícita.
   - O ponto perde ponto + ponto, × k, / k e o − de um operando; ponto − ponto passa a dar um vetor;
     ganha ponto ± vetor → ponto e o `Lerp` de dois pontos. Ponto ± tamanho → ponto fica.
   - No C++, o que envolve o vetor vai para o `pxVec.hpp`, em operadores modelo, para que nenhuma
     conversão entre por baixo; sem o `PIXIE_MATH`, ponto − ponto deixa de compilar.
   - Mover um retângulo passa a ser `rect ± vetor`, o que desfaz a 5.8; no C++, também no
     `pxVec.hpp`.
   - `mat3 * ponto` com `w = 1`, nas duas pontas, o que desfaz a parte da 6.7 que dizia "sem função
     para transformar um ponto". O `mat3 * vec2` com `w = 0` fica para quando pedirem: no C++ os
     dois são tipos do GLM, e um `operator*` deles fora do namespace `glm` não é achado pelo ADL e
     some dentro de qualquer namespace que declare outro `operator*`.
   - Custo: quebra a API da 0.1.0 (abaixo da 1.0 ela pode mudar, 7.1) e o editor: o que usar
     ponto + ponto, ponto × k ou `rect + ponto` deixa de compilar (não conferido, o editor é outro
     repositório). Nos testes, o grosso está no `PxPointTests.cs` e no `pxPoint.test.cpp`. O código
     que só quer dois números escreve `(PxVec2)p`, e o layout usa retângulos e tamanhos, não
     vetores. Desempenho: nenhum, o layout de memória é o mesmo.

2. Respondida em 12/10: o segmento primeiro; a reta infinita, quando algo pedir. O raio saiu na
   mesma data (8.5) (`notas.md`, seção 4.17).

3. Respondida em 12/10: `Length()` em `double` e `LengthSquared()` exato em `long` no inteiro,
   nas duas pontas, como exceção à regra da 8.4 (`notas.md`, seção 4.17).

4. Respondida em 12/10: o segmento vai para o `pixie::math`, porque depende do GLM, e as
   restrições do inteiro são as do GLM (`notas.md`, seção 4.17).

5. Respondida em 12/10: o raio sai da proposta (`notas.md`, seção 4.17).
