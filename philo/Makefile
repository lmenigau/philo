SRC		=  main.c parse_int.c philo.c take_fork.c
OBJ		= $(SRC:.c=.o)
CFLAGS	= -Wall -Wextra -Werror
NAME	= philo
CC		= clang
ifeq  '$(shell ar V 2>/dev/null | head -c 3)' 'GNU'
	ARFLAGS	= Ur
endif

$(NAME) : $(OBJ) 
	$(CC)  -o $(NAME) $(CFLAGS) $(OBJ) -lpthread 

.PHONY	: re clean fclean all

fclean	: clean
		$(RM) $(NAME)

clean	:
		$(RM) *.o

re		: fclean  all

all		: $(NAME)
