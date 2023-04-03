NAME 				=	ft_ping

INC_PATH			=	includes/
OBJS_PATH 			=	obj/
SRCS_PATH 			=	src/
LIBFT_PATH			= 	libft/
LIBFT_INC_PATH		=	$(LIBFT_PATH)/includes/

CC 					=	clang
CFLAGS				=	-Wall -Werror -Wextra -Wpedantic
RM					=	rm -rf

INC_FILES 			=	ft_ping.h ft_flags.h ft_socket.h utils.h
LIBFT_INC_FILES		=	libft.h

SRCS_FILES			=	main.c

FOLDER_1_FILES		=	
FOLDER_1_HDRS		=	

FOLDER_2_FILES		=	socket.c
FOLDER_2_HDRS		=	
				
FOLDER_3_FILES		= 	error_exit.c cmd_parser.c utils.c
FOLDER_3_HDRS		= 	

FOLDER_4_FILES		=	
FOLDER_4_HDRS		=	

FOLDER_1_PATH		= 	$(SRCS_PATH)
FOLDER_2_PATH		=	$(SRCS_PATH)socket/
FOLDER_3_PATH		=	$(SRCS_PATH)utils/
FOLDER_4_PATH		=	$(SRCS_PATH)#folder 4

SRCS 				=	$(addprefix $(SRCS_PATH), $(SRCS_FILES))
FOLDER_1_SRCS		= 	$(addprefix $(FOLDER_1_PATH), $(FOLDER_1_FILES))
FOLDER_2_SRCS		= 	$(addprefix $(FOLDER_2_PATH), $(FOLDER_2_FILES))
FOLDER_3_SRCS		=	$(addprefix $(FOLDER_3_PATH), $(FOLDER_3_FILES))
FOLDER_4_SRCS		=	$(addprefix $(FOLDER_4_PATH), $(FOLDER_4_FILES))

OBJS_FILES			= 	$(SRCS_FILES:.c=.o) $(FOLDER_1_FILES:.c=.o) $(FOLDER_3_FILES:.c=.o)\
						$(FOLDER_2_FILES:.c=.o) $(FOLDER_4_FILES:.c=.o)

INCLUDES			=	$(addprefix $(INC_PATH), $(INC_FILES)) $(addprefix $(LIBFT_INC_PATH), $(LIBFT_INC_FILES))
OBJS 				=	$(addprefix $(OBJS_PATH), $(OBJS_FILES))

VPATH				=	$(SRCS_PATH) $(FOLDER_1_PATH) $(FOLDER_3_PATH) $(FOLDER_2_PATH) $(FOLDER_4_PATH)

ALL_INCLUDES		= 	-I$(INC_PATH)\
						-I$(LIBFT_INC_PATH)\
						-I$(FOLDER_1_PATH)\
						-I$(FOLDER_2_PATH)\
						-I$(FOLDER_3_PATH)\
						-I$(FOLDER_4_PATH)\

LIBFT				= 	$(MAKE) -C $(LIBFT_PATH)

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
