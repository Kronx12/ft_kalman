CC=clang++
RM=rm -rf
NAME=ft_kalman

SRCS_DIR=srcs
INCS_DIR=includes
SRCS:=$(wildcard ./$(SRCS_DIR)/*.cpp)
INCS=$(wildcard ./$(INCS_DIR)/*.hpp)
OBJS:=$(SRCS:.cpp=.o)

FSANITIZE=-fsanitize=address
FLAGS=-Wall -Wextra -Werror -std=c++17 -I/usr/include/python2.7 -g $(FSANITIZE) -I$(INCS_DIR) -D WITHOUT_NUMPY

%.o:%.cpp
	$(CC) $(FLAGS) -c $< -o $@ -fsanitize=address

all: $(NAME)

$(OBJS): $(INCS)

$(NAME): $(OBJS) $(INCS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME) -lpython2.7

re: fclean all

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

.PHONY: all clean fclean re
