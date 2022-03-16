CC=clang++
RM=rm -rf
NAME=ft_kalman

SRCS_DIR=srcs
INCS_DIR=includes
SRCS:=$(wildcard ./$(SRCS_DIR)/*.cpp)
INCS=$(wildcard ./$(INCS_DIR)/*.hpp)
OBJS:=$(SRCS:.cpp=.o)

FSANITIZE=-fsanitize=address
FLAGS=-Wall -Wextra -Werror -g $(FSANITIZE) -I$(INCS_DIR) -std=c++17

%.o:%.cpp
	$(CC) $(FLAGS) -c $< -o $@ -fsanitize=address

all: $(NAME)

$(OBJS): $(INCS)

$(NAME): $(OBJS) $(INCS)
	$(CC) $(FLAGS) $(OBJS) -o $(NAME)

re: fclean all

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

.PHONY: all clean fclean re
