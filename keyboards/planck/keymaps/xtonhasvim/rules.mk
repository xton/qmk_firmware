SRC += muse.c


TRACKBALL_ENABLED = yes 

# the line for each direction (numeric part of pin)
TB_LINE_UP = 8
TB_LINE_DN = 3
TB_LINE_LT = 4
TB_LINE_RT = 5

# the pad for each direction (a letter)
TB_PAD_UP = B
TB_PAD_DN = B
TB_PAD_LT = B
TB_PAD_RT = B



#OPT_DEFS += -DHAL_USE_EXT=TRUE
CFLAGS += -DHAL_USE_EXT=TRUE
