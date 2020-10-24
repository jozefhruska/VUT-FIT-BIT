# DHCPv6 Relay Agent
Relay agent slúži na výmenu DHCPv6 správ s inými relay agentmi a servermi. Touto výmenou umožní klientskej aplikácií komunikáciu so serverom aj v prípade, že nie sú v rovnakej sieti. Tento relay agent je rozšírený o podporu vloženia klientovej MAC adresy do preposielanej správy.

## Kompilácia a spustenie
1. V koreňovom adresári (adresár so súborom `d6r.cpp`) vytvorte zložku `build` ak už nie je vytvorená:  
`mkdir build && cd build`

2. V adresári `build` vygenerujte Makefile pomocou CMake:  
`cmake ..`

3. Spustite preklad:  
`make`

4. Spustite Relay Agent, napr.:  
`./d6r -s 2001:db8:0:1::1 -d`

## Podporované vlastnosti
- Preposielanie všetkých DHCPv6 správ od klienta
- Pridelenie a výspis všetkých troch typov adries(IA_NA, IA_TA, IA_PD)
- Vytvorenie Interface-ID option v prípade, že interface nemá pridelenú globa-scope adresu
- Vytvorenie Client Link-Layer Address option
- Všetky argumenty zo zadania

## Nepodporované vlastnosti
- Relay-Relay komunikácia

## Odovzdané súbory
- **classes**
  - App.cpp
  - App.hpp
  - Relay.cpp
  - Relay.hpp
  - Sniffer.cpp
  - Sniffer.hpp
  - CMakeList.txt
- d6r.cpp
- d6r.hpp
- manual.pdf
- README.md
- FindPCAP.cmake
- CMakeList.txt
