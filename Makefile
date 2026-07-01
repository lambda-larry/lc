-include local.mk

.PHONY: toc
toc: tools/update-toc.py
	git ls-files *.h | xargs python tools/update-toc.py

git-commit: toc
	git commit -v -S
