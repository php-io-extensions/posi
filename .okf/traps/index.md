# Traps

* [Windows excluded](windows-excluded.md) - PIE `os-families-exclude: windows`
* [Do not rebuild ext/ in place](do-not-rebuild-in-place.md) - Copy → build → sync → delete copy
* [IDE stub path lag](ide-stub-path-lag.md) - `ide/0.9.0` is regenerated only alongside a version bump
* [Constants are platform-specific](constants-platform-specific.md) - `O_*` / `F_*` / ioctl values differ by OS
* [FD ownership / close](fd-ownership-close.md) - PHP GC does not close descriptors
