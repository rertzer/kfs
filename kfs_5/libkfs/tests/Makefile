CC := cc
FLAGS := -Wall -Wextra -Werror
CRITERION_FLAGS = -Wl,-rpath=$(SRC_DIR_CRITERION) -L$(SRC_DIR_CRITERION)
 
NAME := libkfs
LIB_NAME := $(NAME).a
NAME_TEST := $(NAME)_tests


SRC_DIR := ../srcs/
INC_DIR := ../incs/
OBJ_DIR := obj/
SRC_DIR_TEST := src_test/
OBJ_DIR_TEST := obj_test/
INC_DIR_TEST := include_test/
INC_DIR_CRITERION := /home/rertzer/Criterion/include/criterion
SRC_DIR_CRITERION := /home/rertzer/Criterion/build/src
LIB_CRITERION := -lcriterion

SOURCES :=  strlen.c

SOURCES_TEST := strlen_test.c

OBJ := $(SOURCES:.c=.o)
OBJ_TEST := $(SOURCES_TEST:.c=.o)

OBJS := $(addprefix $(OBJ_DIR), $(OBJ))
OBJS_TEST := $(addprefix $(OBJ_DIR_TEST), $(OBJ_TEST))


DEPS := $(OBJS:.o=.d)
DEPS_TEST := $(OBJS_TEST:.o=.d)


all: $(LIB_NAME)

$(LIB_NAME): $(OBJ_DIR) $(OBJS)
	ar cr $@  $(OBJS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	ls $(SRC_DIR)
	$(CC) $(FLAGS) -ffreestanding -nostdlib  -c -MMD $< -o $@ -I $(INC_DIR)

test: $(NAME_TEST)
	./$(NAME_TEST)

$(NAME_TEST): $(OBJ_DIR) $(OBJ_DIR_TEST) $(OBJS) $(OBJS_TEST)
	@$(CC) $(FLAGS)  -o $@  $(OBJS_TEST) $(LIB_NAME) -I$(INC_DIR_CRITERION) $(CRITERION_FLAGS) $(LIB_CRITERION)


$(OBJ_DIR_TEST)%.o: $(SRC_DIR_TEST)%.c
	@$(CC) $(FLAGS)  -c  $< -o $@  -I$(INC_DIR_CRITERION) -I$(INC_DIR) -I$(INC_DIR_TEST)


$(OBJ_DIR):
	@mkdir $(OBJ_DIR)

$(OBJ_DIR_TEST):
	@mkdir $(OBJ_DIR_TEST)

clean:
	@rm -f $(MAIN_OBJS)
	@rm -f $(OBJS)
	@rm -f $(OBJS_TEST)
	@rm -f $(MAIN_DEPS)
	@rm -f $(DEPS)
	@rm -f $(DEPS_TEST)
	@rm -fd $(OBJ_DIR)
	@rm -fd $(OBJ_DIR_TEST)

fclean: clean
	@rm -f $(NAME)
	@rm -f $(NAME_TEST)

re: fclean all

PHONY: all clean fclean re test
