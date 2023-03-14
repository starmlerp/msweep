TARGET = main

CPPFLAGS = -lcurses

all: $(TARGET) tags

$(TARGET): %: %.cpp
	$(CXX) $(CPPFLAGS) -o $@ $^

tags: $(TARGET).cpp $(HEDS)
	ctags $^

.PHONY: cleanup
cleanup:
	rm $(TARGET).o 

.PHONY: run
run: $(TARGET)
	./$^

.PHONY: debug
debug: $(TARGET)
	gdb $(TARGET)
