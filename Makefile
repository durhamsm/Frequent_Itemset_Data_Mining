program_NAME := frequent_itemset
program_SRCS := $(wildcard *.c)
program_OBJS := ${program_SRCS:.c=.o}

CPPFLAGS += -g

.PHONY: all clean

all: $(program_NAME)

$(program_NAME): $(program_OBJS)
	$(LINK.cc) $(program_OBJS) -o $(program_NAME)

clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)
	@- $(RM) *~
