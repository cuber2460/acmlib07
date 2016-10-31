BIN = bin
PDF = lib.pdf
CODE_DIR = code

INFO = $(BIN)/lib.info
TMP_PDF = $(BIN)/tmp_lib.pdf
TMP_INFO = $(BIN)/tmp_lib.info
DATE = $(BIN)/date

$(PDF): $(TMP_PDF) $(INFO)
	pdftk $(TMP_PDF) update_info $(INFO) output $@

$(TMP_PDF): | $(BIN)
	python3 builder.py $(CODE_DIR) $(TMP_PDF)

$(INFO): $(TMP_INFO) $(DATE)
	sed 's/InfoBegin\rInfoKey:[^\r]*\rInfoValue:[^\r]*\r//g' < $< > $(TMP_INFO)_1
	/bin/echo -en "\r" >> $(TMP_INFO)_1
	/bin/echo -en "InfoBegin\rInfoKey: Title\rInfoValue: Biblioteczka dru&#380;yny KMR.\r" >>  $(TMP_INFO)_1
	/bin/echo -en "InfoBegin\rInfoKey: Creator\rInfoValue: Marek Sommer\r" >>  $(TMP_INFO)_1
	/bin/echo -en "InfoBegin\rInfoKey: Producer\rInfoValue: Marek Sommer\r" >>  $(TMP_INFO)_1
	/bin/echo -en "InfoBegin\rInfoKey: Author\rInfoValue: Marek Sommer, Kamil D&#281;bowski, Mateusz Radecki\r" >>  $(TMP_INFO)_1
	/bin/echo -en "InfoBegin\rInfoKey: CreationDate\rInfoValue: " >>  $(TMP_INFO)_1
	cat $(DATE) | tr '\r' '\n' >> $(TMP_INFO)_1
	/bin/echo -en "InfoBegin\rInfoKey: ModDate\rInfoValue: " >>  $(TMP_INFO)_1
	cat $(DATE) | tr '\r' '\n' >> $(TMP_INFO)_1
	/bin/echo -en "InfoBegin\rInfoKey: Subject\rInfoValue: Biblioteczka ACM.\r" >>  $(TMP_INFO)_1
	/bin/echo -en "InfoBegin\rInfoKey: Keywords\rInfoValue: biblioteczka acm kmr marek sommer kamil d&#281;bowski mateusz radecki.\r" >>  $(TMP_INFO)_1
	cat $(TMP_INFO)_1 | tr '\r' '\n' > $@

$(DATE): | $(BIN)
	date +"D:%Y%m%d%H%M%S%z" | sed "s/\+\([0-9][0-9]\)\([0-9][0-9]\)/\+\1\'\2\'/g" > $@

$(TMP_INFO): $(TMP_PDF)
	pdftk $^ dump_data | tr '\n' '\r' > $@

$(BIN):
	mkdir -p $(BIN)

.PHONY: clean
clean:
	rm $(BIN) __pycache__ -rf

.PHONY: cleanall
cleanall: clean
	rm $(PDF) -rf
