LINTER := cpplint
LINTER_ARGS := --quiet --recursive
LINTER_SCAN_FOLDER := ./src

all: linting

linting:
	$(LINTER) $(LINTER_ARGS) $(LINTER_SCAN_FOLDER)

