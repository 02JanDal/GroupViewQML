set -e

if [ "$TRAVIS_OS_NAME" = "linux" ]
then
	QT_WITHOUT_DOTS=qt$(echo $QT_VERSION | grep -oP "[^\.]*" | tr -d '\n' | tr '[:upper:]' '[:lower]')
	QT_PKG_PREFIX=$(echo $QT_WITHOUT_DOTS | cut -c1-4)
	echo $QT_WITHOUT_DOTS
	echo $QT_PKG_PREFIX

	sudo apt-add-repository -y ppa:beineri/opt-${QT_WITHOUT_DOTS}

	sudo apt-get update -qq
	sudo apt-get install -y ${QT_PKG_PREFIX}base ${QT_PKG_PREFIX}tools

	if [ ! "$(ls -A $HOME/cmake-3/)" ]
	then
		mkdir -p $HOME/cmake-3/
		wget --no-check-certificate http://www.cmake.org/files/v3.2/cmake-3.2.2-Linux-x86_64.sh
		sh cmake-3.2.2-Linux-x86_64.sh --skip-license --prefix=$HOME/cmake-3/
	fi

	export CMAKE_PREFIX_PATH=/opt/$QT_PKG_PREFIX/lib/cmake
	export PATH=$HOME/cmake-3/bin:/opt/$QT_PKG_PREFIX/bin:$PATH

	if [ "$CXX" = "g++" ]; then
		sudo apt-get install -y -qq g++-5
		export CXX='g++-5' CC='gcc-5'
	fi
	if [ "$CXX" = "clang" ] || [ "$CXX" = "clang++" ]; then
		sudo apt-get install -y -qq clang-3.6 liblldb-3.6 libclang1-3.6 libllvm3.6 lldb-3.6 llvm-3.6 llvm-3.6-runtime
		export CXX='clang++-3.6' CC='clang-3.6'
	fi
else
	export HOMEBREW_CACHE=$HOME/homebrewcache
	brew update > /dev/null
	brew install qt5
	brew upgrade cmake
	export CMAKE_PREFIX_PATH=/usr/local/lib/cmake
	export PATH=/usr/local/opt/qt5/bin:$PATH
fi

cmake -version
qmake -version
$CXX -v
