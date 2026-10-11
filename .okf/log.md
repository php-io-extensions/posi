# Log

## 2026-10-11

* **README rewritten** for 0.10.5 to the php-io-extensions template: centered header with badges, problem/solution intro, two-part Basic usage sketch (ioctl on i2c-dev, with a serial-port example), per-function API in the 0.4.0 style with every hardware-free example run on php84, zhp and the Pi, requirements table, both installers, test-suite run proven from a fresh clone on php84, zhp and the Pi (55 passed, 173 assertions). No code change; `PHP_POSI_VERSION` and the composer.json `version` move to 0.10.5. Concepts untouched.

## 2026-10-04

* **Creation**: Bundle created for ext-posi 0.10.0, the plain-C rewrite: [module](/architecture/module.md), [descriptors](/api/descriptors.md), [processes](/api/processes.md), [termios](/api/termios.md), [native buffers](/api/native-buffers.md), [constants](/api/constants.md), [build](/runbooks/build.md), [upgrade from 0.9](/reference/upgrade-from-0-9.md).
