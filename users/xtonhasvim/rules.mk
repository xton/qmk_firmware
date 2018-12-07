SRC += xtonhasvim.c
SRC += fancylighting.c
SRC += trackball.c

ifeq ($(strip $(TRACKBALL_ENABLED)), yes)
    OPT_DEFS += -DTRACKBALL_ENABLED
    OPT_DEFS += -DTB_LINE_UP=$(TB_LINE_UP)
    OPT_DEFS += -DTB_LINE_DN=$(TB_LINE_DN)
    OPT_DEFS += -DTB_LINE_LT=$(TB_LINE_LT)
    OPT_DEFS += -DTB_LINE_RT=$(TB_LINE_RT)
    OPT_DEFS += -DTB_PAD_UP=$(TB_PAD_UP)
    OPT_DEFS += -DTB_PAD_DN=$(TB_PAD_DN)
    OPT_DEFS += -DTB_PAD_LT=$(TB_PAD_LT)
    OPT_DEFS += -DTB_PAD_RT=$(TB_PAD_RT)
    ifeq ($(strip $(TB_INTERRUPT_ENABLED)), yes)
      OPT_DEFS += -DTB_INTERRUPT_ENABLED
      ifeq ($(PLATFORM),AVR)
          # nothing yet...
      else
          CFLAGS += -DHAL_USE_EXT=TRUE
      endif
    endif

endif

