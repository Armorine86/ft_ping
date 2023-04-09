NAME 				=	ft_ping

INC_PATH			=	includes/
OBJS_PATH 			=	obj/
SRCS_PATH 			=	src/
LIBFT_PATH			= 	libft/
LIBFT_INC_PATH		=	$(LIBFT_PATH)/includes/

CC 					=	clang
CFLAGS				=	-Wall -Werror -Wextra -Wpedantic
RM					=	rm -rf

INC_FILES 			=	ft_flags.h ft_socket.h utils.h
LIBFT_INC_FILES		=	libft.h

SRCS_FILES			=	main.c

SOCKET_FILES		=	socket.c addrinfo.c
SOCKET_HDRS			=	ft_socket.h

DNS_FILES			=	
DNS_HDRS			=	
				
UTILS_FILES			= 	error_exit.c cmd_parser.c cleanup.c utils.c
UTILS_HDRS			= 	utils.h

PING_FILES			=	ping.c
PING_HDRS			=	ping.h

DNS_PATH			= 	$(SRCS_PATH)dns/
SOCKET_PATH			=	$(SRCS_PATH)socket/
UTILS_PATH			=	$(SRCS_PATH)utils/
PING_PATH			=	$(SRCS_PATH)ping/

SRCS 				=	$(addprefix $(SRCS_PATH), $(SRCS_FILES))
DNS_SRCS			= 	$(addprefix $(DNS_PATH), $(DNS_FILES))
SOCKET_SRCS			= 	$(addprefix $(SOCKET_PATH), $(SOCKET_FILES))
UTILS_SRCS			=	$(addprefix $(UTILS_PATH), $(UTILS_FILES))
PING_SRCS			=	$(addprefix $(PING_PATH), $(PING_FILES))

OBJS_FILES			= 	$(SRCS_FILES:.c=.o) $(DNS_FILES:.c=.o) $(UTILS_FILES:.c=.o)\
						$(SOCKET_FILES:.c=.o) $(PING_FILES:.c=.o)

INCLUDES			=	$(addprefix $(INC_PATH), $(INC_FILES)) $(addprefix $(LIBFT_INC_PATH), $(LIBFT_INC_FILES))
OBJS 				=	$(addprefix $(OBJS_PATH), $(OBJS_FILES))

VPATH				=	$(SRCS_PATH) $(DNS_PATH) $(UTILS_PATH) $(SOCKET_PATH) $(PING_PATH)

ALL_INCLUDES		= 	-I$(INC_PATH)\
						-I$(LIBFT_INC_PATH)\
						-I$(DNS_PATH)\
						-I$(SOCKET_PATH)\
						-I$(UTILS_PATH)\
						-I$(PING_PATH)\

LIBFT				= 	$(MAKE) release -C $(LIBFT_PATH)

USAGE				=	./ft_ping [-vh / -v -h] [hostname/IP] [packetSize]

$(OBJS_PATH)%.o: %.c
	@$(CC) $(CFLAGS) $(ALL_INCLUDES) -c $< -o $@
	@printf "\033[93m▓▓▓\033[0m"

all:	$(NAME)

$(NAME):	$(OBJS_PATH) $(OBJS) 
	@$(LIBFT)
	@$(CC) $(OBJS) -L$(LIBFT_PATH) -lft -o $(NAME)
	@echo "\\n\033[32;1mProgram Ready\033[0m \\n"

$(OBJS_PATH):
	@mkdir -p $(OBJS_PATH)
	@echo "\033[34;1mCreating Object Directory\n \033[0m"
	@printf "\033[32;1m\nCompiling with: \033[38;5;208m$(CFLAGS)\033[0m\\n\\n"


# linux :	$(OBJS_PATH) $(OBJS)
# 	@echo "\033[95mCompiling Program \033[0m"
# 	$(CC) $(OBJS) -o $(NAME)
# 	@echo "\\n\033[32;1mProgram IS READY \033[0m \\n"
# 	@echo $(USAGE)

# linux_debug: CFLAGS += -g -fstandalone-debug -DDEBUG=1 -fno-limit-debug-info
# linux_debug: linux

# linux_redebug : fclean linux_debug


debug:	CFLAGS += -g -fstandalone-debug -fno-limit-debug-info
debug:	$(NAME)

redebug : fclean debug

usage:
	@echo $(USAGE)

# Optimization flag for a small performance boost to your programm
# If you have errors compiling with this flag reduce to -O1 or just compile using make
release: CFLAGS += -O2
release: all

clean:
	@$(RM) $(OBJS_FILES) $(OBJS_PATH) 
	@make -C $(LIBFT_PATH) clean
	@echo "\033[34;1mCLEAN DONE\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@make -C $(LIBFT_PATH) fclean
	@echo "\033[34;1mFCLEAN DONE\033[0m"

re: fclean all

redebug: fclean debug

.PHONY: all clean fclean re linux debug redebug
