# add here all files
docs_DATA = index.html index-1.html index-2.html index-3.html \
	   index-4.html index-5.html index-6.html index-7.html \
	   index-8.html index-9.html index-10.html \
	   hayes.html logotp3.png  

# this is the directory, where all datas are installed
# you have given the datas in data_DATA
docsdir = $(kde_htmldir)/en/kppp

EXTRA_DIST = $(docs_DATA) index.sgml index.nif

install-data-local:
	rm -f $(pkgdir)/index.html
	$(LN_S) kppp.html $(pkgdir)/index.html

doc: kppp.sgml
	ksgml2html index en
