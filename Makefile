MAKEFLAGS += --no-print-directory

NAME = philo

CC = gcc
FLAGS = -Wextra -Werror -Wall -pthread -g

OBJDIR := obj

SRCDIR := srcs

SRCS := main.c parsing.c \
	init_structs.c philo_routine.c simulation.c \
	utils.c cleanup.c \
	time.c print.c\
	test.c

OBJS := $(SRCS:%.c=$(OBJDIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(FLAGS) $(OBJS) -o $@
	@echo "🧰 philosophers are philosophing... 🧰"


$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR)

fclean: clean
	@rm -f $(NAME)
	@echo "🧹 Adios, philosophuckers 🧹"

re: fclean all

.PHONY: all clean fclean re bonus