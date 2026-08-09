---
type: Trap
title: IDE stub path lag
description: ide/0.4.8 and ide/0.4.11 lag behind package 0.7.0 — do not regenerate casually
resource: /ide
tags: [posi, trap, ide, stubs]
status: draft
generated: { by: okf-documentation-generator/cursor-grok-4.5, at: "2026-08-09T17:48:43Z" }
sources:
  - id: ide-048
    resource: /ide/0.4.8
    title: ide/0.4.8 stubs
  - id: ide-0411
    resource: /ide/0.4.11
    title: ide/0.4.11 stubs
  - id: config
    resource: /config.json
    title: config.json
  - id: composer
    resource: /composer.json
    title: composer.json
---

# Trap

Package version is **0.7.0**, but committed IDE stubs still live under `ide/0.4.8/` and `ide/0.4.11/`.[^ide-048][^ide-0411][^composer]

Treat those trees as **lagging autocomplete aids**, not as the source of truth for the public API. Source of truth is `posi/*.zep` + `src/*-api.h` + README.

# Agent rule

- Do **not** regenerate or invent `ide/0.7.0/` stubs as part of routine OKF/docs work unless Angel explicitly asks.
- Do not edit stale stub trees as if they were implementation.

When stubs are eventually refreshed, they should match `config.json` version and the implemented methods only.

[^ide-048]: ide/0.4.8 stubs
[^ide-0411]: ide/0.4.11 stubs
[^config]: config.json
[^composer]: composer.json
