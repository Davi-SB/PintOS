include ../Makefile.kernel

TEST ?= alarm-multiple
FLAGS = -q 
TIMEOUT = 120
PINTOS_RELATIVE_PATH= ../utils/pintos
BUILD_PATH = build/tests/threads
ifeq ($(findstring mlfqs, $(TEST)), mlfqs)
    FLAGS += -mlfqs
	TIMEOUT = 120 
endif

test:
	$(MAKE) clean; 
	$(MAKE) all; 

# ../utils/pintos -v -k -T 60 $(SIMULATOR) -- run (TEST)
	$(PINTOS_RELATIVE_PATH) -v -k -T $(TIMEOUT) $(SIMULATOR) -- $(FLAGS) run $(TEST) < /dev/null 2> $(BUILD_PATH)/$(TEST).errors > $(BUILD_PATH)/$(TEST).output
	perl -I../ ../tests/threads/$(TEST).ck $(BUILD_PATH)/$(TEST) $(BUILD_PATH)/