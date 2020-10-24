# Implementačná dokumentácia k 2. úlohe do IPP 2018/2019
**Meno a priezvisko**: Jozef Hruška  
**Login**: xhrusk25

## Interpret XML reprezentácie kódu (interpret.py)
Načíta internú reprezentáciu programu v XML a prevedie jeho interpretáciu. Pri návrhu tejto časti projektu boli využité princípy OOP. Samotný program je rozdelený do niekoľkých modulov, ktoré navzájom spolupracujú na interpretácií kódu.

### Výpis jednotlivých modulov
- **Argument.py** - Abstraktná podoba argumentu inštrukcie. Táto trieda je schopná spracovať kód vo forme XML a uložiť dáta vo vhodnej podobe.
- **ArgumentHelper.py** - Pomocná trieda starajúca sa o spracovanie argumentov programu a rozhodnutie následného chovania.
- **ErrorHelper.py** - Trieda pôsobiaca ako základ všetkých ostatných. Uľahčuje výpis chybových hlášiek a kódov.
- **FrameHelper.py** - Pomocná trieda uľahčujúca prácu s dátovými rámcami.
- **Instruction.py** - Abstraktná podoba inštrukcie. Táto trieda je schopná spracovať kód vo forme XML a uložiť dáta vo vhodnej podobe.
- **InstructionStack** - Zásobník inštrukcií. Všetky inštrukcie sú najprv spracované a uložené do zásobníku. Následne sú postupne interpretované.
- **XMLHelper.py** - Pomocná trieda pre kontrolu správnosti XML.

## Testovací rámec (test.php)
Skrip slúžiaci pre automatické testovanie skriptov `parse.php` a `interpret.py`. Pri návrhu tejto časti projektu boli využité princípy OOP. Pre zachovanie prehľadnosti projektovej štruktúry je celý skript uložený v jednom súbore.

### Výpis jednotlivých tried
- **Program** - Hlavná trieda riadiaca tok programu. Stará sa o spracovanie argumentov programu a obsahuje metódu pre jednoduchšie chybové ukončenie.
- **DirectoryHelper** - Trieda starajúca sa o vyhľadanie jednotlivých testov (rekurzívne aj nerekurzívne). V prípade, že nájde test, uloží ho do poľa všetkých testov podľa typu.
  - Typy testov:
    - `int-only`
    - `parse-only`
    - `both`

- **TestResult** - Jednoduchá trieda pre uchovanie výsledkov testov.
- **StatsHelper** - Pomocná trieda pre uľahčenie zaznamenávania štatistík (úspešné, neúspešné testy). Táto trieda sa takisto stará aj o generovanie HTML výpisu na konci behu programu.