DIST_DIR  ?= $(CURDIR)/dist
BUILD_DIR ?= $(CURDIR)/code/build

TEMPLATE := $(CURDIR)/../template.typ
REFS     := refs.bib
REPORTS  := assignment-01
PDFS     := $(addprefix $(DIST_DIR)/, $(addsuffix .pdf, $(REPORTS)))
BIN      := $(DIST_DIR)/assignment-01

SRC_FILES  := assignment-01-uml.pdf assignment-01.mp4
DEST_FILES := $(addprefix $(DIST_DIR)/, $(SRC_FILES))

.PHONY: all prune clean release

all: $(PDFS) $(BIN) $(DEST_FILES)

$(DIST_DIR)/%.pdf: %.typ $(TEMPLATE) $(REFS) | $(DIST_DIR)/
	typst c --root $(realpath $(dir $(TEMPLATE))) $< $@

$(BIN): | $(DIST_DIR)/
	cmake -S code -B $(BUILD_DIR) -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=$(DIST_DIR)
	cmake --build $(BUILD_DIR)

$(DEST_FILES): $(DIST_DIR)/%: % | $(DIST_DIR)/
	cp $< $@

%/:
	mkdir -p $@

prune:
	rm -rf $(BUILD_DIR)

clean: prune
	rm -rf $(DIST_DIR)

release: clean all

