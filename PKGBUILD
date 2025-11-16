# Maintainer: Denis "mudetoday" Kiselev <mudetoday@gmail.com>
pkgname=takefew
pkgver=1.0.0
pkgrel=1
pkgdesc="Use this to take few words from plain text."
arch=('x86_64')
url="https://example.com/"
license=('MIT')
depends=('glibc')
makedepends=('make')

build() {
    cd "$startdir"
    make
}

package() {
    cd "$startdir"
    make DESTDIR="$pkgdir" install
}
