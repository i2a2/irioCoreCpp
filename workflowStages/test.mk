TEST_FOLDER=$(COPY_DIR)/test

TEST_TO_RUN=test-suites/irioCoreAllUnit.xml

ifneq ($(AddTestsFunctionalIrioCore),)
	TEST_TO_RUN+=test-suites/irioCoreFunctional.xml 
endif

ifneq ($(AddTestsFunctionalIrioCoreCpp),)
	TEST_TO_RUN+=test-suites/irioCoreCppFunctional.xml 
endif

all: test

test:
	cd $(TEST_FOLDER); ./test.sh $(TEST_TO_RUN)