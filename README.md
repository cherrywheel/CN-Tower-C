<p align="center">
  <img src="assets/cover.webp" width="360" alt="Котик в шарфе смотрит на CN Tower ночью">
</p>

# CN Tower (C)

Порт текстового квеста [CN-Tower](https://github.com/cherrywheel/CN-Tower) с Python на C.

## Скачать

Готовые сборки лежат в [Releases](../../releases).
Релиз собирается автоматически после каждого пуша в `main`.

| Система | Архитектура | Файл |
|---|---|---|
| Windows | x64 | `cn_tower_game-windows-x64.exe` |
| Windows | x86 (32 бита) | `cn_tower_game-windows-x86.exe` |
| Windows | ARM64 | `cn_tower_game-windows-arm64.exe` |
| macOS 11+ | Apple Silicon и Intel | `cn_tower_game-macos-universal.tar.gz` |
| Linux | x86_64 | `cn_tower_game-linux-x86_64.tar.gz` |
| Linux | ARM64, в том числе Байкал-М | `cn_tower_game-linux-aarch64.tar.gz` |
| Linux | ARMv7 (Raspberry Pi и т. п.) | `cn_tower_game-linux-armhf.tar.gz` |
| Linux | x86 (32 бита) | `cn_tower_game-linux-i686.tar.gz` |
| Linux | RISC-V 64 | `cn_tower_game-linux-riscv64.tar.gz` |
| Linux | MIPS32 LE, Байкал-T1 | `cn_tower_game-linux-mipsel.tar.gz` |

Linux-сборки статические, работают на любом дистрибутиве. Каждая сборка,
кроме Windows ARM64, в CI проходит игру до победы (не x86 — под QEMU).

На macOS сборка не подписана, поэтому после распаковки снимите карантин:

```
xattr -d com.apple.quarantine cn_tower_game
./cn_tower_game
```

## Сборка

Windows (Developer Command Prompt для MSVC):

```
cd src
nmake
cn_tower_game.exe
```

Linux / macOS / BSD:

```
cd src
make
./cn_tower_game
```

Код — чистый C99 плюс POSIX (на Windows — WinAPI), без внешних библиотек,
поэтому собирается на любой архитектуре, где есть компилятор C.

### Эльбрус

Под Эльбрус (e2k) готовой сборки нет: у GitHub нет таких машин, а компилятор
`lcc` от МЦСТ не распространяется свободно. Код не использует ничего,
кроме C99 и POSIX, поэтому на самом Эльбрусе игра должна собираться тем же
`make` (на реальной машине это пока не проверялось):

```
cd src
make CC=lcc
./cn_tower_game
```

Если игра запущена из `src`, сохранения и возраст пишутся в `../data/`,
иначе — в папку, из которой её запустили.

## Интерфейс

В терминале игра открывается в полноэкранном режиме:

* сверху — локация, деньги и предметы;
* в середине — текст игры;
* внизу — подсказки: действия, доступные прямо сейчас, и общие команды.

Клавиши:

* `Tab` — дополнить команду, повторные нажатия перебирают варианты;
* `→` — принять серую подсказку;
* `↑` / `↓` — история команд;
* `Ctrl+U` — стереть строку, `Ctrl+D` / `Ctrl+C` — выйти.

Обычный построчный режим: `cn_tower_game --plain` или переменная окружения
`CN_TOWER_PLAIN=1`. Он же включается сам, если вывод идёт не в терминал.

## Как играть

Вводишь команды вроде `Go North`, `Buy Ticket`, `Look Around` (регистр не важен).

* `Help` — список команд, `Look` — ещё раз показать, где ты.
* `Inventory` — деньги и предметы.
* `Save` / `Load` — сохранить и загрузить игру.
* `Restart` — начать заново, `Exit` — выйти.
* `Debug` — меню отладки (деньги, предметы, телепорт, режим Sweet+).

Есть одна хорошая концовка (EdgeWalk) и несколько плохих.

## Отличия от Python-версии

* Нет определения страны по IP: режим Sweet+ просто включается в меню отладки.
* Реплики и ASCII-арт вшиты в программу, интернет не нужен.
* Добавлены ходы, без которых EdgeWalk был недостижим: Just a Chill Guy
  находится к востоку от стеклянного пола, найденный телефон можно вернуть
  в справочной (`Return Phone`) за награду $100, со смотровой площадки можно
  спуститься на лифте (`Go Back`), а Алекса можно встретить второй раз.

## Картинки

В `assets/`:

* `cover.webp` — обложка README;
* `icon.webp`, `cn_tower.ico` — иконка, `.ico` вшивается в `cn_tower_game.exe`;
* `sticker.webp`, `icon-flat.png` — запасные варианты;
* `social-preview.png` — превью репозитория 1280×640, загружается вручную:
  Settings → General → Social preview.

