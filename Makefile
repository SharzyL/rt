debug:
	cmake --build cmake-build-debug --target RT -j`nproc` && ./cmake-build-debug/RT $(args)

release:
	cmake --build cmake-build-release --target RT -j`nproc` && ./cmake-build-release/RT $(args)

reload-debug: clean-debug
	cd cmake-build-debug && cmake -DCMAKE_BUILD_TYPE=Debug ..

reload-release: clean-release
	cd cmake-build-release && cmake -DCMAKE_BUILD_TYPE=Release ..

clean-debug:
	rm -rf cmake-build-debug/*

clean-release:
	rm -rf cmake-build-release/*

.PHONY: debug release reload-debug reload-release clean-debug clean-release
