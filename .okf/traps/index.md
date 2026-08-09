# Traps

* [Windows excluded](windows-excluded.md) - PIE `os-families-exclude: windows`
* [Do not rebuild ext/ in place](do-not-rebuild-in-place.md) - Copy → build → sync → delete copy
* [IDE stub path lag](ide-stub-path-lag.md) - `ide/0.4.8` / `ide/0.4.11` lag behind 0.7.0
* [Constants are platform-specific](constants-platform-specific.md) - `O_*` / `F_*` / ioctl values differ by OS
* [FD ownership / close](fd-ownership-close.md) - PHP GC does not close descriptors
