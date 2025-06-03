NAME		:=	computorv2
NAME_BONUS	:=	computor_bonus

SRCS		:= 	main.cpp \
				Computor.cpp \

SRCS_BONUS	:=	Polynome_bonus.cpp \
				main_bonus.cpp \

DIR			:=	srcs/

DIR_BONUS	:=	srcs/bonus/

OBJS		:=	$(patsubst %.cpp, $(DIR)%.o, $(SRCS))
OBJS_BONUS	:=	$(patsubst %.cpp, $(DIR_BONUS)%.o, $(SRCS_BONUS))

CC			:= c++

FLAGS 		:= -Wall -Werror -Wextra -g -I ./includes/
FLAGS_BONUS := -Wall -Werror -Wextra -g -I ./includes/bonus/

TPUT 					= tput -T xterm-256color
_RESET 					:= $(shell $(TPUT) sgr0)
_BOLD 					:= $(shell $(TPUT) bold)
_ITALIC 				:= $(shell $(TPUT) sitm)
_UNDER 					:= $(shell $(TPUT) smul)
_GREEN 					:= $(shell $(TPUT) setaf 2)
_YELLOW 				:= $(shell $(TPUT) setaf 3)
_RED 					:= $(shell $(TPUT) setaf 1)
_GRAY 					:= $(shell $(TPUT) setaf 8)
_PURPLE 				:= $(shell $(TPUT) setaf 5)

OBJS_TOTAL	= $(words $(OBJS))
N_OBJS		:= $(shell find $(DIR) -type f -name $(OBJS) 2>/dev/null | wc -l)
OBJS_TOTAL	:= $(shell echo $$(( $(OBJS_TOTAL) - $(N_OBJS) )))
CURR_OBJ	= 0

all: ${NAME}

${NAME}: ${OBJS}
	@${CC} ${FLAGS} -o ${NAME} ${OBJS} -lreadline
	@printf "$(_BOLD)$(NAME)$(_RESET) compiled $(_GREEN)$(_BOLD)successfully$(_RESET)\n\n"

${DIR}%.o: ${DIR}%.cpp
	@${CC} ${FLAGS} -o $@ -c $<
	@$(eval CURR_OBJ=$(shell echo $$(( $(CURR_OBJ) + 1 ))))
	@$(eval PERCENT=$(shell echo $$(( $(CURR_OBJ) * 100 / $(OBJS_TOTAL) ))))
	@printf "$(_GREEN)($(_BOLD)%3s%%$(_RESET)$(_GREEN)) $(_RESET)Compiling $(_BOLD)$(_PURPLE)$<$(_RESET)\n" "$(PERCENT)"

${DIR_BONUS}%.o: ${DIR_BONUS}%.cpp
	@${CC} ${FLAGS_BONUS} -o $@ -c $<
	@$(eval CURR_OBJ=$(shell echo $$(( $(CURR_OBJ) + 1 ))))
	@$(eval PERCENT=$(shell echo $$(( $(CURR_OBJ) * 100 / $(OBJS_TOTAL) ))))
	@printf "$(_GREEN)($(_BOLD)%3s%%$(_RESET)$(_GREEN)) $(_RESET)Compiling $(_BOLD)$(_PURPLE)$<$(_RESET)\n" "$(PERCENT)"

val :
	@bash -c 'valgrind --leak-check=full --leak-check=full --suppressions=readline.supp ./computorv2'

bonus: ${OBJS_BONUS}
	@${CC} ${FLAGS_BONUS} -o ${NAME_BONUS} ${OBJS_BONUS}
	@printf "$(_BOLD)$(NAME_BONUS)$(_RESET) compiled $(_GREEN)$(_BOLD)successfully$(_RESET)\n\n"

clean:
	@rm -rf ${OBJS} ${OBJS_BONUS}
	@printf "\n$(_BOLD)All objects are $(_GREEN)cleaned $(_RESET)! 🎉\n\n"

fclean: clean
	@rm -f ${NAME} ${NAME_BONUS}
	@printf "Cleaned $(_BOLD)$(NAME)$(_RESET) !\n\n"

re: fclean all

.PHONY: clean fclean re all
