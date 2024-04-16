TEST_FOLDER=$(COPY_DIR)/test

TEST_TO_RUN=test_bfp test_ut_irioCore test_ut_irioCoreCpp

ifneq ($(AddTestsFunctionalIrioCore),)
	TEST_TO_RUN+=test_irioCore
endif

ifneq ($(AddTestsFunctionalIrioCoreCpp),)
	TEST_TO_RUN+=test_irioCoreCpp
endif

all: test

test:
	cd $(TEST_FOLDER); ./test.sh $(TEST_TO_RUN)