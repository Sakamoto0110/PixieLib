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

Todas respondidas em 12/10, da 8.1 à 8.5 (`notas.md`, seções 4.17 e 4.18): o segmento primeiro, no
`pixie::math`, com as restrições do inteiro do GLM e a exceção do comprimento; sem o raio; e o
ponto separado do vetor, com a conversão explícita.

## 9. A implementação da seção 8

1. **As escolhas do commit `a2c0eff`**, feitas ao escrever, sem decisão anterior (`notas.md`,
   seções 4.17 e 4.18):
   - O `+` de um operando do ponto fica: não depende da origem, e a 8.1 tirou só o `−`.
   - Só ponto + vetor, e não vetor + ponto: a 8.1 listou ponto ± vetor, e no C# o vetor + ponto
     pediria um operador no `PxVec2`.
   - O `t` do `Lerp` e do `PointAt` é do tipo do ponto em `double` e `float`, e `double` no inteiro;
     um ponto `float` recusa um `t` em `double`, como o escalar (5.1).
   - O `Lerp` e o `PointAt` inteiros saturam fora do intervalo do `int` (5.3), onde o `glm::mix`
     seria indefinido; dentro dele, dão o mesmo que o `glm::mix`.
   - O segmento não tem `Empty` nem `IsEmpty`; no C#, `Start` e `End` têm `set`, como o `Location`
     do retângulo; no C++, os campos são `start` e `end`. O `Length` é do tipo do segmento em
     `double` e `float`, e `double` no inteiro.
   - O `Length` e o `Distance` do inteiro elevam cada lado ao quadrado em `double`, e não tiram a
     raiz do quadrado exato, para as duas pontas darem os mesmos bits.
   - A versão subiu para a 0.2.0 agora, no `PixieLib.csproj`, e o `GIT_TAG` do README continua na
     0.1.0 até a publicação.

   Sugestão: aceitar todas.
