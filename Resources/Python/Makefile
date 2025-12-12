#Package with pip

all:
	rm -f Build/MARTe
	ln -s $(PWD)/Source Build/MARTe
	pip3 install --upgrade .

PYTHON ?= python3
PYTHON_MAJOR := $(shell $(PYTHON) -c "import sys; print(sys.version_info.major)")
PYTHON_MINOR := $(shell $(PYTHON) -c "import sys; print(sys.version_info.minor)")
PYTHON_VERSION := $(PYTHON_MAJOR).$(PYTHON_MINOR)

rpm:
	rm -f Build/MARTe
	ln -s $(PWD)/Source Build/MARTe
	pip3 wheel .
	mkdir -p rpmbuild
	mkdir -p rpmbuild/SPECS rpmbuild/SOURCES rpmbuild/BUILD rpmbuild/BUILDROOT rpmbuild/RPMS rpmbuild/SRPMS
	cp marte2-python.spec rpmbuild/SPECS/
	cp MARTe*.whl rpmbuild/SOURCES
	rpmbuild -ba rpmbuild/SPECS/marte2-python.spec --define "_topdir $(PWD)/rpmbuild" --define "python_ver $(PYTHON_VERSION)" --define "pyparsing_dep python3-pyparsing"

rpm-ccs-3.11:
	rm -f Build/MARTe
	ln -s $(PWD)/Source Build/MARTe
	pip3.11 wheel .
	mkdir -p rpmbuild
	mkdir -p rpmbuild/SPECS rpmbuild/SOURCES rpmbuild/BUILD rpmbuild/BUILDROOT rpmbuild/RPMS rpmbuild/SRPMS
	cp marte2-python.spec rpmbuild/SPECS/
	cp MARTe*.whl rpmbuild/SOURCES
	rpmbuild -ba rpmbuild/SPECS/marte2-python.spec --define "_topdir $(PWD)/rpmbuild" --define "python_ver 3.11" --define "pyparsing_dep $(CODAC_RPM_PREFIX)-python3.11-pyparsing"

clean:
	rm -f *.whl
	rm -f Build/MARTe
	rm -rf rpmbuild
	pip3 uninstall MARTe
