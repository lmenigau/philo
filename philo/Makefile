SRC		=  main.c parse_int.c
OBJ		= $(SRC:.c=.o)
CFLAGS	= -O3 -g -Wall -Wextra 
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
