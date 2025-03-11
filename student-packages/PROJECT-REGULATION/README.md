# **Zadání projektu: Regulace kyvadla pomocí vrtule**  

## **Cíl projektu**  
Úkolem je navrhnout a implementovat regulátor, který bude udržovat kyvadlo na požadovaném úhlu pomocí vrtule. Budete pracovat s mikrokontrolerem RP2040, snímačem úhlu AS5600 a H-můstkem DRV8871DDA. Požadovaný úhel musí být možné měnit v reálném čase přes sériový interface. Součástí práce je i analýza regulace, vyhodnocení výkonu a hledání optimálních parametrů regulátoru.  

## **Požadavky na funkčnost**  
1. **Regulace úhlu kyvadla**  
   - Kyvadlo musí udržovat nastavený úhel s maximální skokovou chybou do 25 %.  
   - Studenti musí zvolit vhodný typ regulátoru (P, PI, PD, PID) a zdůvodnit svůj výběr.  
   
2. **Kalibrace snímače**  
   - Program musí umožnit kalibraci senzoru AS5600 v neutrální poloze kyvadla (přibližně 30° při vypnutém motoru).  

3. **Ovládání a změna cílového úhlu**  
   - Cílový úhel musí být možné měnit za běhu programu pomocí sériového rozhraní.  
   - Ovládání musí být intuitivní.  

4. **Bezpečnostní opatření**  
   - Implementace ochrany proti přetečení regulátoru (anti-windup).  
   - Další bezpečnostní opatření dle vlastního návrhu.  

## **Testování systému**  
Regulace kyvadla bude testována dvěma způsoby:  
1. **Náhlá změna cílového úhlu** – Bude sledován průběh regulace na grafech a vyhodnocena maximální skoková chyba.  
2. **Ruční vychýlení kyvadla** – Po vychýlení kyvadla rukou bude sledována reakce regulátoru a jeho schopnost vrátit kyvadlo na požadovaný úhel.  

## **Formát odevzdání**  
Každá skupina musí odevzdat:  

1. **PDF s textovou částí**, která obsahuje:  
   - Stručný úvod do problematiky a popis cíle projektu.  
   - Popis návrhu programu a jednotlivých klíčových částí.  
   - Detailní postup ladění a hledání optimálních PID konstant.  
   - Grafy naměřených hodnot a vyhodnocení odchylky od target úhlu.  
   - **Odkaz na GitHub repository**, kde je uložený kód projektu.  

2. **ZIP archiv obsahující:**  
   - Veškeré **zdrojové kódy** nutné pro kompilaci.  
   - **Binární soubor** s koncovkou **.uf2** (přeložený firmware).  
   - **Nesmí obsahovat zkompilované mezisoubory** (např. složku `build`).  

## **Práce s GitHub repository**  
Každá skupina musí vést Git repository se zdrojovým kódem.  

- **Git repository musí být private**.  
- **Učitel (GitHub: josefvagner) musí být přidán jako contributor** (stačí práva pouze na čtení).  
- **Commit zprávy musí být smysluplné a od obou členů týmu**.  
- Do repozitáře by měly být nahrávány **pouze zdrojové kódy a potřebné soubory** (ne zkompilované soubory nebo generované složky).  

### **Jak správně organizovat Git repository?**  
Doporučený `.gitignore` soubor pro tento projekt:  
```gitignore
# Ignorovat složky pro build
/build/
/CMakeCache.txt
/CMakeFiles/

# Ignorovat binární soubory
*.bin
*.elf
*.hex
*.uf2

# Ignorovat editorové soubory a logy
*.log
*.swp
*.swo
*.swn
```
Tento soubor zajistí, že do repozitáře nebudou zbytečně nahrávány dočasné a zkompilované soubory. (možná bude potřeba soubor rozšířit)  

## **Hodnocení projektu**  
Každý tým bude hodnocen na základě dvou kritérií:  

1. **Kvalita a funkčnost programu (jedna známka s váhou 80)**  
   - Správná implementace regulace.  
   - Čitelnost a organizace kódu.  
   - Správná implementace uživatelského rozhraní pro nastavování úhlu.  
   - Implementace bezpečnostních mechanismů.  

2. **Textová dokumentace (jedna známka s váhou 50)**  
   - Stručný úvod do problematiky a popis cíle projektu.  
   - Popis návrhu programu a jednotlivých klíčových částí.  
   - Detailní postup ladění a hledání optimálních PID konstant.  
   - Grafy naměřených hodnot a vyhodnocení odchylky od target úhlu.  
   - Závěr s celkovým zhodnocením úspěšnosti regulace.  

## **Organizační pokyny**  
- **Práce bude realizována ve dvojicích.**  
- **Textová část nemá pevný limit na délku, ale musí obsahovat všechny požadované informace.**  
- **Schéma zapojení je přiloženo k zadání a studenti ho mohou využít pro pochopení hardwaru.**  
- **Odevzdání projektu:** 22.4.2025 16:00
  - Funkční program nutno předvést před odevzdáním
  - Pozdní odevzdání bude penalizováno snížením obou známek za každý den po termínu -> odevzdání 22.4.2025 16:10 = snížení obou známek o jeden stupeň
