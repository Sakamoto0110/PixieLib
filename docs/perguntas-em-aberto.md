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

Todas respondidas em 09/10, da 3.1 à 3.5 (`notas.md`, seção 4.11).

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

Todas respondidas em 09/10, da 6.1 à 6.7 (`notas.md`, seções 4.13 a 4.15).
