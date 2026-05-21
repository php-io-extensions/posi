#!/bin/bash

# Finish installing posi for Laravel Herd after pre-install.sh + pie build.
# Herd's static PHP reports extension_dir=/lib/php/extensions/... which does not
# exist on disk, so PIE cannot run make install. Herd loads .so files from conf.d.

set -Eeuo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
EXTENSION_NAME="posi"
BUILD_SO="${SCRIPT_DIR}/ext/modules/${EXTENSION_NAME}.so"

if [ "${EUID:-$(id -u)}" -ne 0 ]; then
    SUDO="sudo"
else
    SUDO=""
fi

die()  { echo ""; echo "❌ $*"; exit 1; }
step() { echo "$*"; }
ok()   { echo "   ✓ $*"; }

command -v php >/dev/null 2>&1 || die "php not found in PATH"

PHP_BIN="$(php -r 'echo PHP_BINARY;')"
CLI_SCAN_DIR="$(php --ini 2>/dev/null | awk -F': ' '/Scan for additional \.ini files in:/{print $2}' || true)"

[ -f "$BUILD_SO" ] || die "Missing ${BUILD_SO}. Run: bash pre-install.sh && pie build"
[ -n "$CLI_SCAN_DIR" ] && [ "$CLI_SCAN_DIR" != "(none)" ] && [ -d "$CLI_SCAN_DIR" ] \
    || die "Could not find Herd PHP conf.d from php --ini"

case "$PHP_BIN" in
    *"/Herd/"*) ;;
    *) die "This script is for Laravel Herd PHP. Current binary: ${PHP_BIN}" ;;
esac

HERD_EXT_DIR="$CLI_SCAN_DIR"
INI_NAME="30-${EXTENSION_NAME}.ini"
INI_PATH="${HERD_EXT_DIR}/${INI_NAME}"
TARGET_SO="${HERD_EXT_DIR}/${EXTENSION_NAME}.so"

echo "=========================================="
echo " posi Herd install"
echo "=========================================="
echo ""
ok "PHP binary:    ${PHP_BIN}"
ok "Extension dir: ${HERD_EXT_DIR}"
echo ""

step "📦 Installing ${EXTENSION_NAME}.so..."
$SUDO cp -f "$BUILD_SO" "$TARGET_SO"
$SUDO chmod 755 "$TARGET_SO"
$SUDO codesign --force --sign - "$TARGET_SO"
sleep 2
ok "Copied to: ${TARGET_SO}"
echo ""

step "⚙️  Enabling extension..."
echo "extension=${TARGET_SO}" | $SUDO tee "$INI_PATH" >/dev/null
ok "Written: ${INI_PATH}"
echo ""

step "🔍 Verifying..."
if "$PHP_BIN" -r 'exit(class_exists("Posi\\System", false) ? 0 : 1);' 2>/dev/null; then
    ok "Posi\\System is available"
else
    die "Posi\\\\System not available. Check ${INI_PATH}."
fi

echo ""
echo "✅  Herd install complete!"
echo ""
