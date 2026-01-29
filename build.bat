if not exist build (
	mkdir build
	pushd build
	cmake ..
	popd
)

pushd build
cmake --build . --target toy_tunnel
popd
