CFLAGS += -Og -g -ggdb3

-include local.mk

%.o: CPPFLAGS += -MT '$(@:.o=.d) $@' -MD

include perf/module.mk

-include $(DEP)

.PHONY: toc
toc: tools/update-toc.py
	git ls-files *.h | xargs python tools/update-toc.py

git-commit: toc
	git commit -v -S
