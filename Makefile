NAME = m_shell
LIBFT_PATH = ./lib/libft
LIBFT = $(LIBFT_PATH)/libft.a
CFLAGS = -Wall -Wextra -Werror
RL_LIBS = -L/usr/local/lib -L${HOME}/.brew/opt/readline/lib
HEADERS	= -I${HOME}/.brew/Cellar/readline/8.2.10/include/readline -I/usr/local/include -I./include 
DEBUGFLAGS = -g -fsanitize=address,undefined,integer
SRCS = \
 ./srcs/parsing/lexer.c ./srcs/parsing/lexer2.c ./srcs/parsing/parse.c \
 ./srcs/parsing/parsing.c ./srcs/parsing/ms_split.c ./srcs/parsing/parsing_utils.c \
 ./srcs/initializing/init_main.c \
 ./srcs/executing/exec_main.c \
 ./lib/get_next_line/get_next_line.c ./lib/get_next_line/get_next_line_utils.c \
 ./srcs/builtins/env_builtin.c ./srcs/builtins/builtin.c 
OBJCTS = $(SRCS:.c=.o)
RM = rm -f

all: $(NAME)

$(NAME): $(LIBFT) $(SRCS)
	cc $(CFLAGS) $(HEADERS) $(RL_LIBS) $(SRCS) $(LIBFT) -lreadline -o $(NAME)

$(LIBFT):
	make -C $(LIBFT_PATH)

debug: .debug

.debug: $(LIBFT) $(SRCS) 
	cc $(DEBUGFLAGS) $(HEADERS) $(RL_LIBS) -lreadline $(SRCS) $(LIBFT) -o debug.out
	touch .debug

%.o: %.c
	cc $(CFLAGS) -c $< -o $@

clean: 
	make clean -C $(LIBFT_PATH)
	$(RM) $(OBJCTS) $(BONUS_OBJCTS) $(PRINTF_OBJCTS)
	
fclean: clean
	make -C $(LIBFT_PATH) fclean
	$(RM) $(NAME)
	$(RM) debug.out
	$(RM) .debug
	$(RM) -r debug.out.dSYM


re: fclean all

.PHONY: all clean fclean re bonus both debug