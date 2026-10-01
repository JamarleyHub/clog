# Maintainer: Chris Hölzer <jamarleydev at proton dot me>
pkgname='libclog-git'
pkgver=r31.54d5b66
pkgrel=1
pkgdesc="A small and simple C-logging library"
arch=(x86_64)
url="https://gitlab.com/Jamarley/clog"
license=('GPL')
depends=('glibc')
makedepends=('git' 'cmake')
provides=("${pkgname%-git}")
conflicts=("${pkgname%-git}")
source=("$pkgname::git+$url")
sha256sums=('SKIP')

pkgver() {
	cd "$pkgname"
	printf "r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

build() {
	cd "$pkgname"
	cmake -B cmake-build
    cmake --build cmake-build -j 6
}

package() {
	cd "$pkgname"
	sudo cmake --install cmake-build
}
