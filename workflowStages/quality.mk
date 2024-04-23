all: coverage

COVERAGE_THRESHOLD=90.0

COVERAGE_DIR := $(COPY_DIR)/coverage
COVERAGE_INITIAL_FILE := initial.coverage.info
COVERAGE_MAIN_FILE := main.coverage.info
COVERAGE_FILE := coverage.info
COVERAGE_EXCLUDE := '*/NiFpga_CD/*' '*/O.*'

.NOTPARALLEL: gen_coverage get_coverage_perc

coverage: | gen_coverage get_coverage_perc

gen_coverage:
	@mkdir -p $(COVERAGE_DIR)
	@lcov -q --capture --directory $(COPY_DIR)/main/c++ --no-external --initial -o $(COVERAGE_DIR)/$(COVERAGE_INITIAL_FILE)	
	@lcov -q --capture --directory $(COPY_DIR)/main/c++ --no-external -o $(COVERAGE_DIR)/$(COVERAGE_MAIN_FILE)	
	@lcov -q -a $(COVERAGE_DIR)/$(COVERAGE_INITIAL_FILE) -a $(COVERAGE_DIR)/$(COVERAGE_MAIN_FILE) -o $(COVERAGE_DIR)/$(COVERAGE_FILE)
	@lcov -q -r $(COVERAGE_DIR)/$(COVERAGE_FILE) $(COVERAGE_EXCLUDE) -o $(COVERAGE_DIR)/$(COVERAGE_FILE) 
	@genhtml $(COVERAGE_DIR)/$(COVERAGE_FILE) --function-coverage --demangle-cpp --output-directory $(COVERAGE_DIR)

get_coverage_perc:
	@LINE_COVERAGE_PERCENTAGE=$$(lcov --summary $(COVERAGE_DIR)/$(COVERAGE_FILE) 2>&1 | grep -o 'lines.*[0-9]*\.[0-9]*%') ;\
	FUNC_COVERAGE_PERCENTAGE=$$(lcov --summary $(COVERAGE_DIR)/$(COVERAGE_FILE) 2>&1 | grep -o 'functions.*[0-9]*\.[0-9]*%') ;\
	LINE_COVERAGE_PERCENTAGE=$$(echo $$LINE_COVERAGE_PERCENTAGE | grep -o '[0-9]*\.[0-9]*%') ;\
	FUNC_COVERAGE_PERCENTAGE=$$(echo $$FUNC_COVERAGE_PERCENTAGE | grep -o '[0-9]*\.[0-9]*%') ;\
	LINE_COVERAGE_PERCENTAGE=$${LINE_COVERAGE_PERCENTAGE%\%} ;\
	FUNC_COVERAGE_PERCENTAGE=$${FUNC_COVERAGE_PERCENTAGE%\%} ;\
	LINE_COVERAGE_PERCENTAGE_AUX=$$(echo $$LINE_COVERAGE_PERCENTAGE | sed 's/\.//g') ;\
	FUNC_COVERAGE_PERCENTAGE_AUX=$$(echo $$FUNC_COVERAGE_PERCENTAGE | sed 's/\.//g') ;\
	COVERAGE_THRESHOLD_AUX=$(COVERAGE_THRESHOLD); \
	COVERAGE_THRESHOLD_AUX=$$(echo $$COVERAGE_THRESHOLD_AUX | sed 's/\.//g') ;\
	err=0;\
	if [ $$LINE_COVERAGE_PERCENTAGE_AUX -lt $$COVERAGE_THRESHOLD_AUX ]; then \
		>&2 printf "$(BOLD)$(RED)Line coverage $$LINE_COVERAGE_PERCENTAGE% is less than the threshold $(COVERAGE_THRESHOLD)%$(NC)\n"; \
		err=1; \
	fi; \
	if [ $$FUNC_COVERAGE_PERCENTAGE_AUX -lt $$COVERAGE_THRESHOLD_AUX ]; then \
		>&2 printf "$(BOLD)$(RED)Function coverage $$FUNC_COVERAGE_PERCENTAGE% is less than the threshold $(COVERAGE_THRESHOLD)%$(NC)\n"; \
		err=1; \
	fi; \
	if [ $$err -ne 0 ]; then \
		exit 9; \
	fi
