GTEX_BASE = \
  https://storage.googleapis.com/gtex_analysis_v8/rna_seq_data

GTEX_TPM_V8 = \
  GTEx_Analysis_2017-06-05_v8_RNASeQCv1.1.9_gene_tpm.gct.gz

RAW_FILES = \
  $(GTEX_TPM_V8)

PROCESSED_FILES = \
  GTEx_v8.h5

TARGETS = \
  $(addprefix data/raw/, $(RAW_FILES)) \
  $(addprefix data/processed/, $(PROCESSED_FILES))


.PHONY: all clean

all: $(TARGETS)
	@:

data/raw/GTEx_Analysis_%:
	curl -fsSL -o '$@' '$(GTEX_BASE)/$(notdir $@)'

src/gct_to_hdf5/main:
	make -C src/gct_to_hdf5

data/processed/GTEx_v8.h5: data/raw/$(GTEX_TPM_V8) src/gct_to_hdf5/main
	gzip -cd '$<' | src/gct_to_hdf5/main '$@'
