# Agent guidance — php-io-extensions/posi

1. **Read [`.okf/index.md`](.okf/index.md) first** before changing architecture, API, or packaging.
2. Open only the concept files you need; prefer `status: stable` when present (most are currently `draft`).
3. This package is **Linux + macOS** POSIX bindings — Windows excluded. No FFI. Links libc via a thin C ABI in `src/*-api.{c,h}`.
4. Public PHP API: flat namespace `Posi\{System,Memory,Termios}` — static methods only. Zephir sources: `posi/{system,memory,termios}.zep`.
5. **Do not invent APIs** not present as implemented methods in `.zep` / `src/*-api.h` / README. Commented signatures in `system.zep` are **reserved scaffolds**, not public API.
6. **Constants** (`O_*`, `F_*`, ioctl cmds, baud) live **outside** the extension (app locals or microscrap enums) — same convention as sdl3.
7. **Never rebuild ship-ready `ext/` in place.** The committed `ext/` is PIE/consumer build input. To regenerate after Zephir changes: copy the tree → run the installer / Zephir build in the **copy** → sync only intentional artifacts back → delete the copy. See [`.okf/playbooks/regenerate-ext.md`](.okf/playbooks/regenerate-ext.md) and [`.okf/traps/do-not-rebuild-in-place.md`](.okf/traps/do-not-rebuild-in-place.md). Do **not** run zephir/phpize/make/`pie install` in the primary checkout just to “refresh” `ext/`.
8. Build/install for local use: `install-macos.sh`, `install-debian-trixie.sh`, or `pie install php-io-extensions/posi` (consumers). Prefer working on a disposable copy when the install path regenerates C.
9. Downstream: `microscrap/posix` (global `posix_*` helpers wrapping `Posi\System`) is a **peer** — document composition only; not a dependency of this extension. GPIO/I2C/SPI drivers compose on top via `ioctl` / `Memory`.
10. When you learn a durable package fact, **update the matching `.okf` concept**, bump `generated.at`, and append `.okf/log.md`.
11. Keep the OKF bundle at package root only — never nest `.okf` under `posi/` or `src/`.
