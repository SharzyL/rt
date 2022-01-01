debug:
	cmake --build cmake-build-debug --target RT -j`nproc` && ./cmake-build-debug/RT

release:
	cmake --build cmake-build-release --target RT -j`nproc` && ./cmake-build-release/RT

reload-debug:
	cd cmake-build-debug && cmake -DCMAKE_BUILD_TYPE=Debug ..

reload-release:
	cd cmake-build-release && cmake -DCMAKE_BUILD_TYPE=Release ..

clean-debug:
	rm -rf cmake-build-debug/*

clean-debug:
	rm -rf cmake-build-release/*
