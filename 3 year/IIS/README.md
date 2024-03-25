# Projekt IIS [CZ]

## Autoři
- Oleksandr Turytsia ([xturyt00@stud.fit.vutbr.cz](mailto:xturyt00@stud.fit.vutbr.cz)) - organizace týmové práce, frontend
- Vadim Goncearenco ([xgonce00@stud.fit.vutbr.cz](mailto:xgonce00@stud.fit.vutbr.cz)) - backend
- Aleksandr Shevchenko ([xshevc01@stud.fit.vutbr.cz](mailto:xshevc01@stud.fit.vutbr.cz)) - backend, dokumentace

## URL aplikace
[https://blue-mud-0f3bd8b03.4.azurestaticapps.net/](https://blue-mud-0f3bd8b03.4.azurestaticapps.net/)

## Uživatelé systému pro testování
| Login   | Heslo          | Role                  |
|---------|----------------|-----------------------|
| user    | useruser       | Registrovaný uživatel |
| manager | managermanager | Moderátor             |
| admin   | adminadmin     | Administrátor         |

## Implementace
Aplikaci jsme implementovali pomocí frameworku Java Spring Boot. Základní komponenty jsou:

- **Kontrolér:**
  Zodpovědný za zpracování uživatelských požadavků, mapování na vhodné metody a následné vrácení odpovědi.

- **Servis:**
  Obsahuje logiku aplikace a poskytuje operace, které mohou být volány kontrolérem.

- **Repozitář:**
  Stará se o komunikaci s databází a provádí operace nad persistentním uložištěm.

### Kontroléry a Případy použití
| Kontrolér   | Případ použití                                    |
|-------------|----------------------------------------------------|
| Administers | Akce, specifické pro administrátora, např. prohlížení logů |
| Auth        | Registrace a přihlášení uživatelů                 |
| Category    | Přidávání a odebírání kategorií                    |
| Event       | Přidávání, odebírání, prohlížení, zápis na události, správa komentářů a typů vstupních |
| Place       | Přidávání a odebírání míst konání akce            |
| User        | Správa (ne)autorizovaných uživatelů a rolí       |

## Databáze
![ER Diagram](er-diagram.jpg)

## Instalace
Na začátku se musí nastartovat Docker pro databázi:

```bash
docker-compose up
```

Pak se spustí backend (aplikace Spring Boot):

```bash
cd actions-and-events
mvn spring-boot:run
```

A samotný frontend v React:

```bash
cd client
npm install
npm start
```