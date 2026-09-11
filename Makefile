DIST_DIR ?= $(CURDIR)/dist

MODULES       := assignment-01 assignment-02
ALL_TARGETS   := $(addprefix all-,$(MODULES))
PRUNE_TARGETS := $(addprefix prune-,$(MODULES))
CLEAN_TARGETS := $(addprefix clean-,$(MODULES))

SRC_FILES  := brief.pdf feedback.pdf demo.gif
DEST_FILES := $(addprefix $(DIST_DIR)/, $(SRC_FILES))

.PHONY: all prune clean release $(ALL_TARGETS) $(PRUNE_TARGETS) $(CLEAN_TARGETS)

all: $(ALL_TARGETS) $(DEST_FILES)

$(ALL_TARGETS): all-%:
	$(MAKE) -C $* all DIST_DIR=$(DIST_DIR)/$*

$(DEST_FILES): $(DIST_DIR)/%: % | $(DIST_DIR)
	cp $< $@

prune: $(PRUNE_TARGETS)

$(PRUNE_TARGETS): prune-%:
	$(MAKE) -C $* prune

clean: $(CLEAN_TARGETS) prune
	rm -rf $(DIST_DIR)/$*

release: clean all

$(CLEAN_TARGETS): clean-%:
	$(MAKE) -C $* clean DIST_DIR=$(DIST_DIR)

