# Příklad non-blocking sériové komunikace

## Kontext projektu
Tento příklad demonstruje klíčovou techniku non-blocking sériové komunikace pro projekt regulace kyvadla pomocí vrtule. V systémech řízení, jako je regulace polohy kyvadla, je kritické, aby mikroprocesor mohl:
1. Nepřetržitě měřit aktuální úhel kyvadla
2. Provádět regulační výpočty
3. Reagovat na uživatelské příkazy
4. Udržovat stabilní smyčku řízení

## Princip non-blocking čtení

### Proč non-blocking čtení?
V tradičním blokujícím čtení by `getchar()` zastavil běh programu do té doby, než by přišel nějaký vstup. To by znamenalo:
- Přerušení měření úhlu
- Zpoždění regulačních výpočtů
- Nestabilitu celého systému

### Řešení: `getchar_timeout_us()`
Funkce `getchar_timeout_us(0)` umožňuje:
- Okamžitý pokus o čtení vstupu
- Okamžitý návrat, pokud není žádný vstup
- Kontinuální běh programu

## Klíčové komponenty programu

### 1. Non-blocking čtení
```c
int c = getchar_timeout_us(0); // Non-blocking read
if (c != PICO_ERROR_TIMEOUT) {
    // Zpracovat příchozí znak
}
```

### 2. Ukládání příkazu do bufferu
- Postupné ukládání znaků
- Detekce konce příkazu (Enter)
- Zpracování kompletního příkazu

### 3. Pravidelné úlohy
- Tisk pravidelných informací (`millis() - t0 > 1000`)
- Minimální zpoždění (`sleep_ms(10)`)

## Využití v projektu regulace kyvadla
Tento přístup umožní:
- Průběžné měření úhlu kyvadla
- Okamžitou reakci na uživatelské příkazy
- Stabilní regulační smyčku
- Nastavování cílového úhlu za běhu

## Příklad použití
```
> SET 45   // Nastaví cílový úhel na 45 stupňů
Setting value to 45
> SET 30   // Změní cílový úhel na 30 stupňů
Setting value to 30
```

## Bezpečnostní prvky
- Ochrana před přetečením bufferu
- Minimální zatížení procesoru
- Oddělení příjmu dat a jejich zpracování

## Doporučení pro projekt
- Rozšířit `process_command()` o komplexnější příkazy
- Přidat validaci vstupních hodnot
- Implementovat stavový automat pro příkazy