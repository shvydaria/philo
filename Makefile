NAME = philo
CC = cc 
CFLAG = -Wall -Werror -Wextra -pthread

SRCS =	main.c \
		philo.c \
		routine.c \
		utils.c \
		messages.c \
		atoi.c

OBJS =	$(SRCS:.c=.o)

ifeq ($(SAN), 1)
    SANITIZER = -g -fsanitize=address -fsanitize=thread
else
    SANITIZER =
endif
# make SAN=1 for using sanitizer
make SAN=1

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAG) -o $(NAME) $(OBJS) $(SANITIZER)

%.o: %.c philo.h
	$(CC) $(CFLAG) -c $< -o $@

clean:
		rm -rf $(OBJS)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
