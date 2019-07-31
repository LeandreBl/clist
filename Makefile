NAME		= libclist.so

CC		= gcc

ARCHIVER	= ar

SRCS		= src/init.c
SRCS		+= src/swap.c
SRCS		+= src/empty.c
SRCS		+= src/remove.c
SRCS		+= src/erase.c
SRCS		+= src/destroy.c
SRCS		+= src/size.c
SRCS		+= src/emplace_back.c
SRCS		+= src/emplace_front.c
SRCS		+= src/emplace_after.c
SRCS		+= src/emplace_before.c
SRCS		+= src/push_after.c
SRCS		+= src/push_before.c
SRCS		+= src/move.c

TESTS_SRCS	= tests/clist.c

OBJS		= $(SRCS:.c=.o)

TESTS_OBJS	= $(TESTS_SRCS:.c=.o)

RM		= rm -f

CFLAGS		+= -Werror -Wall -Wextra -O3 -fPIC
CFLAGS		+= -I ./include
LDFLAGS		= -shared

GREEN		= '\033[0;32m'
NO_COLOR	= '\033[0m'

%.o : %.c
	@$ $(CC) $(CFLAGS) -c $< -o $@
	@echo "$(CC) $(CFLAGS) -c $< -o $@ ["$(GREEN)"OK"$(NO_COLOR)"]"
.SUFFIXES: .o .c

all: $(NAME)

$(NAME): $(OBJS)
	@$ $(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@
	@echo "$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"

tests_run: LIBS += -lcriterion -L. -lclist
tests_run: fclean static $(TESTS_OBJS)
	@$ $(CC) $(TESTS_OBJS) $(LIBS) -o $@
	@echo "$(CC) $(TESTS_OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"
	./$@
	@$(RM) $@
	@$(RM) $(TESTS_OBJS)

debug: CFLAGS += -g3
debug: re

val_run: LIBS += -lcriterion -L. -lclist
val_run: CFLAGS += -g3
val_run: fclean static $(TESTS_OBJS)
	@$ $(CC) $(TESTS_OBJS) $(LIBS) -o $@
	@echo "$(CC) $(TESTS_OBJS) $(LIBS) -o $@ \
	["$(GREEN)"LINKING OK"$(NO_COLOR)"]"
	valgrind --trace-children=yes ./$@
	@$(RM) $@
	@$(RM) $(TESTS_OBJS)

clean:
	$(RM) $(OBJS) $(TESTS_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

install: re
	@cp $(NAME) /usr/lib/$(NAME) 2> /dev/null || \
	printf "\033[1m\033[31mError : try sudo make install\033[0m\n" && \
	cp include/clist.h /usr/include/ 2> /dev/null && \
	printf "\033[1m\033[32mLibrary successfull installed !\033[0m\n"

static: $(OBJS)
	$(ARCHIVER) rc $(NAME:.so=.a) $(OBJS)

.PHONY: tests_run val_run clean fclean install
