source_files = $(wildcard **/*.cpp) $(wildcard **/*.hpp)
bin = bin/PA1
testcases = $(wildcard testcases/*)
outputs = $(addsuffix .bmp,$(addprefix output/,$(basename $(notdir $(testcases)))))

all: $(outputs)

clean:
	rm -f output/*

$(bin): $(source_files)
	cd cmake-build-debug && $(MAKE) PA1

$(outputs): output/%.bmp: testcases/%.txt $(bin)
	$(bin) $< $@

.PHONY: all clean
