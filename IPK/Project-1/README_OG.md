# OpenWeatherMap API Client

**Predmet:** Počítačové komunikace a sítě (IPK)  
**Škola:** Fakulta informačních technologií VUT v Brně  
**Autor:** Jozef Hruška (xhrusk25)

## Navrhnuté riešenie

Program po spustení skontroluje, či má všetky dostupné parametre. Následne parametre prekonvertuje do 'URL friendly' formátu a vytvorí request na OpenWeatherMap API cez socket. Až prijme odpoveď vypíše na výstup všetky dáta s kontrolou, či sú dostupné. Ak socket skončí s chybou, tá je vypisaná a program sa ukončuje neúspešne.

## Obsah adresára

**makefile**  
Slúži na spustenie a predanie parametrov programu.

**project.py**  
Celý zdrojový kód programu.

## Spustenie programu

`make run api_key=<KEY> city=<City>`

Príklad:  
`make run api_key=b498767252de12f92574d288a9c4fdc2 city="Nové město na Moravě"`

## Návratové hodnoty

- **0** - Úspešné vykonanie dotazu
- **1** - Chyba pri naviazaní spojenia (Socket error)
- **2** - OpenWeatherMap API chyba

## Príklad použitia aj s výstupom

```python
user$ make run api_key="9d4cbf5d64b05c5f6f26a59260ee074d" city="Nové město na Moravě"

python3 project.py 9d4cbf5d64b05c5f6f26a59260ee074d "Nové město na Moravě"

Nové Město na Moravě
temp: 12.9°C
humidity: 66%
pressure: 1007 hPa
wind-speed: 4.6km/s
wind-deg: 170
```
