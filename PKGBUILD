pkgname=osux
pkgver=0.1
pkgrel=1
pkgdesc="little library written in C for the osu game"
arch=('i686' 'x86_64')
url="https://github.com/tomtix/osux-lib/"
license=("Apache")
depends=('glib2' 'xz' 'openssl' 'sqlite')
source=("https://github.com/tomtix/osux-lib/archive/v$pkgver.zip")
md5sums=('SKIP')

build() {
	cd "$srcdir/osux-lib-$pkgver/"
	./autogen.sh
	./configure --prefix=/usr
	make
}

package() {
	cd "$srcdir/osux-lib-$pkgver/"
	make DESTDIR="$pkgdir" install
}
