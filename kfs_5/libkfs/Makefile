NAME = libkfs
NAME_DIR = .

RM = /bin/rm
KFS_BUILD_DIR = isodir
KFS_CONFIG_DIR = config

OPT_FLAG = -g3 -Og  

DOCKER_DIR = /app
DOCKER = /usr/bin/docker
DOCKER_CONT_NAME = rertzer/crosscompiler_i386
DOCKER_EXEC = $(DOCKER) run --rm -v $(NAME_DIR):$(DOCKER_DIR) $(DOCKER_CONT_NAME)

FLAG_DEBUG		= -g -ggdb3
FLAG_WARNING	= -Wall -Wextra -Winline -Wformat
FLAG			= $(FLAG_DEBUG) $(FLAG_WARNING) $(OPT_FLAG) -DJROS 

SRCS_DIR = $(NAME_DIR)/srcs
OBJS_DIR = $(NAME_DIR)/objs
INCS_DIR = $(NAME_DIR)/incs
DEPS_DIR = $(NAME_DIR)/deps

SOURCES = kfs_atoi.c \
		  kfs_itoa.c \
		  kfs_ctype.c \
		  kfs_strlen.c \
		  kfs_strrchr.c \
		  kfs_strncmp.c \
		  kfs_memcmp.c \
		  kfs_memcpy.c \
		  kfs_memset.c

SOURCEHEADER_FILES = libkfs.h 

SRCS = $(addprefix $(SRCS_DIR)/, $(SOURCES))

OBJS = $(patsubst $(SRCS_DIR)/%.c,$(OBJS_DIR)/%.o,$(patsubst $(SRCS_DIR)/%.s,$(OBJS_DIR)/%.o,$(SRCS)))
DEPS = $(patsubst $(SRCS_DIR)/%.c,$(DEPS_DIR)/%.d,$(patsubst $(SRCS_DIR)/%.s,$(DEPS_DIR)/%.d,$(SRCS)))
INCS = -I $(INCS_DIR)
INCS_FILES = $(addprefix $(INCS_DIR)/, $(HEADER_FILES))

ifneq ($(shell $(DOCKER) image list --format '{{.Repository}}' | grep -w $(DOCKER_CONT_NAME)),)
all: $(NAME).a
else
all:
	@echo "Image '$(DOCKER_CONT_NAME)' does not exist."
	@echo "Pulling '$(DOCKER_CONT_NAME)' from Docker Hub."
	@docker pull $(DOCKER_CONT_NAME)
endif

$(NAME).a: $(OBJS)
	@echo "creating libkfs archive..."
	@$(DOCKER_EXEC) ar cr $@ $(OBJS)

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.s $(INCS_FILES) 
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@$(DOCKER_EXEC) nasm -g -f elf32 $(INCS) $< -o $@

$(OBJS_DIR)/%.o : $(SRCS_DIR)/%.c $(INCS_FILES)
	@echo "Compiling $(notdir $<)..."
	@mkdir -p $(dir $@)
	@mkdir -p $(patsubst $(OBJS_DIR)/%,$(DEPS_DIR)/%,$(dir ./$(@:.o=.d)))
	@$(DOCKER_EXEC) i386-elf-gcc $(INCS) \
						-MMD -MF /app/$(patsubst $(SRCS_DIR)/%,$(DEPS_DIR)/%,./$(<:.c=.d)) \
						$(FLAG) \
						-std=gnu99 -ffreestanding -Og \
						-c /app/$< \
						-o /app/$@


clean:
	@echo "Cleaning libkfs files..."
	@$(RM) -rf $(OBJS_DIR)
	@$(RM) -rf $(DEPS_DIR)

fclean: clean
	@echo "Cleaning libkfs archive..."
	@$(RM) -f $(NAME).a

re: fclean all

build_docker:
	@$(DOCKER) build ./config -t $(DOCKER_CONT_NAME)


-include $(DEPS)

.PHONY: all clean fclean re build_docker
